#include "pch.h"
#include "terminal.h"
#include "macros.h"
#include "terwind.h"
#include "logg.h"
#include "thread.h"

void main_exit(int signo);

int main()
{
    signal(SIGINT, main_exit);
    terminal_setup();
    logg_setup(2, "fps.dat", "misc.dat");

    TerminalCanvas_t wnd = terwind_get_canvas();
    terwind_set_buffer(&wnd);

    terwind_game_loop(60);

    main_exit(SIGINT);
}

void main_exit(int signo)
{
    if (signo == SIGINT)
    {
        logg_close();
        terwind_fill_canvas(' ');
        terwind_draw_canvas();
        terminal_reset();
        terwind_free();
        exit(0);
    }
}