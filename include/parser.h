#ifndef PARSER_H
#define PARSER_H

#include "job.h"

int parse_jobs_from_csv(const char* filename, job_t* jobs);
void print_jobs_array(job_t* jobs, int count);

#endif
