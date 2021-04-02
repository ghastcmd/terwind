#pragma once

typedef struct {
    int height, width;
} tupledim_t;

tupledim_t terminal_get_dims();

void terminal_setup();

void terminal_reset();

#ifndef _WIN32
int getch(void);
int kbhit(void);

#endif