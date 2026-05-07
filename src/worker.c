#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/worker.h"
#include "../include/job.h"
#include "../include/queue.h"
#include "../include/synchronization.h"

extern queue_t ready_queue;
static worker_t* workers = NULL;

static void execute_job(worker_t* worker, job_t* job) {
    worker->busy = 1;
    job->status = JOB_RUNNING;
    printf("[WORKER %d] Start Job %d\n", worker->worker_id, job->job_id);
    usleep(job->estimated_runtime * 100000);
    job->status = JOB_DONE;
    worker->jobs_completed++;
    printf("[WORKER %d] Finish Job %d\n", worker->worker_id, job->job_id);
    worker->busy = 0;
}

void* worker_loop(void* arg) {
    worker_t* worker = (worker_t*)arg;
    while (system_running) {
        pthread_mutex_lock(&queue_mutex);
        while (queue_isempty(&ready_queue) && system_running) {
            pthread_cond_wait(&job_available, &queue_mutex);
        }
        if(!system_running) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        job_t* job = dequeue(&ready_queue);
        pthread_mutex_unlock(&queue_mutex);
        if(job != NULL) {
            execute_job(worker, job);
        }
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
        pthread_create(&workers[i].thread,NULL, worker_loop, &workers[i]);
    }
}

void destroy_workers(int num_workers)
{
    pthread_mutex_lock(&queue_mutex);
    system_running = 0;
    pthread_cond_broadcast(&job_available);
    pthread_mutex_unlock(&queue_mutex);
    for(int i = 0; i < num_workers; i++) {
        pthread_join(workers[i].thread, NULL);
    }
    free(workers);
    workers = NULL;
}