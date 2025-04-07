#include "pch.h"
#include "terwind.h"
#include "logg/logg.h"
#include "thread.h"
#include "render.h"

void terwind_update_func(GameVars_t* vars, kbd_keys_t key, float delta_time);
void terwind_draw_func(const GameVars_t* vars);

extern TerminalCanvas_t* wnd_buffer;

void terwind_game_loop(const int fps_cap)
{
    GameVariables_t vars = { 0 };
    vars.i = 1;
    vars.another_x_pos = 15 + 4;
    vars.another_y_pos = 15 + 4;

    thread_t async_thread = thread_init_async_input();

    const uint32_t fps_tick = 1000 / fps_cap;
    stime_t time_start = { 0 }, time_ending = { 0 };
    float dt = 0;
    int idx = 0;
    while(!vars.stop)
    {
        if (fps_cap != 0) terwind_gettime(&time_start);

        logg_info("\nframe %i\n", idx++);

        kbd_keys_t key = thread_get_async_input();

        if (key == kb_ctrl_c || key == kb_esc) break;

        terwind_fill_canvas(' ');

        dt = terwind_get_deltatime();
        terwind_update_func(&vars, key, dt);
        terwind_draw_func(&vars);

        terwind_draw_canvas();

        if (fps_cap != 0)
        {
            terwind_gettime(&time_ending);
            terwind_sleep_difftime(&time_start, &time_ending, fps_tick);
        }
    }

    thread_terminate(async_thread);
}

void terwind_update_func(GameVars_t* vars, kbd_keys_t key, float dt)
{
    if (key == kb_right_arrow)
    {
        vars->another_x_pos += 1;
    }
    else if (key == kb_left_arrow)
    {
        vars->another_x_pos -= 1;
    }
    else if (key == kb_up_arrow)
    {
        vars->another_y_pos -= 1;
    }
    else if (key == kb_bottom_arrow)
    {
        vars->another_y_pos += 1;
    }

    vars->x_pos += (float)(vars->i * 1) * dt;
    vars->y_pos += (float)(vars->j * 0.5f) * dt;

    logg_info("x_pos: %f\ty_pos: %f\n", vars->x_pos, vars->y_pos);
    logg_info("delta_time: %f\ti: %i\tj: %i\n", dt, vars->i, vars->j);

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
    terwind_put_pixel(vars->x_pos * 1.2, vars->y_pos+3, 94);

    render_letters(vars->x_pos, 2, "this is the start", sizeof("this is the start"));

    render_line(2, 10, 10, 1, true);
    float spacing = 12.f;
    render_line(spacing + 2, 2, spacing + 10, 1, true);
    render_line(spacing + 0, 0, spacing + 10, 1, true);
    render_line(spacing + 2, 1, spacing + 10, 10, true);
    spacing += 12;
    render_line(spacing + 2, 2, spacing + 2, 10, true);

    render_line(15, 15, vars->another_x_pos, vars->another_y_pos, true);

    terwind_put_pixel(vars->x_pos + 10, 13, '*');
}