#include "pch.h"

#include "window.h"

#include "terwind.h"
#include "render.h"

void window_draw_box(RenderWindow_t info)
{
    render_line(info.top_corner_x, 
               info.top_corner_y,
               info.top_corner_x + info.width + 1, 
               info.top_corner_y,
               true
    ); 
    render_line(info.top_corner_x, 
               info.top_corner_y,
               info.top_corner_x, 
               info.top_corner_y + info.line_count + 1,
               true
    );
    render_line(info.top_corner_x, 
               info.top_corner_y + info.line_count + 1,
               info.top_corner_x + info.width + 1, 
               info.top_corner_y + info.line_count + 1,
               true
    );
    render_line(info.top_corner_x + info.width + 1, 
               info.top_corner_y,
               info.top_corner_x + info.width + 1, 
               info.top_corner_y + info.line_count + 1,
               true
    );

    for (size_t i = 0; i < (info.lines_length); ++i)
    {
        if (i >= (size_t)info.line_count) break;
        int line_size = strlen(info.lines_content[i]);
        render_letters(info.top_corner_x + 1,
                       info.top_corner_y + 1 + i,
                       info.lines_content[i],
                       line_size < info.width ? line_size : info.width
        ); 
    }
}