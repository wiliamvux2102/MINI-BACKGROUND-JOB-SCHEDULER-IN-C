#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include "job.h"

typedef struct job_node {
    job_t * job;
    struct job_node * next_job;
} job_node_t;

typedef struct {
    job_node_t * head;
    job_node_t * tail;
    size_t size;
} queue_t;

void queue_init(queue_t * q);
void queue_destroy(queue_t * q);

int queue_isempty(const queue_t * q) { return !q || q->size == 0; } // return 1/0 for true/false
size_t queue_size(const queue_t * q) { return (q ? q->size: 0); }

int enqueue(queue_t *q, job_t * j); // return 1 if success
job_t * dequeue (queue_t * q);

int sorted_insert_sjf(queue_t * q, job_t * j);

#endif

