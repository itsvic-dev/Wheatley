#include "stdint.h"
#include "sys/isr.h"
#include <assert.h>
#include <sched/sched.h>
#include <sys/spinlock.h>

static sched_task_t *firstTask = NULL;
uint64_t nextTid = 0;

void sched_start(void) { assert(firstTask != NULL); }

void sched_resched(registers_t *registers) {
  // do smth
}

void sched_init(void) { isr_register_handler(48, &sched_resched); }
