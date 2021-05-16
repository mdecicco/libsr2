#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace math {
        void identity(mat3x4f& m);
        void identity_3x3(mat3x4f& m);
        void zero(mat3x4f& m);
        void copy(mat3x4f& to, const mat3x4f& from);
        void transpose(mat3x4f& m);
        void transposed(mat3x4f& out, const mat3x4f& in);
        void mult(mat3x4f& lhs, const mat3x4f& rhs);
        void mult_with_translation(mat3x4f& lhs, const mat3x4f& rhs);
        void mult(mat3x4f& out, const mat3x4f& lhs, const mat3x4f& rhs);
        void mult(vec3f& out, const mat3x4f& lhs, const vec3f& rhs);
    };
};