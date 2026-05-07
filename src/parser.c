#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job.h"

#define MAX_JOBS 200
#define MAX_LINE_LENGTH 256

// Number of jobs
int parse_jobs_from_csv(const char* filename, job_t* jobs) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("[-] Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int job_count = 0;

    // Skip header
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        if (job_count >= MAX_JOBS) {
            fprintf(stderr, "[-] Reached maximum job limit (%d).\n", MAX_JOBS);
            break;
        }

        // Remove newline (\r\n or \n)
        line[strcspn(line, "\r\n")] = 0;

        int parsed = sscanf(line, "%d,%31[^,],%d,%d,%d,%63[^\n]",
               &jobs[job_count].job_id,
               jobs[job_count].seller_id,
               &jobs[job_count].arrival_time,
               &jobs[job_count].estimated_runtime,
               &jobs[job_count].priority,
               jobs[job_count].job_type);

        if (parsed == 6) {
            jobs[job_count].status = JOB_WAITING;
            jobs[job_count].start_time = -1;
            jobs[job_count].finish_time = -1;
            job_count++;
        } else {
            fprintf(stderr, "[-] Warning: Malformed line skipped: %s\n", line);
        }
    }

    fclose(file);
    return job_count;
}


void print_jobs_array(job_t* jobs, int count) {
    printf("\n+----+------+--------+---------+----------+--------------------+\n");
    printf("| ID | Sell | Arrive | Runtime | Priority | Type               |\n");
    printf("+----+------+--------+---------+----------+--------------------+\n");
    for (int i = 0; i < count; i++) {
        printf("| %-2d | %-4s | %-6d | %-7d | %-8d | %-18s |\n",
               jobs[i].job_id,
               jobs[i].seller_id,
               jobs[i].arrival_time,
               jobs[i].estimated_runtime,
               jobs[i].priority,
               jobs[i].job_type);
    }
    printf("+----+------+--------+---------+----------+--------------------+\n");
}
