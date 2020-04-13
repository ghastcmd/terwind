#define DEBUG

#ifdef RELEASE
#undef DEBUG
#endif

#ifdef DEBUG
#define LOGG
#endif

#ifdef _WIN32
#define sleep(num) Sleep(num * 1000)
#endif