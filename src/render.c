#include "pch.h"

#include "render.h"

#include "terwind.h"
#include "logg/logg.h"
#include "internal/dll.h"
#include "recompile.h"

void render_letters(float start_pos_x, float start_pos_y, const char * to_write, size_t string_size)
{
    for (size_t i = 0; i < string_size-1; ++i)
    {
        terwind_put_pixel(start_pos_x + i, start_pos_y, to_write[i]);
    }
}

DLLEXPORT
char get_inclination(float val)
{
    if (val > 2.5f)
    {
        return '\\';
    }
    else if (val >= 0.5f && val <= 2.5f)
    {
        return '-';
    }
    else if (val < 2.5f && val > 0.2f)
    {
        return '/';
    }
    else if (val <= 0.2f && val >= -0.18f)
    {
        return '|';
    }
    else if (val < -0.18f && val >= -0.4f)
    {
        return '\\';
    }
    else if (val < -1.f && val >= -1.5f)
    {
        return '-';
    } 
    else
    {
        return '\\';
    }
}

char (*inc_get_inclination)(float) = NULL;

void get_func_get_inclination()
{
    void * handle = recompile_get_handle();
    handle = dll_get_symbol(handle, "get_inclination");
    inc_get_inclination = (char (*)(float))(handle);
}

static char numbers_display[64] = {0};
bool display_numbers = false;

void assign_logger()
{
    display_numbers = true;
}

const char * get_logger()
{
    return numbers_display;
}

void remove_logger()
{
    display_numbers = false;
}

