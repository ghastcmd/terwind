#include "pch.h"
#include "terwind.h"
#include "logg.h"

TerDim_t terwind_get_dimensions()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    
    return (TerDim_t)
    {
        .width = csbi.srWindow.Right - csbi.srWindow.Left + 1,
        .height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1,
    };

#else
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    return (TerDim_t)
    {
        .width = window.ws_col,
        .height = window.ws_row,
    };

#endif
}

TerminalCanvas_t terwind_create_buffer(TerminalDimensions_t dims)
{
    int max = dims.width * dims.height;
    char* grid = (char*)calloc(max + 1, sizeof(char));

    return (TerminalCanvas_t)
    {
        .dim = dims,
        .canvas_grid = grid,
    };
}

TerminalCanvas_t terwind_get_canvas()
{
    register TerDim_t dims = terwind_get_dimensions();
    return terwind_create_buffer(dims);
}

TerminalCanvas_t* wnd_buffer = NULL;

void terwind_free()
{
    free(wnd_buffer->canvas_grid);
}

void terwind_set_buffer(TerminalCanvas_t* wnd)
{
    wnd_buffer = wnd;
}

void terwind_fill_canvas(const char key)
{
    int max = wnd_buffer->dim.height * wnd_buffer->dim.width;
    memset(wnd_buffer->canvas_grid, key, max);
}

#ifndef TER_DEBUG
void terwind_draw_canvas()
{
    printf("%s\r\x1b[0d", wnd_buffer->canvas_grid);
    fflush(stdout);
}
#endif

void terwind_put_pixel(uint32_t x, uint32_t y, char key)
{
    assert(wnd_buffer->dim.width >= x);
    assert(wnd_buffer->dim.height >= y);
    
    register int pos = wnd_buffer->dim.width * y + x;
    wnd_buffer->canvas_grid[pos] = key;
}

uint32_t terwind_get_ticks()
{
#ifdef TER_DEBUG
    clock_t tick = clock();
    uint64_t al = (uint64_t)1000000*((float)tick / CLOCKS_PER_SEC);
    logg_terminal("tick: %li ret: %"PRIu64"\n", tick, al);
    return al;
#endif
    struct timespec tick = { 0 };
#ifdef CLOCK_MONOTONIC
    clock_gettime(CLOCK_MONOTONIC, &tick);
#else
    timespec_get(&tick, TIME_UTC);
#endif
    return (uint32_t)tick.tv_nsec / 1000;
    // return (uint32_t)1000000*((float)clock() / CLOCKS_PER_SEC);
}

void terwind_gettime(struct timespec *tp)
{
#ifdef CLOCK_MONOTONIC
    clock_gettime(CLOCK_MONOTONIC, tp);
#else
    timespec_get(tp, TIME_UTC);
#endif
    tp->tv_sec = 0;
}

void terwind_sleep_difftime(struct timespec *tvar1, struct timespec *tvar2, int frame_lock)
{
    tvar2->tv_nsec -= tvar1->tv_nsec;
    if (tvar2->tv_nsec > 0 && frame_lock > tvar2->tv_nsec)
    {
        tvar2->tv_nsec = frame_lock - tvar2->tv_nsec;
        nanosleep(tvar2, tvar1);
    }
}

void terwind_get_deltatime(float *dt, struct timespec *tp1, struct timespec *tp2)
{
    *dt = (float)(tp2->tv_nsec) / 1000000000.0f;
    (void)*tp1;
    if (*dt < 0)
    {
        *dt += 1;
    }
}