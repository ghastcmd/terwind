#include "pch.h"
#include "terwind.h"
#include "logg/logg.h"
#include "internal/thread.h"
#include "render.h"
#include "recompile.h"
#include "window.h"

void terwind_update_func(GameVars_t* vars, kbd_keys_t key, float delta_time);
void terwind_draw_func(const GameVars_t* vars);

extern TerminalCanvas_t* wnd_buffer;

void terwind_game_loop(const int fps_cap)
{
    GameVariables_t vars = { 0 };
    vars.i = 1;
    vars.another_x_pos = 15 + 4;
    vars.another_y_pos = 15 + 4;
    vars.can_rotate = true;

    thread_t async_thread = thread_init_async_input();
    recompile_code_lib_init_mutex();

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
    recompile_code_lib_free_mutex();
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

    if (key == kb_letter_m)
    {
        recompile_code_lib();
    }
    
    if (key == kb_letter_r)
    {
        vars->can_rotate = !vars->can_rotate;
    }

    if (vars->can_rotate)
    {
        float x = vars->another_x_pos - 15, y = vars->another_y_pos - 15;
        float theta = dt * 1.3f;
        vars->another_x_pos = x * cos(theta) - y * sin(theta) + 15;
        vars->another_y_pos = x * sin(theta) + y * cos(theta) + 15;
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

    assign_logger();
    render_line(15, 15, vars->another_x_pos, vars->another_y_pos, true);
    
    const char * coef_string = get_logger();
    const size_t coef_string_size = strlen(coef_string);
    const size_t w = wnd_buffer->dim.width, h = wnd_buffer->dim.height;
    render_letters(w - coef_string_size, h - 1, coef_string, coef_string_size);

    logg_info("x %f, y %f, %s\n", vars->another_x_pos, vars->another_y_pos, coef_string);

    remove_logger();

    terwind_put_pixel(vars->x_pos + 10, 13, '*');

    RenderWindow_t info = {
        .width = 20,
        .line_count = 3,
        .top_corner_x = 2,
        .top_corner_y = 10,
    };

    window_draw_box(info);
}