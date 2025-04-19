#include "pch.h"

#include "sys/thread.h"
#include "logg/logg.h"

int version = 0;
thread_mutex_t version_mutex = NULL;

void recompile_code_lib_init_mutex()
{
    version_mutex = thread_create_mutex();
}

void recompile_code_lib_free_mutex()
{
    thread_clean_mutex(version_mutex);
}

thread_t recompile_thread_alloc[2];

thread_ret_t recompile_code_lib_do(void * ptr)
{
    (void)ptr;
    
    thread_mutex_lock(version_mutex);
    int tmp_version;
    if (version == 0)
    {
        tmp_version = 1;
    }
    else
    {
        tmp_version = 0;
    }
    thread_mutex_unlock(version_mutex);

    char cmd[128] = {0};
    snprintf(cmd, 128, "make code_lib_hot_reload code_lib_version=%i > NUL 2>&1", tmp_version);
    int cmd_run_ret = system(cmd);
    (void)cmd_run_ret;

    logg_status("%i\n", cmd_run_ret);

    thread_mutex_lock(version_mutex);
    version = tmp_version;
    thread_mutex_unlock(version_mutex);

    return (thread_ret_t)0;
}

void recompile_code_lib()
{
    recompile_thread_alloc[version] = thread_create(recompile_code_lib_do, NULL);
}

void recompile_terminate_thread(int no)
{
    thread_terminate(recompile_thread_alloc[no]);
}

int recompile_code_lib_get_version()
{
    thread_mutex_lock(version_mutex);
    int ret_version = version;
    thread_mutex_unlock(version_mutex);

    return ret_version;
}