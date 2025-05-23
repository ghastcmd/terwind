#pragma once
#include "thread_keys.h"

#ifdef __linux__
typedef pthread_t thread_t;
typedef void*(*thread_sts_t)(void*);
typedef void* thread_ret_t;
typedef pthread_mutex_t* thread_mutex_t;
#elif _WIN32
typedef HANDLE thread_t;
typedef PTHREAD_START_ROUTINE thread_sts_t;
typedef unsigned long int thread_ret_t;
typedef HANDLE thread_mutex_t;
#endif

thread_t thread_create(thread_ret_t (*foo)(void*), void* param);

void thread_join(thread_t thread);

void thread_terminate(thread_t thread);

thread_mutex_t thread_create_mutex();

void thread_clean_mutex(thread_mutex_t handle);

void thread_mutex_lock(thread_mutex_t mutex);

void thread_mutex_unlock(thread_mutex_t mutex);

thread_t thread_init_async_input();

kbd_keys_t thread_get_async_input();

void thread_test_keys();