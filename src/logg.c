#include "logg.h"

#ifdef LOGG

static FILE** file_list;
static size_t file_list_size;

enum fileno
{
    loggno_status,
    loggno_default,
};


void logg_setup(const uint32_t size, ...)
{
    va_list args;
    va_start(args, size);

    file_list = malloc(sizeof(FILE*) * size);

    for(uint32_t i = 0; i < size; i++)
    {
        register const char* arg = va_arg(args, char*);
        file_list[i] = fopen(arg, "w+");
    }

    file_list_size = size;

    va_end(args);
}

void __logg_fileno_list__(const enum fileno fileno, const char* fmt, va_list args)
{
    vfprintf(file_list[fileno], fmt, args);
}

void logg_status(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __logg_fileno_list__(loggno_status, fmt, args);
    va_end(args);
}

void logg_info(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __logg_fileno_list__(loggno_default, fmt, args);
    va_end(args);
}

// void logg_terminal(const char* fmt, ...)
// {
//     va_list args;
//     va_start(args, fmt);
//     vprintf(fmt, args);
//     va_end(args);
// }

// ! UNUSED ! //
void logg_fileno(const enum fileno fileno, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(file_list[fileno], fmt, args);
    va_end(args);
}

void logg_close()
{
    register int size = file_list_size;
    for (int i = 0; i < size; i++)
    {
        fclose(file_list[i]);
    }
}

/*
 * logg_status(const char*, ...);
 * logg_info;
 * logg_warning;
 * logg_error;
 * 
 * logg_fileno(enum fileno, char* restrict, ...); 
*/
//

#endif