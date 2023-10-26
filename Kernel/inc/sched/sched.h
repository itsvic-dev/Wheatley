#ifndef __KERNEL_SCHED_SCHED_H
#define __KERNEL_SCHED_SCHED_H

#include <sys/isr.h>
#include <sys/spinlock.h>

typedef struct sched_task {
  uint64_t usLeft;
  uint64_t tid;
  uint8_t state;
  registers_t registers;
  spinlock_t lock;
  struct sched_task *next;
} sched_task_t;

// should not be ran from deep main
void sched_init(void);

// starts the scheduler loop
void sched_start(void);

#endif
