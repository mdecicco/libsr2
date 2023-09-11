#include <libsr2/math/vec3f.h>
#include <math.h>

namespace sr2 {
    namespace math {
        void normalize(vec3f& out, const vec3f& v) {
            f32 imag = 1.0f / magnitude(v);
            out.x = v.x * imag;
            out.y = v.y * imag;
            out.z = v.z * imag;
        }

        void normalize(vec3f& out) {
            f32 imag = 1.0f / magnitude(out);
            out.x *= imag;
            out.y *= imag;
            out.z *= imag;
        }

        void cross(vec3f& out, const vec3f& a, const vec3f& b) {
            vec3f o;
            o.x = a.y * b.z - b.y * a.z;
            o.y = a.z * b.x - b.z * a.x;
            o.z = a.x * b.y - b.x * a.y;
            copy(out, o);
        }

        f32 dot(const vec3f& a, const vec3f& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        void copy(vec3f& to, const vec3f& from) {
            to.x = from.x;
            to.y = from.y;
            to.z = from.z;
        }

        void zero(vec3f& a) {
            a.x = a.y = a.z = 0.0f;
        }

        void negate(vec3f& a) {
            a.x = -a.x;
            a.y = -a.y;
            a.z = -a.z;
        }

        void negate(vec3f& a, const vec3f& b) {
            a.x = -b.x;
            a.y = -b.y;
            a.z = -b.z;
        }

        f32 magnitude(const vec3f& v) {
            return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        }

        f32 magnitudeSq(const vec3f& v) {
            return v.x * v.x + v.y * v.y + v.z * v.z;
        }

        void mult(vec3f& o, const vec3f& lhs, const vec3f& rhs) {
            o.x = lhs.x * rhs.x;
            o.y = lhs.y * rhs.y;
            o.z = lhs.z * rhs.z;
        }

        void mult(vec3f& o, const vec3f& lhs, f32 rhs) {
            o.x = lhs.x * rhs;
            o.y = lhs.y * rhs;
            o.z = lhs.z * rhs;
        }

        void sub(vec3f& o, const vec3f& lhs, const vec3f& rhs) {
            o.x = lhs.x - rhs.x;
            o.y = lhs.y - rhs.y;
            o.z = lhs.z - rhs.z;
        }

        void add(vec3f& o, const vec3f& lhs, const vec3f& rhs) {
            o.x = lhs.x + rhs.x;
            o.y = lhs.y + rhs.y;
            o.z = lhs.z + rhs.z;
        }

        void mult(vec3f& lhs, const vec3f& rhs) {
            lhs.x *= rhs.x;
            lhs.y *= rhs.y;
            lhs.z *= rhs.z;
        }

        void mult(vec3f& lhs, f32 rhs) {
            lhs.x *= rhs;
            lhs.y *= rhs;
            lhs.z *= rhs;
        }

        void sub(vec3f& lhs, const vec3f& rhs) {
            lhs.x -= rhs.x;
            lhs.y -= rhs.y;
            lhs.z -= rhs.z;
        }

        void add(vec3f& lhs, const vec3f& rhs) {
            lhs.x += rhs.x;
            lhs.y += rhs.y;
            lhs.z += rhs.z;
        }

        bool VertOrderIsOK(vec3f* verts) {
            vec3f d10;
            sub(d10, verts[1], verts[0]);
            vec3f d20;
            sub(d20, verts[2], verts[0]);
            vec3f d21;
            sub(d21, verts[2], verts[1]);

            f32 f0 = dot(d20, d10);
            negate(d10);
            f32 f1 = dot(d21, d10);

            if (f0 < 0.0f || f1 < 0.0f) return false;
            return true;
        }

        void ReOrderVerts(vec3f* verts, f32* param_2) {
            while (!VertOrderIsOK(verts)) {
                f32 x0 = verts[0].x;
                f32 y0 = verts[0].y;
                f32 z0 = verts[0].z;
                verts[0].x = verts[1].x;
                verts[0].y = verts[1].y;
                verts[0].z = verts[1].z;
                verts[1].x = verts[2].x;
                verts[1].y = verts[2].y;
                verts[1].z = verts[2].z;
                verts[2].x = x0;
                verts[2].y = y0;
                verts[2].z = z0;

                if (param_2) {
                    f32 tmp = param_2[0];
                    param_2[0] = param_2[1];
                    param_2[1] = param_2[2];
                    param_2[2] = tmp;
                }
            }
        }

        void FUN_0031b2f8(vec3f& out, const vec3f& in) {
            f32 outDotIn = dot(out, in);
            if (outDotIn <= 0.0f) {
                add(out, in);
                return;
            }

            f32 pMagSq = magnitudeSq(in);
            if (outDotIn < pMagSq) {
                vec3f x;
                mult(x, in, 1.0f - outDotIn / pMagSq);
                add(out, x);
            }
        }
    };
};