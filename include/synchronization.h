#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <pthread.h>

extern pthread_mutex_t queue_mutex;
extern pthread_cond_t job_available;

extern volatile int system_running;

#endif