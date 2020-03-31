#include "pch.h"
#include "macros.h"

#ifndef LOGG
#define logg_info(fmt, ...)
#define logg_status(fmt, ...)
#define logg_error(fmt, ...)
#define logg_warning(fmt, ...)

#define logg_setup(size, ...)
#define logg_close()

#else

void logg_status(const char* fmt, ...);

void logg_info(const char* fmt, ...);

void logg_setup(const uint32_t size, ...);

void logg_close();

#endif

#ifdef TER_DEBUG
#define logg_terminal(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define logg_terminal(fmt, ...)
#endif
