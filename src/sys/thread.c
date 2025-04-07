#include "pch.h"
#include "sys/thread.h"
#include "terminal.h"

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

    // TODO: Error handling
}

void thread_terminate(thread_t thread)
{
#ifdef _WINDOWS_
    TerminateThread(thread, 0);
#else
    pthread_kill(thread, SIGKILL);
    pthread_join(thread, NULL);
    pthread_cancel(thread);
#endif

    // TODO: Error handling
}

thread_mutex_t thread_create_mutex()
{
#ifdef _WIN32
    return CreateMutexA(NULL, 0, NULL);
#else
    return PTHREAD_MUTEX_INITIALIZER;
#endif
}

void thread_mutex_lock(thread_mutex_t mutex)
{
#ifdef _WIN32
    WaitForSingleObject(mutex, INFINITE);
#else
    pthread_mutex_lock(mutex);
#endif
}

void thread_mutex_unlock(thread_mutex_t mutex)
{
#ifdef _WIN32
    ReleaseMutex(mutex);
#else
    pthread_mutex_unlock(mutex);
#endif
}

static inline void emmit(unsigned char byte, kbd_keys_t* smt)
{
    *smt = (*smt << 8) | byte;
}

static thread_ret_t thread_async_input(void* token)
{
    unsigned char kb_key;
    while(1)
    {
        kb_key = getch();
        *(int*)token = 0;
        emmit(kb_key == 0 ? 0xf0 : kb_key, token);

        while (kbhit())
        {
            emmit(getch(), token);
        }
    }

    return (thread_ret_t)0;
}

static kbd_keys_t async_token;

kbd_keys_t thread_get_async_input()
{
    kbd_keys_t ret = async_token;
    async_token = 0;
    return ret;
}

thread_t* thread_async_input_handle;

thread_t thread_init_async_input()
{
    return thread_create(thread_async_input, &async_token);
}

void thread_test_keys()
{
    unsigned char kb_key;
    kbd_keys_t token = 0;
    while(token != kb_esc)
    {
        kb_key = getch();
        token = 0;
        emmit(kb_key == 0 ? 0xf0 : kb_key, &token);

        while (kbhit())
        {
            emmit(getch(), &token);
        }

        printf("%i\n", token);
    }
}