#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>
#include <stdarg.h>
#include <signal.h>
#include <math.h>


#ifdef _WIN32
#include <Windows.h>
#include <conio.h>

#include <direct.h>
#include <AccCtrl.h>
#include <AclAPI.h>

#else
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <dirent.h>

#include <sys/time.h>

#include <dlfcn.h>


#define INIT

#endif