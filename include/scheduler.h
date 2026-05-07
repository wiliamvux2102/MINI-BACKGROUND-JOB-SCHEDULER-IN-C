#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>
#include "queue.h"

typedef enum {
    POLICY_FIFO = 0,
    POLICY_SJF  = 1
} policy_t;

void scheduler_init(void);
void scheduler_destroy(void);

void scheduler_close(void);

int scheduler_add_job(policy_t policy, job_t *job);
job_t *scheduler_get_next_job(policy_t policy);

size_t scheduler_ready_size(void);

#endif