#include "pch.h"
#include "terminal.h"
#include "terwind.h"
#include "logg/logg.h"

void main_exit(int signo);

#define KEY_TEST
#undef KEY_TEST

#ifndef __library__

int main(int argc, char *argv[])
{
    (void)argv, (void)argc;
    signal(SIGINT, main_exit);
    signal(SIGABRT, main_exit);
    terminal_setup();
    logg_setup(2, "fps.dat", "misc.dat");

    TerminalCanvas_t wnd = terwind_get_canvas();
    terwind_set_buffer(&wnd);

#ifdef KEY_TEST
    extern void thread_test_keys();
    thread_test_keys();
#else
    if (argc > 1)
    {
        register int fps_arg = atoi(argv[1]);
        terwind_game_loop(fps_arg);
    }
    else
    {
        terwind_game_loop(60);
    }
#endif

    main_exit(SIGINT);
    return 0;
}

void main_exit(int signo)
{
    logg_close();
    terwind_fill_canvas(' ');
    terwind_draw_canvas();
    terminal_reset();
    terwind_free();

    switch (signo)
    {
    case SIGINT:
        exit(0);
    case SIGABRT:
        exit(3);
    }
}

#endif