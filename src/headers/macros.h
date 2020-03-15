#ifdef _WIN32
#define sleep(num) Sleep(num * 1000)

void terminal_setup();

#else
#define setup_func() {};

#endif