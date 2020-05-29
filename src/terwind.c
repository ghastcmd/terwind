#include "pch.h"
#include "terwind.h"
#include "logg.h"

TerDim_t terwind_get_dimensions()
{
    TerDim_t dims;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    
    dims.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    dims.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

#else
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    dims.width = window.ws_col;
    dims.height = window.ws_row;

#endif
    return dims;
}

TerminalCanvas_t terwind_create_buffer(TerminalDimensions_t dims)
{
    TerminalCanvas_t canvas;
    int max = dims.width * dims.height;
    char* grid = (char*)calloc(max + 1, sizeof(char));

    canvas.dim = dims;
    canvas.canvas_grid = grid;
    return canvas;
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

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0x1
#endif

void terwind_gettime(stime_t *tp)
{
#ifdef CLOCK_MONOTONIC
    clock_gettime(CLOCK_MONOTONIC, (struct timespec*)tp);
#else
    timespec_get((struct timespec*)tp, TIME_UTC);
#endif
    tp->seconds = 0;
}

static inline void terwind_nanosleep(stime_t *tp1, stime_t *tp2)
{
    nanosleep((struct timespec*)tp1, (struct timespec*)tp2);
}

void terwind_sleep_difftime(stime_t *tvar1, stime_t *tvar2, int fpscap)
{
    tvar2->nanosec -= tvar1->nanosec;
    if (tvar2->nanosec > 0 && fpscap > tvar2->nanosec)
    {
        tvar2->nanosec = fpscap - tvar2->nanosec;
        terwind_nanosleep(tvar2, tvar1);
    }
}

static float last = 0;
float terwind_get_deltatime()
{
    const float old = last;
    stime_t tp;
    terwind_gettime(&tp);
    last = ((float)tp.nanosec / 1000000000.0f);
    float ret = last - old;
    return ret < 0 ? ret + 1 : ret;
}