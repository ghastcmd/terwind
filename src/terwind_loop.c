#include "terwind.h"
#include "logg.h"

void terwind_draw_func(const GameVars_t* vars);
void terwind_update_func(GameVars_t* vars);

extern TerminalCanvas_t* wnd_buffer;

void terwind_game_loop(const int fps_cap)
{
    const uint64_t fps_tick = 1000000000 / fps_cap;
    struct timespec tim, tim2;

    GameVariables_t vars = { 0 };

    logg_terminal("ticks: %"PRIu64"\n", terwind_get_ticks());

    register uint64_t starting_tick, ending_tick;
    while(!vars.stop)
    {
        starting_tick = terwind_get_ticks();

        terwind_fill_canvas(' ');

        terwind_update_func(&vars);
        terwind_draw_func(&vars);

        terwind_draw_canvas();

        ending_tick = terwind_get_ticks() - starting_tick;
        if (fps_tick > ending_tick)
        {
            tim.tv_sec = 0;
            tim.tv_nsec = (fps_tick - ending_tick);
            logg_status("fps_tick: %"PRIu64" starting: %"PRIu64" ending: %"PRIu64" delay: %li\n", fps_tick, starting_tick, ending_tick, tim.tv_nsec);
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