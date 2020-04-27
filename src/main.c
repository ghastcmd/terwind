#include "pch.h"
#include "terminal.h"
#include "terwind.h"
#include "logg.h"

void main_exit(int signo);

int main()
{
    signal(SIGINT, main_exit);
    terminal_setup();
    logg_setup(2, "fps.dat", "misc.dat");

    TerminalCanvas_t wnd = terwind_get_canvas();
    terwind_set_buffer(&wnd);

#ifdef KEY_TEST
    thread_test_keys();
#else
    terwind_game_loop(60);
#endif

    main_exit(SIGINT);
}

void main_exit(int signo)
{
    if (signo)
    {
        logg_close();
        terwind_fill_canvas(' ');
        terwind_draw_canvas();
        terminal_reset();
        terwind_free();
        exit(0);
    }
}