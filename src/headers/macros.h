#define DEBUG

#ifdef RELEASE
#undef DEBUG
#endif

#ifdef DEBUG
#define LOGG
#endif

#ifdef _WIN32
#define sleep(num) Sleep(num * 1000)

void terminal_setup();

#else
#define terminal_setup() 

#endif