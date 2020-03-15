#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>

#define sleep(num) Sleep(num * 1000)

void setup_function();
#define INIT setup_function()

#else
#include <sys/ioctl.h>
#include <unistd.h>

#define INIT

#endif