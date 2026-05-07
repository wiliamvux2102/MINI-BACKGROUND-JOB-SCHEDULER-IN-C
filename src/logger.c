#include <stdio.h>
#include <time.h>
#include "../include/job.h"

static struct timespec logger_start_time;
static double logger_scale_usec = 100000.0;
static int logger_initialized = 0;

void logger_init(double scale_usec) {
    if (scale_usec > 0.0) {
        logger_scale_usec = scale_usec;
    }
    clock_gettime(CLOCK_MONOTONIC, &logger_start_time);
    logger_initialized = 1;
}

int logger_sim_time(void) {
    if (!logger_initialized) {
        return 0;
    }

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long seconds = now.tv_sec - logger_start_time.tv_sec;
    long nsec = now.tv_nsec - logger_start_time.tv_nsec;
    if (nsec < 0) {
        seconds -= 1;
        nsec += 1000000000L;
    }

    double elapsed_usec = seconds * 1e6 + nsec / 1e3;
    int sim_time = (int)(elapsed_usec / logger_scale_usec + 0.5);
    return sim_time < 0 ? 0 : sim_time;
}

void log_worker_start(int worker_id, const job_t *job) {
    printf("[time=%d] Worker %d starts Job %d seller=%s runtime=%d priority=%d\n",
           logger_sim_time(), worker_id, job->job_id, job->seller_id,
           job->estimated_runtime, job->priority);
}

void log_worker_finish(int worker_id, const job_t *job) {
    printf("[time=%d] Worker %d finishes Job %d seller=%s runtime=%d priority=%d\n",
           logger_sim_time(), worker_id, job->job_id, job->seller_id,
           job->estimated_runtime, job->priority);
}
