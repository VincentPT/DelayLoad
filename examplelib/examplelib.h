#pragma once
#ifdef _WIN32
#ifdef EXAMPLELIB_EXPORTS
#define EXAMPLELIB_API __declspec(dllexport)
#else
#define EXAMPLELIB_API __declspec(dllimport)
#endif // EXAMPLELIB_EXPORTS
#else
#define EXAMPLELIB_API
#endif // _WIN32


int EXAMPLELIB_API doSomething( int a, int b );