#ifndef __KERNEL_SCHED_SCHED_H
#define __KERNEL_SCHED_SCHED_H

#include <sys/isr.h>
#include <sys/spinlock.h>

typedef enum {
  TASK_NEEDS_TO_INIT,
  TASK_WAITING,
  TASK_RUNNING,
  TASK_RETURNED
} task_state_t;

typedef struct sched_task {
  uint64_t runtime;
  uint64_t tid;
  task_state_t state;
  registers_t registers;
  spinlock_t lock;
  struct sched_task *next;
} sched_task_t;

// should not be ran from deep main
void sched_init(void);

// starts the scheduler loop
void sched_start(void);

void sched_spawn_kernel_task(void *entrypoint);

#endif
