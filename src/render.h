#pragma once

void render_letters(float start_pos_x, float start_pos_y, const char * to_write, size_t string_size);

void assign_logger();

const char * get_logger();

void remove_logger();

void render_line(float start_pos_x, float start_pos_y, float end_pos_x, float end_pos_y, bool do_intersection);