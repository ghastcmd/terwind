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
    for (int i = 0; i < max; i++)
    {
        wnd_buffer->canvas_grid[i] = key;
    }
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

uint64_t terwind_get_ticks()
{
#ifdef TER_DEBUG
    clock_t tick = clock();
    uint64_t al = (uint64_t)1000000000*((float)tick / CLOCKS_PER_SEC);
    logg_terminal("tick: %li ret: %"PRIu64"\n", tick, al);
    return al;
#endif
    return (uint64_t)1000000000*((float)clock() / CLOCKS_PER_SEC);
}