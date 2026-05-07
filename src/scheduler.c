#include "scheduler.h"
#include <pthread.h>

static queue_t ready_queue;
static pthread_mutex_t ready_mutex;
static pthread_cond_t job_available;
static int initialized = 0;
static int closed = 0;

static int insert_locked(policy_t policy, job_t *j) {
    switch (policy) {
        case POLICY_FIFO:
            return enqueue(&ready_queue, j);
        case POLICY_SJF:
            return sorted_insert_sjf(&ready_queue, j);
        case POLICY_PRIORITY:
            return sorted_insert_priority(&ready_queue, j);
        default:
            return 0;
    }
}

void scheduler_init(void) {
    queue_init(&ready_queue);
    pthread_mutex_init(&ready_mutex, NULL);
    pthread_cond_init(&job_available, NULL);
    initialized = 1;
    closed = 0;
}

void scheduler_close(void) {
    if (!initialized) return;

    pthread_mutex_lock(&ready_mutex);
    closed = 1;
    pthread_cond_broadcast(&job_available);
    pthread_mutex_unlock(&ready_mutex);
}

void scheduler_destroy(void) {
    if (!initialized) return;

    queue_destroy(&ready_queue);
    pthread_cond_destroy(&job_available);
    pthread_mutex_destroy(&ready_mutex);
    initialized = 0;
    closed = 0;
}

int scheduler_add_job(policy_t policy, job_t *j) {
    if (!initialized || !j) return 0;

    pthread_mutex_lock(&ready_mutex);

    if (closed) {
        pthread_mutex_unlock(&ready_mutex);
        return 0;
    }

    int ok = insert_locked(policy, j);
    if (ok) {
        pthread_cond_signal(&job_available);
    }

    pthread_mutex_unlock(&ready_mutex);
    return ok;
}

job_t *scheduler_get_next_job(void) {
    if (!initialized) return NULL;

    pthread_mutex_lock(&ready_mutex);

    while (queue_isempty(&ready_queue) && !closed) {
        pthread_cond_wait(&job_available, &ready_mutex);
    }

    if (queue_isempty(&ready_queue) && closed) {
        pthread_mutex_unlock(&ready_mutex);
        return NULL;
    }

    job_t *job = dequeue(&ready_queue);

    pthread_mutex_unlock(&ready_mutex);
    return job;
}

size_t scheduler_ready_size(void) {
    if (!initialized) return 0;

    pthread_mutex_lock(&ready_mutex);
    size_t n = queue_size(&ready_queue);
    pthread_mutex_unlock(&ready_mutex);
    return n;
}