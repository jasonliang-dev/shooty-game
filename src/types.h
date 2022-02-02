#pragma once

#include <stdint.h>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

struct vec2 {
    // wtf gcc
    vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    float x;
    float y;
};

struct Rect {
    float x1, y1, x2, y2;
};

#define array_count(a) (sizeof(a) / sizeof((a)[0]))
