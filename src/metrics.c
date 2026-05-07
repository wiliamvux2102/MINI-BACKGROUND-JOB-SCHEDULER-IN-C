#include <stdio.h>
#include <stdlib.h>
#include "metrics.h"

void calculate_core_metrics(job_t jobs[], int num_jobs, double* avg_waiting, double* avg_turnaround, double* avg_runtime, int* total_busy_time, int* starvation_count) {
    int total_waiting = 0;
    int total_turnaround = 0;
    *total_busy_time = 0;

    for (int i = 0; i < num_jobs; i++) {
        int waiting_time = jobs[i].start_time - jobs[i].arrival_time;
        int turnaround_time = jobs[i].finish_time - jobs[i].arrival_time;

        total_waiting += waiting_time;
        total_turnaround += turnaround_time;
        *total_busy_time += jobs[i].estimated_runtime;
    }

    *avg_waiting = (double)total_waiting / num_jobs;
    *avg_turnaround = (double)total_turnaround / num_jobs;
    *avg_runtime = (double)(*total_busy_time) / num_jobs;

    *starvation_count = 0;
    for (int i = 0; i < num_jobs; i++) {
        int waiting_time = jobs[i].start_time - jobs[i].arrival_time;
        if (waiting_time > 2 * jobs[i].estimated_runtime) {
            (*starvation_count)++;
        }
    }
}

void print_summary_metrics(job_t jobs[], int num_jobs, int num_workers, int total_simulation_time, const char* policy_name) {
    double avg_waiting = 0.0, avg_turnaround = 0.0, avg_runtime = 0.0;
    int total_busy_time = 0, starvation_count = 0;

    calculate_core_metrics(jobs, num_jobs, &avg_waiting, &avg_turnaround, &avg_runtime, &total_busy_time, &starvation_count);

    double throughput = (double)num_jobs / total_simulation_time;
    double utilization = ((double)total_busy_time / (num_workers * total_simulation_time)) * 100.0;

    printf("\nPolicy: %s\n", policy_name);
    printf("Workers: %d\n", num_workers);
    printf("Total jobs: %d\n", num_jobs);
    printf("Average waiting time: %.2f\n", avg_waiting);
    printf("Average turnaround time: %.2f\n", avg_turnaround);
    printf("Throughput: %.3f jobs/unit time\n", throughput);
    printf("Worker utilization: %.2f%%\n", utilization);
    printf("Starvation-risk jobs: %d\n\n", starvation_count);
}

void export_metrics_csv(const char* filename, job_t jobs[], int num_jobs, int num_workers, int total_simulation_time, const char* policy_name) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("[ERROR] Cannot open file %s for exporting metrics.\n", filename);
        return;
    }

    double avg_waiting = 0.0, avg_turnaround = 0.0, avg_runtime = 0.0;
    int total_busy_time = 0, starvation_count = 0;

    calculate_core_metrics(jobs, num_jobs, &avg_waiting, &avg_turnaround, &avg_runtime, &total_busy_time, &starvation_count);

    double throughput = (double)num_jobs / total_simulation_time;
    double utilization = ((double)total_busy_time / (num_workers * total_simulation_time)) * 100.0;

    fprintf(file, "%s,%d,%.2f,%.2f,%.3f,%.2f,%d\n", 
            policy_name, num_workers, avg_waiting, avg_turnaround, throughput, utilization, starvation_count);

    fclose(file);
}