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

void sched_start(void) {
  assert(firstTask != NULL);
  // force a free task search, then trigger scheduler
  // interrupt
  pcrb_get()->currentTask = 0;
  asm("int 48");
}

void sched_task_ended(void) { printf("hi a task ended\n"); }

static sched_task_t *find_free_task() {
  sched_task_t *task = firstTask;
  while (task != NULL) {
    // if task isn't locked, make sure we can use it
    if (spinlock_acquire(&task->lock)) {
      // if the task's runtime has expired, we can skip it
      if (task->runtime == 0) {
        spinlock_release(&task->lock);
        continue;
      }
      return task;
    }
    // otherwise, move on
    task = task->next;
  }

  return task;
}

void sched_resched(registers_t *registers) {
  pcrb_t *pcrb = pcrb_get();
  sched_task_t *task = pcrb->currentTask;
  if (!task) {
    task = find_free_task();
    pcrb->currentTask = task;
  } else {
    // save the task's registers
    task->registers = *registers;
  }
  if (task == NULL) {
    // reschedule in 10ms
    apic_eoi();
    timer_sched_oneshot(48, 10 * 1000);
    return;
  }

  // make sure we initialise task
  if (task->state == TASK_NEEDS_TO_INIT) {
    printf("resched (%d): initting task %#llx\n", pcrb->apicID, task);
    // FIXME: we're assuming every task is a kernel task
    task->registers.cs = 0x8;
    task->registers.ss = 0x10;
    task->registers.rsp = (uint64_t)kmalloc(8 * 4096) + (8 * 4096) - 8;
    task->registers.rbp = task->registers.rsp;
    *(uint64_t *)task->registers.rsp = (uint64_t)&sched_task_ended;
    task->state = TASK_RUNNING;
  }

  // apply the task registers and start execution
  uint64_t runtime = task->runtime;
  // FIXME: we need a better way of measuring task's remaining runtime
  task->runtime = 0;
  *registers = task->registers;
  apic_eoi();
  timer_sched_oneshot(48, runtime);
  return;
}

void sched_init(void) {
  isr_register_handler(48, &sched_resched);
  // set up first task for execution
  firstTask = kmalloc(sizeof(sched_task_t));
  memset(firstTask, 0, sizeof(sched_task_t));
  firstTask->runtime = 200 * 1000; // 200 ms
  firstTask->tid = nextTid++;
  firstTask->state = TASK_NEEDS_TO_INIT;
  firstTask->registers.rip = (uint64_t)&kernelTaskEntry;
}
