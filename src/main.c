#include "pch.h"
#include "macros.h"
#include "structs.h"

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
    char* grid = (char*)calloc(max, sizeof(char));

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

void terwind_draw_canvas()
{
    printf("%s\r\x1b[0d", wnd_buffer->canvas_grid);
}

void terwind_put_pixel(uint32_t x, uint32_t y, char key)
{
    assert(wnd_buffer->dim.width >= x);
    assert(wnd_buffer->dim.height >= y);
    
    register int pos = wnd_buffer->dim.width * y + x;
    wnd_buffer->canvas_grid[pos] = key;
}

void terwind_draw_func();
void terwind_update_func();

void terwind_game_loop(const int fps_cap)
{
    register clock_t starting_tick;
    register const int fps_tick = 1000 / fps_cap;
    struct timespec tim, tim2;

    struct GameVariables vars = { 0 };

    while(1)
    {
        starting_tick = clock();

        terwind_fill_canvas(219);

        terwind_update_func(&vars);
        terwind_draw_func(&vars);

        terwind_draw_canvas();

        register clock_t ending_tick = clock() - starting_tick;
        if (fps_tick > ending_tick)
        {
            tim.tv_sec = 0;
            tim.tv_nsec = (fps_tick - ending_tick) * 1000000;
            nanosleep(&tim, &tim2);
        }
    }
}

void terwind_draw_func(const GameVars_t* vars)
{
    terwind_put_pixel(vars->i, 4, 94);
}

void terwind_update_func(GameVars_t* vars)
{
    vars->i++;
    if (vars->i >= wnd_buffer->dim.width)
    {
        vars->i = 0;
    }
}

int main()
{
    terminal_setup();
    TerminalCanvas_t wnd = terwind_get_canvas();
    terwind_set_buffer(&wnd);

    terwind_game_loop(60);

    return 0;
}