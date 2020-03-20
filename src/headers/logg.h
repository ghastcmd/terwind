#include "pch.h"
#include "macros.h"

#ifndef LOGG

#define logg_info(fmt, ...)
#define logg_stats(fmt, ...)
#define logg_error(fmt, ...)
#define logg_warning(fmt, ...)

#endif

void logg_setup(const uint32_t size, ...);

void logg_status(const char* fmt, ...);

void logg_info(const char* fmt, ...);

void logg_close();