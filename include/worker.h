#ifndef WORKER_H
#define WORKER_H

#include <pthread.h>

typedef struct {
    int worker_id;
    pthread_t thread;
    int busy;
    int jobs_completed;
    double total_busy_time;
} worker_t;

void init_workers(int num_workers);

void destroy_workers(int num_workers);

void* worker_loop(void* arg);

#endif