#pragma once
#include "structs.h"

typedef struct time_struct
{
    union
    {
        uint64_t tv_sec;
        uint64_t seconds;
    };
    union
    {
        uint64_t tv_nsec;
        uint64_t nanosec;
    };
} stime_t;

TerDim_t terwind_get_dimensions();

TerminalCanvas_t terwind_create_buffer(TerminalDimensions_t dims);

TerminalCanvas_t terwind_get_canvas();

void terwind_free();

void terwind_set_buffer(TerminalCanvas_t* wnd);

void terwind_fill_canvas(const char key);

#ifndef TER_DEBUG
void terwind_draw_canvas();
#else
#define terwind_draw_canvas()
#endif

void terwind_put_pixel(uint32_t x, uint32_t y, char key);

void terwind_game_loop(const int fps_cap);
void terwind_sleep_difftime(stime_t *tvar1, stime_t *tvar2, uint64_t fpscap);
void terwind_gettime(stime_t *tp);

float terwind_get_deltatime();