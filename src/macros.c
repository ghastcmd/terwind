#include "pch.h"
#include "macros.h"

#ifdef _WIN32
void terminal_setup()
{
    system("color 0");
}
#endif