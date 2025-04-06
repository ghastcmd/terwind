#include "pch.h"

#include "render.h"

#include "terwind.h"
#include "logg/logg.h"

void render_letters(float start_pos_x, float start_pos_y, const char * to_write, size_t string_size)
{
    for (size_t i = 0; i < string_size-1; ++i)
    {
        terwind_put_pixel(start_pos_x + i, start_pos_y, to_write[i]);
    }
}

void render_line(float start_pos_x, float start_pos_y, float end_pos_x, float end_pos_y)
{
    TerDim_t dims = terwind_get_dimensions();
    float x_delta_int = end_pos_x - start_pos_x;
    float y_delta_int = end_pos_y - start_pos_y;

    float pixel_height = (1.f / dims.height);
    float pixel_width = (1.f / dims.width);
    // float pixel_coef = (pixel_width + 0.5f) / (pixel_height + 0.5f);

    float true_coords_start_x = (start_pos_x / dims.width) - 0.5f - (pixel_width / 2.0f);
    float true_coords_start_y = (start_pos_y / dims.height) - 0.5f - (pixel_height / 2.0f);
    
    float true_coords_end_x = (end_pos_x / dims.width) - 0.5f + (pixel_width / 2.0f);;
    float true_coords_end_y = (end_pos_y / dims.height) - 0.5f + (pixel_height / 2.0f);;

    float x_delta = true_coords_end_x - true_coords_start_x;
    float y_delta = true_coords_end_y - true_coords_start_y;
    float coef = fabsf(x_delta / y_delta);

    logg_info("vv: %f %f\n", pixel_width, pixel_height);

    for (int i = 0; i <= x_delta_int; ++i)
    {
        for (int j = 0; j <= y_delta_int; ++j)
        {
            float current_pos_x_upper_right = ((start_pos_x + i) / dims.width) - 0.5f + (pixel_width / 2.0f);
            float current_pos_y_upper_right = ((start_pos_y + j) / dims.height) - 0.5f - (pixel_height / 2.0f);

            float current_x_delta_upper_right = current_pos_x_upper_right - true_coords_start_x;
            float current_y_delta_upper_right = current_pos_y_upper_right - true_coords_start_y;
            float current_upper_right_delta = fabsf(current_x_delta_upper_right / current_y_delta_upper_right);

            float current_pos_x_lower_left = ((start_pos_x + i) / dims.width) - 0.5f - (pixel_width / 2.0f);
            float current_pos_y_lower_left = ((start_pos_y + j) / dims.height) - 0.5f + (pixel_height / 2.0f);
            
            float current_x_delta_lower_left = current_pos_x_lower_left - true_coords_start_x;
            float current_y_delta_lower_left = current_pos_y_lower_left - true_coords_start_y;
            float current_lower_left_delta = fabsf(current_x_delta_lower_left / current_y_delta_lower_left);
            
            logg_info("v: lf %f, c %f, up %f\n", current_lower_left_delta, coef, current_upper_right_delta);

            if (coef <= current_upper_right_delta && coef >= current_lower_left_delta)
            {
                terwind_put_pixel(start_pos_x + i, start_pos_y + j, '#');
            }
        }
    }
}