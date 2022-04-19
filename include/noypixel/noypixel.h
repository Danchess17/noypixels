#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cstdlib>
#include <string>
#include <cstring>

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#ifdef DEBUG
#define ASSERT(x, msg) do { \
    if (!(x)) { \
        std::cerr << "\033[1;31mnoypixel: assetion failed: '" << msg << "' at " << \
            __FILE__ << ':' << __LINE__ << ':' << __func__ << "\033[0m\n"; \
        std::exit(1); \
    } } while (false)
#else
#define ASSERT(x, msg)
#endif

#include "noypixel/log.h"

#include "sprite.h"
#include "canvas.h"
#include "project.h"
#include "serializer.h"

#include "graphics.h"
