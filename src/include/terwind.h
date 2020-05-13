#pragma once
#include "structs.h"

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

uint32_t terwind_get_ticks();

void terwind_game_loop(const int fps_cap);
void terwind_sleep_difftime(struct timespec *tvar1, struct timespec *tvar2, int frame_lock);
void terwind_gettime(struct timespec *tp);
void terwind_get_deltatime(float *dt, struct timespec *tp1, struct timespec *tp2);