void render_line(float start_pos_x, float start_pos_y, float end_pos_x, float end_pos_y, bool do_intersections)
{
    get_func_get_inclination();

    TerDim_t dims = terwind_get_dimensions();
    float pixel_width = (1.f / dims.width);
    float pixel_height = (1.f / dims.height);
    // float pixel_coef = (pixel_width + 0.5f) / (pixel_height + 0.5f);

    const float dx_abit = pixel_width * 0.01f;
    const float dy_abit = pixel_height * 0.01f;

    if (start_pos_x > end_pos_x)
    {
        float temp = start_pos_x;
        start_pos_x = end_pos_x;
        end_pos_x = temp;
        temp = start_pos_y;
        start_pos_y = end_pos_y;
        end_pos_y = temp;
    }

    float true_coords_start_x = (start_pos_x / dims.width) - 0.5f;
    float true_coords_start_y = (start_pos_y / dims.height) - 0.5f;
    
    float true_coords_end_x = (end_pos_x / dims.width) - 0.5f;
    float true_coords_end_y = (end_pos_y / dims.height) - 0.5f;

    float x_delta = true_coords_end_x - true_coords_start_x;
    float y_delta = true_coords_end_y - true_coords_start_y;
    float coef = -(x_delta / y_delta);

    if (coef > 0.f)
    {
        true_coords_start_x = (start_pos_x / dims.width) - 0.5f - (pixel_width * 0.5f);
        true_coords_start_y = (start_pos_y / dims.height) - 0.5f + (pixel_height * 0.5f);
        
        true_coords_end_x = (end_pos_x / dims.width) - 0.5f + (pixel_width * 0.5f);
        true_coords_end_y = (end_pos_y / dims.height) - 0.5f - (pixel_height * 0.5f);
        
        x_delta = true_coords_end_x - true_coords_start_x;
        y_delta = true_coords_end_y - true_coords_start_y;
        coef = -(x_delta / y_delta);
        
        int x_delta_int = (int)(end_pos_x - start_pos_x);
        int y_delta_int = (int)(end_pos_y - start_pos_y);
        
        for (int i = 0; i <= x_delta_int; ++i)
        {
            logg_info("i %li\n", i);

            for (int j = 0; j >= y_delta_int; --j)
            {
                float current_pos_x_lower_right = ((start_pos_x + i) * pixel_width) - 0.5f + (pixel_width * 0.5f);
                float current_pos_y_lower_right = ((start_pos_y + j) * pixel_height) - 0.5f + (pixel_height * 0.5f) - dy_abit;
                
                float current_x_delta_lower_right = current_pos_x_lower_right - true_coords_start_x;
                float current_y_delta_lower_right = current_pos_y_lower_right - true_coords_start_y;
                float current_lower_right_coef = -(current_x_delta_lower_right / current_y_delta_lower_right);
                
                float current_pos_x_upper_left = ((start_pos_x + i) * pixel_width) - 0.5f - (pixel_width * 0.5f) + dx_abit;
                float current_pos_y_upper_left = ((start_pos_y + j) * pixel_height) - 0.5f - (pixel_height * 0.5f);
                
                float current_x_delta_upper_left = current_pos_x_upper_left - true_coords_start_x;
                float current_y_delta_upper_left = current_pos_y_upper_left - true_coords_start_y;
                float current_upper_left_coef = -(current_x_delta_upper_left / current_y_delta_upper_left);
                
                logg_info("dlr %f, c %f, dul %f\n", current_lower_right_coef, coef, current_upper_left_coef);

                snprintf(numbers_display, 64, "c %f", coef);

                if (coef <= current_lower_right_coef && coef >= current_upper_left_coef)
                {
                    char resolved_inclination = inc_get_inclination(coef);

                    terwind_put_pixel(start_pos_x + i, start_pos_y + j, resolved_inclination);
                }
            }
        }
    }
    else
    {
        true_coords_start_x = (start_pos_x * pixel_width) - 0.5f - (pixel_width * 0.5f);
        true_coords_start_y = (start_pos_y * pixel_height) - 0.5f - (pixel_height * 0.5f);
        
        true_coords_end_x = (end_pos_x * pixel_width) - 0.5f + (pixel_width * 0.5f);;
        true_coords_end_y = (end_pos_y * pixel_height) - 0.5f + (pixel_height * 0.5f);;

        x_delta = true_coords_end_x - true_coords_start_x;
        y_delta = true_coords_end_y - true_coords_start_y;
        coef = -(x_delta / y_delta);

        int x_delta_int = (int)(end_pos_x - start_pos_x);
        int y_delta_int = (int)(end_pos_y - start_pos_y);

        for (int i = 0; i <= x_delta_int; ++i)
        {
            for (int j = 0; j <= y_delta_int; ++j)
            {
                float current_pos_x_upper_right = ((start_pos_x + i) * pixel_width) - 0.5f + (pixel_width * 0.5f);
                float current_pos_y_upper_right = ((start_pos_y + j) * pixel_height) - 0.5f - (pixel_height * 0.5f) + dy_abit;

                float current_x_delta_upper_right = current_pos_x_upper_right - true_coords_start_x;
                float current_y_delta_upper_right = current_pos_y_upper_right - true_coords_start_y;
                float current_upper_right_coef = -(current_x_delta_upper_right / current_y_delta_upper_right);

                float current_pos_x_lower_left = ((start_pos_x + i) * pixel_width) - 0.5f - (pixel_width * 0.5f) + dx_abit;
                float current_pos_y_lower_left = ((start_pos_y + j) * pixel_height) - 0.5f + (pixel_height * 0.5f);
                
                float current_x_delta_lower_left = current_pos_x_lower_left - true_coords_start_x;
                float current_y_delta_lower_left = current_pos_y_lower_left - true_coords_start_y;
                float current_lower_left_coef = -(current_x_delta_lower_left / current_y_delta_lower_left);

                logg_info("c %f\n", coef);

                snprintf(numbers_display, 64, "c %f", coef);

                if (coef >= current_upper_right_coef && coef <= current_lower_left_coef)
                {
                    char resolved_inclination = inc_get_inclination(coef);

                    terwind_put_pixel(start_pos_x + i, start_pos_y + j, resolved_inclination);
                }
            }
        }
    }

    if (do_intersections)
    {
        terwind_put_pixel(start_pos_x, start_pos_y, '*');
        terwind_put_pixel(end_pos_x, end_pos_y, '*');
    }
}