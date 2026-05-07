#include "../include/synchronization.h"

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t job_available = PTHREAD_COND_INITIALIZER;

volatile int system_running = 1;