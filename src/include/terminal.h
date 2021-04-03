#pragma once

typedef struct {
    int height, width;
} tupledim_t;

tupledim_t terminal_get_dims();

typedef enum terminal_colors
{
    black = 30, red, green, yellow,
    blue, magenta, cyan, white
} tcolor_t;

void terminal_set_bgcolor(tcolor_t color);
void terminal_set_color(tcolor_t color);
void terminal_reset_color();

void terminal_zeropos();
void terminal_psleep(int psec);

void terminal_setup();

void terminal_reset();

#ifndef _WIN32
int getch(void);
int kbhit(void);

#endif