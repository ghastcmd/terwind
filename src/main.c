#include "pch.h"
#include "terminal.h"
#include "macros.h"
#include "terwind.h"
#include "logg.h"

void main_exit_cleanup(int signo)
{
    switch (signo)
    {
    case SIGINT:
        terwind_fill_canvas(' ');
        terwind_draw_canvas();
        terminal_reset();
        terwind_free();
        logg_close();
        exit(0);
    default:
        perror("undefined behaviour\n");
        exit(-1);
    }
}

int main()
{
    signal(SIGINT, main_exit_cleanup);
    terminal_setup();
    logg_setup(1, "fps.dat");

    printf("\x1b[32m");

    TerminalCanvas_t wnd = terwind_get_canvas();
    terwind_set_buffer(&wnd);

    terwind_game_loop(60);

    raise(SIGINT);
}