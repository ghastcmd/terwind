#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>
#include <stdarg.h>
#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>

#else
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>

#include <pthread.h>
#include <unistd.h>
#include <termios.h>

#define INIT

#endif