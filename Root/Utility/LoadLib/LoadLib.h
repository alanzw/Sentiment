#ifndef SENTIMENT_LOADLIB_H
#define SENTIMENT_LOADLIB_H

#include <string>

#ifdef BUILD_DLL
#define LIBLINK extern "C" __declspec(dllexport)
#else
#define LIBLINK extern "C" __declspec(dllimport)
#endif

void* LoadLib(const char* fileName);

void* GetFunction(void* lib, const char* fnName);

bool UnloadLib(void* lib);


#endif
