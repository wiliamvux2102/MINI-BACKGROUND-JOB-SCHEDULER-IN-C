#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include "parser.c"

int main() {
    job_t my_jobs[MAX_JOBS];
    int count = parse_jobs_from_csv("workload_a.csv", my_jobs);
    
    printf("Parsed %d jobs successfully.\n", count);
    for(int i = 0; i < count; i++) {
        printf("ID: %d, Seller: %s, Arrival: %d, Runtime: %d, Priority: %d, Type: %s\n",
            my_jobs[i].job_id, my_jobs[i].seller_id, my_jobs[i].arrival_time, 
            my_jobs[i].estimated_runtime, my_jobs[i].priority, my_jobs[i].job_type);
    }
    return 0;
}