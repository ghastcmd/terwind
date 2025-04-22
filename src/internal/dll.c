#include "pch.h"

char dll_load_error_fmt[512] = {0};

void * dll_load(const char * name)
{
    char fmt_name[128] = {0};
#ifdef _WINDOWS_
    snprintf(fmt_name, 128, "lib%s.dll", name);
#else
    snprintf(fmt_name, 128, "%s.so", name);
#endif

#ifdef _WINDOWS_
    void * ret = LoadLibraryA(fmt_name);
#else
    void * ret = dlopen(fmt_name, RTLD_LAZY);
#endif

    return ret;
}

char * dll_check_errors_load(void * dll_handle)
{
    if (!dll_handle)
    {
#ifdef _WINDOWS_
        snprintf(dll_load_error_fmt, 512, "Dll Error: %li", GetLastError());
#else
        snprintf(dll_load_error_fmt, 512, "Dll Error: %s", dlerror());
#endif
        return dll_load_error_fmt;
    }
    return NULL;
}

void * dll_get_symbol(void * handle, const char * name)
{
#ifdef _WINDOWS_
    void * proc_handle = GetProcAddress(handle, name);
#else
    void * proc_handle = dlsym(handle, name);
#endif
    
    if (!proc_handle)
    {
#ifdef _WINDOWS_
        snprintf(dll_load_error_fmt, 512, "Couldn't load symbol <%s>.\nError: %li", name, GetLastError());
#else
        snprintf(dll_load_error_fmt, 512, "Couldn't load symbol <%s>.\nError: %s", name, dlerror());
#endif
    }

    return proc_handle;
}

// char * dll_check_errors_get_symbol(void * proc_handle)
// {
    
//     return NULL;
// }

void dll_close(void * handle)
{
    if (!handle) return;

#ifdef _WINDOWS_
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}