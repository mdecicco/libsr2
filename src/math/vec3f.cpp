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

        void copy(vec3f& a, const vec3f& b) {
            a.x = b.x;
            a.y = b.y;
            a.z = b.z;
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
    };
};