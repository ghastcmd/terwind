#include "thread.h"

thread_t thread_create(thread_ret_t (*foo)(void*), void* param)
{
    thread_t thread;

#ifdef _WINDOWS_
    thread = CreateThread(NULL, 0, foo, param, 0, NULL);
#else
    pthread_create(&thread, NULL, foo, param);
#endif

    // TODO: Error handling

    return thread;
}

void thread_join(thread_t thread)
{
#ifdef _WINDOWS_
    WaitForSingleObject(thread, INFINITE);
#else
    pthread_join(thread, NULL);
#endif

    //TODO: Error handling
}
