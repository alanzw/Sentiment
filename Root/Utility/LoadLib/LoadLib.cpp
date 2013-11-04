#include "LoadLib.h"

#ifdef _WIN32

#include <windows.h>

void* LoadLib(const char* fileName)
{
    std::string file(fileName);
    file+= ".dll";
    return (void*)LoadLibrary(file.c_str());
}

void* GetFunction(void* lib, const char* fnName)
{
    return (void*) GetProcAddress((HINSTANCE)lib, fnName);
}

bool UnloadLib(void* lib)
{
    return FreeLibrary((HINSTANCE) lib);
}

#elif _UNIX

#include <dlfcn.h>

void LoadLibrary(const char* fileName)
{
    std::string file(fileName);
    file+=".so";
    return dlopen(file.c_str(), 2);
}

void GetFunction(void* lib, const char* fnName)
{
    return dlsym(lib, fnName);
}

bool UnloadLib(void* lib)
{
    return dlclose(lib);
}

#else
#error Loading of Dynmaic Libraries not supported on your operating system
#endif
