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
        void mult_with_translation(mat3x4f& out, const mat3x4f& lhs, const mat3x4f& rhs);
        void mult_inverse_with_translation(mat3x4f& lhs, const mat3x4f& rhs);
        void mult(mat3x4f& out, const mat3x4f& lhs, const mat3x4f& rhs);
        void mult(vec3f& out, const mat3x4f& lhs, const vec3f& rhs);
        void mult_transposed(vec3f& out, const mat3x4f& lhs, const vec3f& rhs);
        void mult_transposed(mat3x4f& out, const mat3x4f& lhs, const mat3x4f& rhs);
        void rotation_x(mat3x4f& out, f32 angle);
        void rotation_y(mat3x4f& out, f32 angle);
        void rotation_z(mat3x4f& out, f32 angle);
        void rotation_axis_angle(mat3x4f& out, const vec3f& axis, f32 angle);
        void make_rotation(mat3x4f& out, const vec3f& axis, f32 angle);
        void from_eulers_zxy(mat3x4f& out, const vec3f& eulers);
        void from_eulers_xzy(mat3x4f& out, const vec3f& eulers);
        void from_quaternion(mat3x4f& out, const quat& q);
        void from_position_and_length_encoded(mat3x4f& out, const vec3f& pos, const vec3f& encoded);
        void rotate(mat3x4f& out, const vec3f& axis, f32 angle);
        void dot3x3CrossProdMtx(mat3x4f& out, const vec3f& unk);
    };
};