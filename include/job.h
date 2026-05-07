#ifndef JOB_H
#define JOB_H

typedef enum{
    JOB_WAITING,
    JOB_RUNNING,
    JOB_DONE
} job_status_t;


typedef struct{
    int job_id;
    char seller_id[32];
    int arrival_time;
    int estimated_runtime;
    int priority;
    char job_type[64];

    int start_time;
    int finish_time;

    job_status_t status;

} job_t;

#endif