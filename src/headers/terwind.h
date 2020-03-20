#include "pch.h"
#include "structs.h"

TerDim_t terwind_get_dimensions();

TerminalCanvas_t terwind_create_buffer(TerminalDimensions_t dims);

TerminalCanvas_t terwind_get_canvas();

void terwind_set_buffer(TerminalCanvas_t* wnd);

void terwind_fill_canvas(const char key);

void terwind_draw_canvas();

#ifdef TER_DEBUG
#define logg_terminal(fmt, ...) printf(fmt, __VA_ARGS__)
#define terwind_draw_canvas()
#define TER_WND(x)
#else
#define TER_WND(x) x
#define logg_terminal(fmt, ...)
#endif

void terwind_put_pixel(uint32_t x, uint32_t y, char key);

uint64_t terwind_get_ticks();

void terwind_game_loop(const int fps_cap);