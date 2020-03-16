#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>

#ifdef _WIN32
#include <windows.h>

#else
#include <sys/ioctl.h>
#include <unistd.h>

#define INIT

#endif