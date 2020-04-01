#include "terminal.h"

#ifdef _WIN32

static HANDLE hd_stdout;
static DWORD out_mode_init;

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

void terminal_setup()
{
    DWORD out_mode = 0;
    hd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hd_stdout, &out_mode);
    
    out_mode_init = out_mode;

    out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hd_stdout, out_mode);
    
    // setbuf(stdout, NULL);
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

    setbuf(stdout, NULL);
}

void terminal_reset()
{
    printf("\x1b[0m\x1b[0d");

    tcsetattr(STDIN_FILENO, TCSANOW, &term_old);
}

#endif