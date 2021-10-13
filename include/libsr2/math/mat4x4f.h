#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace math {
        void FastInverse(mat4x4f& out, const mat4x4f& src);
        void Dot(mat4x4f& out, const mat4x4f& a, const mat4x4f& b);
        void Dot(mat4x4f& inout_a, const mat4x4f& b);
    };
};