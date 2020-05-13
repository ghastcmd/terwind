#pragma once

void terminal_setup();

void terminal_reset();

#ifndef _WIN32
int getch(void);
int kbhit(void);

#endif