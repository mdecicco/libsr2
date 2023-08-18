#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace math {
        constexpr f32 RPS_TO_RPM = 9.549296f; // radians/sec -> rotations/min
        constexpr f32 RPM_TO_RPS = 0.1047198f; // rotations/min -> radians/sec
        constexpr f32 NMS_TO_HP = 0.001340483f; // Newton*meter/sec -> horsepower
        constexpr f32 DEG_TO_RAD = 0.01745329f;
        constexpr f32 RAD_TO_DEG = 57.2957795f;
        constexpr f32 FIBONACCI_NUMBER = 1.618034f;
        
        template <typename T> constexpr T min(T a, T b) { return a < b ? a : b; }
        template <typename T> constexpr T max(T a, T b) { return a < b ? b : a; }

        f32 RealCubic(f32 a, f32 b, f32 c, f32 d, f32* result_0, f32* result_1, f32* result_2);
        f32 CubeRoot(f32 x);
        f32 frand();

        f32 findHomingAccel(f32 a, f32 b, f32 c, f32 d, f32 e);
    };
};