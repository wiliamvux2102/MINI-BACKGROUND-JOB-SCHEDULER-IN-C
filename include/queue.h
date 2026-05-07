#ifndef QUEUE_H
#define QUEUE_H

#include "job.h"

typedef struct job_node {
    job_t* job;
    struct job_node* next;
} job_node_t;

typedef struct {
    job_node_t* front;
    job_node_t* rear;
} job_queue_t;

int queue_empty(job_queue_t* queue);

void enqueue_job(
    job_queue_t* queue,
    job_t* job
);

job_t* dequeue_job(
    job_queue_t* queue
);

#endif