#ifndef _THREAD_ARGS_H_
#define _THREAD_ARGS_H_

#include <pthread.h>

typedef struct thread_args {
    int nr_office;
    pthread_t main_thread;
} thread_args_t;

#endif