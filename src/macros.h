#pragma once

#define DEBUG

#ifdef RELEASE
#undef DEBUG
#endif

#ifdef DEBUG
// #define LOGG
#endif

#ifdef _WIN32
#define sleep(num) Sleep(num * 1000)
#endif


#ifdef _WIN32

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#else

#define DLLEXPORT __attribute__((visibility("default")))
#define DLLIMPORT

#endif



#ifdef _EXPORT

#define RELOAD DLLEXPORT

#else

#define RELOAD

#endif