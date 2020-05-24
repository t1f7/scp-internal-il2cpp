#pragma once
// x86 support
#ifndef POINTER
#ifdef _WIN64
typedef unsigned long long pointer;
#else
typedef unsigned int       pointer;
#endif
#define POINTER 1
#endif