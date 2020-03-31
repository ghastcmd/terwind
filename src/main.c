#include "pch.h"
#include "terminal.h"
#include "macros.h"
#include "terwind.h"
#include "logg.h"

int main()
{
    terminal_setup();
    logg_setup(1, "fps.dat");

    printf("\x1b[32m");

    TerminalCanvas_t wnd = terwind_get_canvas();
    terwind_set_buffer(&wnd);

    terwind_game_loop(60);

    terwind_free(wnd);
    terminal_reset();
    logg_close();
    return 0;
}