#include "pch.h"
#include "terminal.h"

tupledim_t terminal_get_dims()
{
    tupledim_t dims;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    
    dims.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    dims.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

#else
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    dims.width = window.ws_col;
    dims.height = window.ws_row;

#endif
    return dims;
}

void terminal_set_color(tcolor_t color)
{
    printf("\x1b[%im", color);
}

void terminal_set_bgcolor(tcolor_t color)
{
    terminal_set_color(color + 10);
}

void terminal_reset_color()
{
    printf("\033[0m");
}

/**
 esc[PL;PcH
 esc[PnA - up
 esc[PnB - down
 esc[PnC - forward
 esc[PnD - backward
 esc[s - saves position
 esc[u - restores position
 esc[J - clears screen and moves cursor to end
 esc[2J - clears screen and moves cursor do start
 esc[K - clears the line from cursor positon to end
 esc[2K - clears the entire line
*/

#define move_func(name, code) \
void name(int count) \
{ \
    printf("\x1b[%i"#code, count); \
}

#define noval_func(name, code) \
void name() \
{ \
    printf("\x1b["#code); \
}

move_func(terminal_move_up, A)
move_func(terminal_move_down, B)
move_func(terminal_move_forward, C)
move_func(terminal_move_backward, D)

noval_func(terminal_save_pos, s)
noval_func(terminal_restore_pos, u)
noval_func(terminal_clear_screen, J)
noval_func(terminal_clear_screen_start, 2J)
noval_func(terminal_clear_line_end, K)
noval_func(terminal_clear_line, 2K)

void terminal_set_pos(int line, int column)
{
    printf("\x1b[%i;%iH", line, column);
}

void terminal_zeropos()
{
    printf("\x1b[0d");
}

void terminal_psleep(int psec)
{
    struct timespec time1 = { 0, 0 }, time2 = { 0, 0 };
    time1.tv_nsec = psec * 1000;
    nanosleep(&time1, &time2);
}

void terminal_hide_cursor()
{
    printf("\e[?25l");
}

void terminal_show_cursor()
{
    printf("\e[?25h");
}

#ifdef _WIN32

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

HANDLE terminal_get_console_handle()
{
    static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    return handle; 
}

void terminal_hidecursor()
{
    static const HANDLE console_handle = terminal_get_console_handle();
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(console_handle, &cursor_info);

    cursor_info.bVisible = false;
    SetConsoleCursorInfo(console_handle, &cursor_info);
}

void terminal_show_cursor()
{
    static const HANDLE console_handle = terminal_get_console_handle();
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(console_handle, &cursor_info);

    cursor_info.bVisible = true;
    SetConsoleCursorInfo(console_handle, &cursor_info);
}

void terminal_setup()
{
    DWORD out_mode = 0;
    HANDLE hd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hd_stdout, &out_mode);
    
    out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hd_stdout, out_mode);
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d"); // resets color and cursor position

    DWORD out_mode = 0;
    HANDLE hd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hd_stdout, &out_mode);

    out_mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hd_stdout, out_mode);
}

#else

void terminal_setup()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    term.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    setbuf(stdout, NULL); //  disables stdout buffer
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d");

    struct termios term;

    tcgetattr(STDIN_FILENO, &term);

    term.c_lflag |= (ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int getch(void)
{
    char ret;
    read(STDIN_FILENO, &ret, 1);
    return ret;
}

int kbhit (void)
{
    int remaining_bytes;
    ioctl(STDIN_FILENO, FIONREAD, &remaining_bytes);
    return remaining_bytes > 0;
}

#endif