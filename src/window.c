#include "pch.h"

#include "window.h"

#include "terwind.h"
#include "render.h"

void window_draw_box(RenderWindow_t info)
{
    render_line(info.top_corner_x, 
               info.top_corner_y,
               info.top_corner_x + info.width, 
               info.top_corner_y,
               true
    ); 
    render_line(info.top_corner_x, 
               info.top_corner_y,
               info.top_corner_x, 
               info.top_corner_y + info.line_count,
               true
    );
    render_line(info.top_corner_x, 
               info.top_corner_y + info.line_count,
               info.top_corner_x + info.width, 
               info.top_corner_y + info.line_count,
               true
    );
    render_line(info.top_corner_x + info.width, 
               info.top_corner_y,
               info.top_corner_x + info.width, 
               info.top_corner_y + info.line_count,
               true
    );
}