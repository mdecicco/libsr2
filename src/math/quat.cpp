#include <libsr2/math/quat.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/math.h>
#include <math.h>

namespace sr2 {
    namespace math {
        u8 getMinParamIdx(f32 a, f32 b, f32 c, f32 d) {
            u8 idx = 0;
            f32 min = a;
            
            if (b < min) {
                idx = 1;
                min = b;
            }
            
            if (c < min) {
                idx = 2;
                min = c;
            }
            
            if (d < min) {
                idx = 3;
                min = d;
            }

            return idx;
        }

        void from_matrix(quat& out, const mat3x4f& m) { 
            f32 xx = m.x.x;
            f32 yy = m.y.y;
            f32 zz = m.z.z;
            f32 sum = xx + yy + zz;

            u8 minIdx = getMinParamIdx(sum, xx, yy, zz);

            switch (minIdx) {
                case 0: {
                    f32 unk = sqrtf(sum + 1.0f) * 0.5f;
                    f32 unk1 = 0.25f / unk;

                    out.axis.x = (m.y.z - m.z.y) * unk1;
                    out.axis.y = (m.z.x - m.x.z) * unk1;
                    out.axis.z = (m.x.y - m.y.x) * unk1;
                    out.angle = unk;
                    break;
                }
                case 1: {
                    f32 unk = sqrtf(((m.x.x + m.x.x) - sum) + 1.0f) * 0.5f;
                    f32 unk1 = 0.25f / unk;

                    out.axis.x = unk;
                    out.axis.y = (m.x.y + m.y.x) * unk1;
                    out.axis.z = (m.z.x + m.x.z) * unk1;
                    out.angle = (m.y.z - m.z.y) * unk1;
                    break;
                }
                case 2: {
                    f32 unk = sqrtf(((m.y.y + m.y.y) - sum) + 1.0) * 0.5;
                    f32 unk1 = 0.25 / unk;

                    out.axis.x = (m.x.y + m.y.x) * unk1;
                    out.axis.y = unk;
                    out.axis.z = (m.y.z + m.z.y) * unk1;
                    out.angle = (m.z.x - m.x.z) * unk1;
                    break;
                }
                case 3: {
                    f32 unk = sqrtf(((m.z.z + m.z.z) - sum) + 1.0f) * 0.5f;
                    f32 unk1 = 0.25f / unk;

                    out.axis.x = (m.z.x + m.x.z) * unk1;
                    out.axis.y = (m.y.z + m.z.y) * unk1;
                    out.axis.z = unk;
                    out.angle = (m.x.y - m.y.x) * unk1;
                    break;
                }
            }
        }

        void from_length_encoded(quat& out, const vec3f& encoded) {
            f32 magSq = math::magnitudeSq(encoded);
            if (magSq > 0.0f) {
                f32 mag = sqrtf(magSq);
                vec3f norm;
                math::mult(norm, encoded, 1.0f / mag);
                return from_axis_angle(out, norm, mag);
            }

            out.axis = { 0.0f, 0.0f, 0.0f };
            out.angle = 1.0f;
        }

        void from_axis_angle(quat& out, const vec3f& axis, f32 angle) {
            math::mult(out.axis, axis, sinf(angle * 0.5f));
            out.angle = cosf(angle * 0.5f);
        }
        
        void rotate(const quat& q, const vec3f& in, vec3f& out) {
            vec3f result;

            math::mult(result, q.axis, math::dot(q.axis, in) * 2.0f);

            vec3f tmp;
            math::mult(tmp, in, (((q.angle * q.angle) * 2.0f) - 1.0f));
            math::add(result, tmp);

            math::cross(tmp, q.axis, in);
            math::mult(tmp, q.angle * 2.0f);
            math::add(result, tmp);

            math::copy(out, result);
        }
        
        void unrotate(const quat& q, const vec3f& in, vec3f& out) { 
            vec3f result;
            
            math::mult(result, q.axis, math::dot(q.axis, in) * 2.0f);

            vec3f tmp;
            math::mult(tmp, in, ((q.angle * q.angle) * 2.0f) - 1.0f);
            math::add(result, tmp);

            math::cross(tmp, q.axis, in);
            math::mult(tmp, q.angle * 2.0f);
            math::sub(result, tmp);

            math::copy(out, result);
        }
        
        void mult(quat& out, const quat& lhs, const quat& rhs) {
            quat o;
            o.axis.x = (lhs.angle * rhs.axis.x + rhs.angle * lhs.axis.x + lhs.axis.y * rhs.axis.z) - lhs.axis.z * rhs.axis.y;
            o.axis.y = (lhs.angle * rhs.axis.y + rhs.angle * lhs.axis.y + lhs.axis.z * rhs.axis.x) - lhs.axis.x * rhs.axis.z;
            o.axis.z = (lhs.angle * rhs.axis.z + rhs.angle * lhs.axis.z + lhs.axis.x * rhs.axis.y) - lhs.axis.y * rhs.axis.x;
            o.angle = ((lhs.angle * rhs.angle - lhs.axis.x * rhs.axis.x) - lhs.axis.y * rhs.axis.y) - lhs.axis.z * rhs.axis.z;
        }
        
        void mult(quat& lhs, const quat& rhs) {
            quat o;
            o.axis.x = (lhs.angle * rhs.axis.x + rhs.angle * lhs.axis.x + lhs.axis.y * rhs.axis.z) - lhs.axis.z * rhs.axis.y;
            o.axis.y = (lhs.angle * rhs.axis.y + rhs.angle * lhs.axis.y + lhs.axis.z * rhs.axis.x) - lhs.axis.x * rhs.axis.z;
            o.axis.z = (lhs.angle * rhs.axis.z + rhs.angle * lhs.axis.z + lhs.axis.x * rhs.axis.y) - lhs.axis.y * rhs.axis.x;
            o.angle = ((lhs.angle * rhs.angle - lhs.axis.x * rhs.axis.x) - lhs.axis.y * rhs.axis.y) - lhs.axis.z * rhs.axis.z;

            lhs.axis = o.axis;
            lhs.angle = o.angle;
        }

        void negate(quat& lhs) {
            lhs.axis = {
                -lhs.axis.x,
                -lhs.axis.y,
                -lhs.axis.z
            };
            lhs.angle = -lhs.angle;
        }

        f32 dot(const quat& a, const quat& b) {
            return a.axis.x * b.axis.x + a.axis.y * b.axis.y + a.axis.z * b.axis.z + a.angle * b.angle;
        }

        void FUN_002c3720(f32 p0, f32 dt, vec3f& out, const quat& p4, const vec3f& p5, const quat& p6) {
            quat a = p4;
            math::negate(a.axis);
            math::mult(a, p6);

            vec3f b = a.axis;
            vec3f rotated;
            math::rotate(p4, b, rotated);
            math::normalize(rotated);

            f32 c = math::dot(rotated, p5);
            vec3f t0;
            math::mult(t0, rotated, math::dot(rotated, p5));
            math::sub(t0, p5, t0);
            math::normalize(t0);

            f32 t3 = 0.0f;
            if (a.angle > 0.0f) {
                if (a.angle < 1.0f) t3 = acosf(a.angle) * 2.0f;
            } else t3 = 3.141593f;

            vec3f t4, t5;
            math::mult(t4, t0, -math::dot(t0, p5) * dt);
            math::mult(t5, rotated, findHomingAccel(p0, 0.0f, c, t3, 0.0f));
            math::add(out, t4, t5);
        }
    };
};