#include "terwind.h"
#include "logg.h"
#include "thread.h"

void terwind_update_func(GameVars_t* vars, float delta_time);
void terwind_draw_func(const GameVars_t* vars);

extern TerminalCanvas_t* wnd_buffer;

void terwind_game_loop(const int fps_cap)
{
    const uint64_t fps_tick = 1000000000 / fps_cap;
    struct timespec tim, tim2;

    GameVariables_t vars = { 0 };
    vars.i = 1;

    logg_terminal("ticks: %"PRIu64"\n", terwind_get_ticks());

    thread_t async_thread = thread_init_async_input();
    thread_buffer_async_input_handle(&async_thread);

    register uint64_t starting_tick, ending_tick = fps_tick;
    float delta_time;
    while(!vars.stop)
    {
        starting_tick = terwind_get_ticks();

        terwind_fill_canvas(' ');

        delta_time = (float)(fps_tick - ending_tick) / 100000000;
        terwind_update_func(&vars, delta_time);
        terwind_draw_func(&vars);

        terwind_draw_canvas();

        ending_tick = terwind_get_ticks() - starting_tick;
        if (fps_tick > ending_tick)
        {
            tim.tv_sec = 0;
            tim.tv_nsec = (fps_tick - ending_tick);
            logg_status("fps_tick: %10"PRIu64" starting: %10"PRIu64" ending: %10"PRIu64" delay: %8li\n", fps_tick, starting_tick, ending_tick, tim.tv_nsec);
            nanosleep(&tim, &tim2);
        }
    }

    raise(SIGINT);
}

void terwind_update_func(GameVars_t* vars, float delta_time)
{
    kbd_keys_t key = thread_get_async_input();
    if (key == kb_esc) vars->stop = true;

    if (key == kb_right_arrow)
    {
        vars->i = !(vars->i);
        vars->j = !(vars->j);
    }

    vars->x_pos += (float)(((float)vars->i) * delta_time);
    vars->y_pos += (float)(((float)vars->j) * delta_time);

    logg_info("x_pos: %f\ty_pos: %f\n", vars->x_pos, vars->y_pos);
    logg_info("delta_time: %f\ti: %i\tj: %i\n", delta_time, vars->i, vars->j);

    if (vars->x_pos >= wnd_buffer->dim.width)
    {
        vars->x_pos = 0;
    }

    if (vars->y_pos >= wnd_buffer->dim.height)
    {
        vars->y_pos = 0;
    }
}

void terwind_draw_func(const GameVars_t* vars)
{
    terwind_put_pixel(vars->x_pos, vars->y_pos, 94);
}