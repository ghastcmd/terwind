#include "pch.h"
#include "terwind.h"
#include "logg/logg.h"

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

void terwind_gettime(stime_t *tp)
{
#ifdef _WIN32
    tp->nanosec = GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tp->nanosec = tv.tv_usec / 1000;
#endif
    logg_info("miliseconds %li\n", tp->nanosec);
    tp->seconds = 0;
}

static inline void terwind_nanosleep(stime_t *tp1, stime_t *tp2)
{
    (void)tp1, (void)tp2;
    // ! TODO : Find a replacement for nanosleep on windows
    // nanosleep((struct timespec*)tp1, (struct timespec*)tp2);
}

void terwind_sleep_difftime(stime_t *tvar1, stime_t *tvar2, uint64_t fpscap)
{
    logg_info("tvar2: %lu, tvar1: %lu\n", tvar2->nanosec, tvar1->nanosec);
    tvar2->nanosec -= tvar1->nanosec;
    logg_info("fps_cap: %lu, dftime dt: %lu\n", fpscap, tvar2->nanosec);
    if (fpscap > tvar2->nanosec)
    {
        tvar2->nanosec = (fpscap - tvar2->nanosec) * 1000000;
        logg_info("sec: %li, sleep: %li\n", tvar2->seconds, tvar2->nanosec);
        logg_info("sec: %li, sleep: %li\n", tvar1->seconds, tvar1->nanosec);
        terwind_nanosleep(tvar2, tvar1);
    }
}

static float last = 0;
float terwind_get_deltatime()
{
    const float old = last;
    stime_t tp;
    terwind_gettime(&tp);
    last = ((float)tp.nanosec / 1000.0f);
    float ret = last - old;
    logg_info("last: %f, ret: %f, old: %f\n", last, ret, old);
    return ret < 0 ? ret + 1 : ret;
}