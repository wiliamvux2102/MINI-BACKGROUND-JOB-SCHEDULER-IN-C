#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "job.h"
#include "parser.h"
#include "scheduler.h"
#include "worker.h"
#include "metrics.h"

extern void logger_init(double scale_usec);
extern int logger_sim_time(void);

static int compare_arrival(const void *a, const void *b) {
    const job_t *j1 = (const job_t *)a;
    const job_t *j2 = (const job_t *)b;
    return j1->arrival_time - j2->arrival_time;
}

static const char *policy_name(policy_t policy) {
    switch (policy) {
        case POLICY_FIFO: return "FIFO";
        case POLICY_SJF: return "SJF";
        case POLICY_PRIORITY: return "Priority";
        default: return "UNKNOWN";
    }
}

static int parse_policy(const char *text, policy_t *policy) {
    if (!text || !policy) return 0;
    if (strcasecmp(text, "fifo") == 0) {
        *policy = POLICY_FIFO; return 1;
    }
    if (strcasecmp(text, "sjf") == 0) {
        *policy = POLICY_SJF; return 1;
    }
    if (strcasecmp(text, "priority") == 0) {
        *policy = POLICY_PRIORITY; return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <jobs.csv> <fifo|sjf|priority> <num_workers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *path = argv[1];
    policy_t policy;
    if (!parse_policy(argv[2], &policy)) {
        fprintf(stderr, "Unknown policy '%s'. Supported: fifo, sjf, priority\n", argv[2]);
        return EXIT_FAILURE;
    }

    int num_workers = atoi(argv[3]);
    if (num_workers <= 0) {
        fprintf(stderr, "Invalid worker count: %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    job_t jobs[256];
    int job_count = parse_jobs_from_csv(path, jobs);
    if (job_count < 0) {
        return EXIT_FAILURE;
    }
    if (job_count == 0) {
        fprintf(stderr, "No jobs available in %s\n", path);
        return EXIT_FAILURE;
    }

    qsort(jobs, job_count, sizeof(job_t), compare_arrival);

    const double scale_usec = 100000.0;
    scheduler_init();
    logger_init(scale_usec);
    init_workers(num_workers);

    for (int i = 0; i < job_count; ++i) {
        int arrival = jobs[i].arrival_time;
        int now = logger_sim_time();
        if (arrival > now) {
            usleep((useconds_t)((arrival - now) * scale_usec));
        }

        if (!scheduler_add_job(policy, &jobs[i])) {
            fprintf(stderr, "Failed to schedule job %d\n", jobs[i].job_id);
        }
    }

    scheduler_close();
    destroy_workers(num_workers);
    scheduler_destroy();

    int max_finish = 0;
    for (int i = 0; i < job_count; ++i) {
        if (jobs[i].finish_time > max_finish) {
            max_finish = jobs[i].finish_time;
        }
    }

    print_summary_metrics(jobs, job_count, num_workers, max_finish, policy_name(policy));

    return EXIT_SUCCESS;
}