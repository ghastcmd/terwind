#pragma once

typedef struct {
    int top_corner_x, top_corner_y, width, line_count;
    char **lines_content;
    size_t lines_length;
} RenderWindow_t;

void window_draw_box(RenderWindow_t infos);