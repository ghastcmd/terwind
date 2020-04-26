#pragma once
#include "pch.h"
#include "thread_keys.h"

#ifndef _WINDOWS_
typedef pthread_t thread_t;
typedef void*(*thread_sts_t)(void*);
typedef void* thread_ret_t;
#else
typedef HANDLE thread_t;
typedef PTHREAD_START_ROUTINE thread_sts_t;
typedef unsigned long int thread_ret_t;
#endif

thread_t thread_create(thread_ret_t (*foo)(void*), void* param);

void thread_join(thread_t thread);

void thread_terminate(thread_t thread);

thread_t thread_init_async_input();

kbd_keys_t thread_get_async_input();

void thread_test_keys();