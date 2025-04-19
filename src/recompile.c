#include "pch.h"

#include "internal/thread.h"
#include "internal/dll.h"
#include "logg/logg.h"

int version = 10;
thread_mutex_t version_mutex = NULL;

thread_t recompile_thread_alloc[3];

int lib_version = 0;
void * current_lib_handle = NULL;

void recompile_code_lib_init_mutex()
{
    version_mutex = thread_create_mutex();
}

void recompile_code_lib_free_mutex()
{
    thread_clean_mutex(version_mutex);
}

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
    logg_status("before starting new thread\n");
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

void * recompile_get_handle()
{
    int tmp_lib_version = recompile_code_lib_get_version();
    if (tmp_lib_version != lib_version)
    {
        dll_close(current_lib_handle);

        recompile_terminate_thread(lib_version);
        lib_version = tmp_lib_version;

        char lib_name[128] = {0};
        snprintf(lib_name, 128, "code_lib%i", lib_version);
        void * handle = dll_load(lib_name);
        
        current_lib_handle = handle;
    
        char * error_fmt = dll_check_errors_load(handle);
        if (error_fmt) 
        {
            logg_status(error_fmt);
        }
    }

    
    return current_lib_handle;
}