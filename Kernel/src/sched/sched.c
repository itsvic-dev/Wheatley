#include "drivers/timer/apic.h"
#include "libk.h"
#include "mm/mm.h"
#include "stdint.h"
#include "sys/apic.h"
#include "sys/isr.h"
#include "sys/pcrb.h"
#include <assert.h>
#include <printf.h>
#include <sched/kernel.h>
#include <sched/sched.h>
#include <sys/spinlock.h>

#define HALT()                                                                 \
  for (;;)                                                                     \
    asm("hlt");

uint64_t nextTid = 0;
// a pointer to the first task, usually the main kernel task
static sched_task_t *firstTask = NULL;
static sched_task_t *lastTask = NULL;

void sched_start(void) {
  assert(firstTask != NULL);
  // force a free task search, then trigger scheduler
  // interrupt
  pcrb_get()->currentTask = 0;
  asm("int 48");
}

void sched_task_ended(void) {
  pcrb_t *pcrb = pcrb_get();
  sched_task_t *task = (sched_task_t *)pcrb->currentTask;
  assert(task != 0);
  printf("sched_task_ended with task %#llx\n", task);
  assert(task != firstTask || !"kernel task should never return");
  spinlock_wait_and_acquire(&task->lock);
  task->state = TASK_RETURNED;
  task->usLeft = 0;
  // FIXME: remove the task from the linked list, then free it
  spinlock_release(&task->lock);

  // cancel the APIC timer, then force a reschedule with a free task search
  timer_stop_sched();
  pcrb_get()->currentTask = 0;
  asm("int 48");
}

void sched_resched(registers_t *registers) {
  pcrb_t *pcrb = pcrb_get();
  sched_task_t *task = (sched_task_t *)pcrb->currentTask;
  if (!task) {
    // find a free task
    task = firstTask;
    while (task) {
      spinlock_wait_and_acquire(&task->lock);
      if ((task->state != TASK_WAITING && task->state != TASK_NEEDS_TO_INIT) ||
          task->usLeft == 0) {
        spinlock_release(&task->lock);
        task = task->next;
      } else {
        spinlock_release(&task->lock);
        break;
      }
    }
    // FIXME: if there are no free tasks left, reset all task runtimes
    pcrb->currentTask = task;
  } else {
    // we had a task assigned but its runtime has ran out
    // FIXME: is this done impl-wise?
    spinlock_wait_and_acquire(&task->lock);
    task->registers = *registers;
    spinlock_release(&task->lock);
    task = task->next;
  }

  printf("sched_resched: task=%#llx\n", task);

  // if we still dont have a task, resched in 10ms
  if (!task) {
    apic_eoi();
    timer_sched_oneshot(48, 10 * 1000);
    return;
  }

  spinlock_wait_and_acquire(&task->lock);

  // if we have a task, but it needs to be init'd, give it some stack and set
  // the CS/SS properly
  if (task->state == TASK_NEEDS_TO_INIT) {
    // FIXME: for now, we assume that everything is in kernel space so we'll
    // assign kernel segments
    task->registers.cs = 0x8;
    task->registers.ss = 0x10;
    // give it 8K of stack
    void *stack = kmalloc(8 * 4096);
    task->registers.rsp = (uint64_t)stack + (8 * 4096) - 8;
    task->registers.rflags |= 1;
    // push the return value (sched_task_ended) to the stack
    // in case the task returns before its runtime ends
    *(uint64_t *)(task->registers.rsp) = (uint64_t)&sched_task_ended;
    task->state = TASK_WAITING;
  }

  // run the task for its runtime
  uint64_t usLeft = task->usLeft;
  task->state = TASK_RUNNING;
  task->usLeft = 0; // FIXME: is there a proper way to count usLeft? what if
                    // we're interrupted in the middle of the task?
  *registers = task->registers;
  spinlock_release(&task->lock);
  apic_eoi();
  timer_sched_oneshot(48, usLeft);
}

void sched_init(void) {
  isr_register_handler(48, &sched_resched);
  // set up first task for execution
  firstTask = kmalloc(sizeof(sched_task_t));
  memset(firstTask, 0, sizeof(sched_task_t));
  firstTask->usLeft = 200 * 1000; // 200 ms
  firstTask->tid = nextTid++;
  firstTask->state = TASK_NEEDS_TO_INIT;
  firstTask->registers.rip = (uint64_t)&kernelTaskEntry;
  lastTask = firstTask;
}
