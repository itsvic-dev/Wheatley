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
  HALT();
  asm("int 48");
}

// void sched_task_ended(void) { pcrb_t *pcrb = pcrb_get(); }

static sched_task_t *find_free_task() {
  sched_task_t *task = firstTask;
  while (task != NULL) {
    // if task isn't locked, process it
    // otherwise, move on
    task = task->next;
  }

  return task;
}

void sched_resched(registers_t *registers) {
  pcrb_t *pcrb = pcrb_get();
  printf("pcrb @ %#llx\n", pcrb);
  sched_task_t *task = pcrb->currentTask;
  if (!task)
    task = find_free_task();
  printf("resched (%d): will handle task %#llx\n", pcrb->apicID, task);
  apic_eoi();
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
}
