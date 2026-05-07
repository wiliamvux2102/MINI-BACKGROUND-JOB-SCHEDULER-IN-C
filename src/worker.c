#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/worker.h"
#include "../include/job.h"
#include "../include/scheduler.h"

extern int logger_sim_time(void);
extern void log_worker_start(int worker_id, const job_t *job);
extern void log_worker_finish(int worker_id, const job_t *job);
static worker_t* workers = NULL;

static void dispatcher_assign_job(worker_t* worker, job_t* job) {
    worker->busy = 1;
    job->status = JOB_RUNNING;
    job->start_time = logger_sim_time();
    log_worker_start(worker->worker_id, job);
    usleep(job->estimated_runtime * 100000);
    job->finish_time = logger_sim_time();
    job->status = JOB_DONE;
    worker->jobs_completed++;
    worker->total_busy_time += job->estimated_runtime;
    log_worker_finish(worker->worker_id, job);
    worker->busy = 0;
}

void* worker_loop(void* arg) {
    worker_t* worker = (worker_t*)arg;
    while (1) {
        job_t* job = scheduler_get_next_job();
        if (job == NULL) {
            break;
        }
        dispatcher_assign_job(worker, job);
    }
    return NULL;
}

void init_workers(int num_workers)
{
    workers = malloc(sizeof(worker_t) * num_workers);
    if(workers == NULL) {
        perror("Failed to allocate workers");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < num_workers; i++)
    {
        workers[i].worker_id = i;
        workers[i].busy = 0;
        workers[i].jobs_completed = 0;
        workers[i].total_busy_time = 0.0;
        pthread_create(&workers[i].thread, NULL, worker_loop, &workers[i]);
    }
}

void destroy_workers(int num_workers)
{
    for(int i = 0; i < num_workers; i++) {
        pthread_join(workers[i].thread, NULL);
    }
    free(workers);
    workers = NULL;
}