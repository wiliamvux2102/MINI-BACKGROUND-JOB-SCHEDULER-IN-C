#include <stdio.h>
#include <string.h>
#include "metrics.h"
#include "job.h" 

int main() {
    job_t dummy_jobs[3];

    // Job 1
    dummy_jobs[0].job_id = 1;
    strcpy(dummy_jobs[0].seller_id, "Seller_A");
    dummy_jobs[0].arrival_time = 0;
    dummy_jobs[0].estimated_runtime = 5;
    dummy_jobs[0].start_time = 0;
    dummy_jobs[0].finish_time = 5;
    dummy_jobs[0].status = JOB_DONE;

    // Job 2
    dummy_jobs[1].job_id = 2;
    strcpy(dummy_jobs[1].seller_id, "Seller_B");
    dummy_jobs[1].arrival_time = 1;
    dummy_jobs[1].estimated_runtime = 3;
    dummy_jobs[1].start_time = 5;
    dummy_jobs[1].finish_time = 8;
    dummy_jobs[1].status = JOB_DONE;

    // Job 3
    dummy_jobs[2].job_id = 3;
    strcpy(dummy_jobs[2].seller_id, "Seller_A");
    dummy_jobs[2].arrival_time = 2;
    dummy_jobs[2].estimated_runtime = 2;
    dummy_jobs[2].start_time = 8;
    dummy_jobs[2].finish_time = 10;
    dummy_jobs[2].status = JOB_DONE;

    int num_jobs = 3;
    int num_workers = 2;
    int total_simulation_time = 10; 
    const char* policy = "PRIORITY";

    printf("--- KET QUA TEST METRICS ---\n");
    // Xoá bỏ ở cuối dòng này
    print_summary_metrics(dummy_jobs, num_jobs, num_workers, total_simulation_time, policy); 

    printf("Dang xuat ket qua ra file metrics_test.csv...\n");
    // Xoá bỏ ở cuối dòng này
    export_metrics_csv("metrics_test.csv", dummy_jobs, num_jobs, num_workers, total_simulation_time, policy); 

    return 0;
}