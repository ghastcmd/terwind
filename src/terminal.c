#include "terminal.h"

#ifdef _WIN32

static HANDLE hd_stdout;
static DWORD out_mode_init;

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void terminal_setup()
{
    DWORD out_mode = 0;
    hd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hd_stdout, &out_mode);
    
    out_mode_init = out_mode;

    out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hd_stdout, out_mode);
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d");

    SetConsoleMode(hd_stdout, out_mode_init);
}

#else

static struct termios term_old;

void terminal_setup()
{
    struct termios term_new;
    tcgetattr(STDIN_FILENO, &term_old);

    term_new = term_old;

    term_new.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &term_new);

    setbuf(stdout, NULL); //  disables stdout buffer
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d");

    tcsetattr(STDIN_FILENO, TCSANOW, &term_old);
}

int getch(void)
{
    char ret;
    read(STDIN_FILENO, &ret, 1);
    return ret;
}

int kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}

#endif