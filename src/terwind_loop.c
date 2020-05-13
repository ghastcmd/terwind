#include "pch.h"
#include "terwind.h"
#include "logg.h"
#include "thread.h"

void terwind_update_func(GameVars_t* vars, kbd_keys_t key, float delta_time);
void terwind_draw_func(const GameVars_t* vars);

extern TerminalCanvas_t* wnd_buffer;

void terwind_game_loop(const int fps_cap)
{
    const uint32_t fps_tick = 1000000 / fps_cap;

    GameVariables_t vars = { 0 };
    vars.i = 1;

    thread_t async_thread = thread_init_async_input();

    struct timespec time_start = { 0 }, time_ending = { 0 }; 
    float delta_time;
    while(!vars.stop)
    {
        terwind_get_deltatime(&delta_time, &time_start, &time_ending);
        terwind_gettime(&time_start);

        kbd_keys_t key = thread_get_async_input();

        if (key == kb_ctrl_c) break;

        terwind_fill_canvas(' ');

        terwind_update_func(&vars, key, delta_time);
        terwind_draw_func(&vars);

        terwind_draw_canvas();

        terwind_gettime(&time_ending);
        terwind_sleep_difftime(&time_start, &time_ending, fps_tick);
    }

    thread_terminate(async_thread);
}

void terwind_update_func(GameVars_t* vars, kbd_keys_t key, float delta_time)
{
    if (key == kb_right_arrow)
    {
        vars->i = !(vars->i);
        vars->j = !(vars->j);
    }

    vars->x_pos += (float)(vars->i * 4) * delta_time;
    vars->y_pos += (float)(vars->j * 2) * delta_time;

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