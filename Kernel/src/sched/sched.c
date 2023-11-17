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
static sched_task_t *firstTask = NULL;
static sched_task_t *lastTask = NULL;
static spinlock_t globalTaskLock = SPINLOCK_INIT;

void sched_start(void) {
  assert(firstTask != NULL);
  // force a free task search, then trigger scheduler
  // interrupt
  pcrb_get()->currentTask = 0;
  asm("int 48");
}

void sched_task_ended(void) {
  timer_stop_sched();
  asm("cli");

  pcrb_t *pcrb = pcrb_get();
  assert(pcrb->currentTask != NULL);
  assert(pcrb->currentTask != firstTask || !"first task should never return");
  sched_task_t *task = pcrb->currentTask;
  // printf("sched (%d): task %#llx returned\n", pcrb->apicID, task);
  task->state = TASK_RETURNED;
  spinlock_release(&task->lock);
  pcrb->currentTask = NULL;

  // FIXME: remove this task from the list and free it

  // reschedule again
  asm("sti; int 48");
}

static sched_task_t *find_free_task(bool didRefresh) {
  sched_task_t *task = firstTask;
  while (task != NULL) {
    // if task isn't locked, make sure we can use it
    if (spinlock_acquire(&task->lock)) {
      // if the task's runtime hasn't expired, we jump to the end so we can
      // return it
      if (task->runtime != 0) {
        goto end;
      }
      spinlock_release(&task->lock);
    }
    task = task->next;
  }

  // printf("sched: we ran out of tasks!!\n");
end:
  return task;
}

void sched_resched(registers_t *registers) {
  asm("cli");
  pcrb_t *pcrb = pcrb_get();
  sched_task_t *task = pcrb->currentTask;
  timer_stop_sched();
  // printf("sched (%d): hi, initial task=%#llx\n", pcrb->apicID, task);
  if (task != NULL) {
    // runtime for this task has (likely) run out, let's find another free task
    task->registers = *registers;
    spinlock_release(&task->lock);
    task = NULL;
  }
  if (task == NULL)
    task = find_free_task(false);

  // if task is STILL null, reschedule in 10ms
  if (task == NULL) {
    timer_sched_oneshot(48, 10 * 1000);
    apic_eoi();
    asm("sti");
    HALT();
  }
  pcrb->currentTask = task;

  // printf("sched (%d): handling task %#llx\n", pcrb->apicID, task);

  // make sure we initialise task
  if (task->state == TASK_NEEDS_TO_INIT) {
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
  timer_sched_oneshot(48, runtime);
  apic_eoi();
  asm("sti");
}

void sched_init(void) {
  isr_register_handler(48, &sched_resched);
  // set up first task for execution
  spinlock_wait_and_acquire(&globalTaskLock);
  firstTask = kmalloc(sizeof(sched_task_t));
  memset(firstTask, 0, sizeof(sched_task_t));
  firstTask->runtime = 10 * 1000; // 200 ms
  firstTask->tid = nextTid++;
  firstTask->state = TASK_NEEDS_TO_INIT;
  firstTask->registers.rip = (uint64_t)&kernelTaskEntry;
  lastTask = firstTask;
  spinlock_release(&globalTaskLock);
}

void sched_spawn_kernel_task(void *entrypoint) {
  spinlock_wait_and_acquire(&globalTaskLock);
  sched_task_t *newTask = kmalloc(sizeof(sched_task_t));
  memset(newTask, 0, sizeof(sched_task_t));
  newTask->runtime = 10 * 1000;
  newTask->tid = nextTid++;
  newTask->state = TASK_NEEDS_TO_INIT;
  newTask->registers.rip = (uint64_t)entrypoint;
  lastTask->next = newTask;
  lastTask = newTask;
  spinlock_release(&globalTaskLock);
}
