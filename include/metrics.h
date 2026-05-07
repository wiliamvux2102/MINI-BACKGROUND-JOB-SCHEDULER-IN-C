#ifndef METRICS_H
#define METRICS_H

#include "job.h"

void print_summary_metrics(job_t jobs[], int num_jobs, int num_workers, int total_simulation_time, const char* policy_name);

void export_metrics_csv(const char* filename, job_t jobs[], int num_jobs, int num_workers, int total_simulation_time, const char* policy_name);

#endif