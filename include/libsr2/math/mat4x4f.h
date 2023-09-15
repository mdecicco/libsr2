#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace math {
        void FastInverse(mat4x4f& out, const mat4x4f& src);
        void Dot(mat4x4f& out, const mat4x4f& a, const mat4x4f& b);
        void Dot(mat4x4f& inout_a, const mat4x4f& b);
        void zero(mat4x4f& m);
        mat4x4f lookAt(const vec3f& eye, const vec3f& center, const vec3f& up);
        mat4x4f perspective(f32 fov, f32 aspect, f32 near, f32 far);
    };
};