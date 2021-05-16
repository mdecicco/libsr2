#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace math {
        void normalize(vec3f& out, const vec3f& v);
        void normalize(vec3f& out);
        void cross(vec3f& out, const vec3f& a, const vec3f& b);
        f32 dot(const vec3f& a, const vec3f& b);
        void copy(vec3f& a, const vec3f& b);
        void zero(vec3f& a);
        void negate(vec3f& a);
        void negate(vec3f& a, const vec3f& b);
        f32 magnitude(const vec3f& v);
        f32 magnitudeSq(const vec3f& v);
        void mult(vec3f& o, const vec3f& lhs, const vec3f& rhs);
        void sub(vec3f& o, const vec3f& lhs, const vec3f& rhs);
        void add(vec3f& o, const vec3f& lhs, const vec3f& rhs);
        void sub(vec3f& lhs, const vec3f& rhs);
        void add(vec3f& lhs, const vec3f& rhs);
    };
};