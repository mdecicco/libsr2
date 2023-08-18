#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace math {
        void from_matrix(quat& out, const mat3x4f& m);
        void from_length_encoded(quat& out, const vec3f& encoded);
        void from_axis_angle(quat& out, const vec3f& axis, f32 angle);
        void rotate(const quat& q, const vec3f& in, vec3f& out);
        void mult(quat& out, const quat& lhs, const quat& rhs);
    };
};