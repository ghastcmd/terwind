#include "pch.h"

#include "render_letters.h"

#include "terwind.h"

void render_letters(float start_pos_x, float start_pos_y, const char * to_write, size_t string_size)
{
    for (size_t i = 0; i < string_size-1; ++i)
    {
        terwind_put_pixel(start_pos_x + i, start_pos_y, to_write[i]);
    }
}