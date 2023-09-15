#include <libsr2/math/mat4x4f.h>
#include <math.h>

namespace sr2 {
    namespace math {
        void FastInverse(mat4x4f& out, const mat4x4f& src) {
        }

        void Dot(mat4x4f& out, const mat4x4f& a, const mat4x4f& b) {
        }

        void Dot(mat4x4f& inout_a, const mat4x4f& b) {
        }

        void zero(mat4x4f& m) {
            m.x = m.y = m.z = m.w = {
                0.0f, 0.0f, 0.0f, 0.0f
            };
        }

        mat4x4f lookAt(const vec3f& eye, const vec3f& center, const vec3f& up) {
            vec3f f = (center - eye).normalized();
            vec3f s = f.cross(up).normalized();
            vec3f u = s.cross(f);

            return {
                { s.x, u.x, -f.x, 0.0f },
                { s.y, u.y, -f.y, 0.0f },
                { s.z, u.z, -f.z, 0.0f },
                { -s.dot(eye), -u.dot(eye), f.dot(eye), 1.0f }
            };
        }

        mat4x4f perspective(f32 fov, f32 aspect, f32 near, f32 far) {
            mat4x4f out;
            zero(out);

            f32 a = 1.0f / tanf(fov * 0.5f);
            f32 b = a * aspect;

            out.x.x = a;
            out.y.y = b;
            out.z.z = (far + near) / (near - far);
            out.z.w = -1.0f;
            out.w.z = (2.0f * near * far) / (near - far);

            return out;
        }
    };

    vec3f mat4x4f::operator*(const vec3f& rhs) const {
        vec3f out;
        out.x = x.x * rhs.x + y.x * rhs.y + z.x * rhs.z + w.x;
        out.y = x.y * rhs.x + y.y * rhs.y + z.y * rhs.z + w.y;
        out.z = x.z * rhs.x + y.z * rhs.y + z.z * rhs.z + w.z;
        return out;
    }

	mat4x4f mat4x4f::operator*(const mat3x4f& rhs) const {
        mat4x4f out;

        out.x.x = x.x * rhs.x.x + x.y * rhs.y.x + x.z * rhs.z.x + x.w * rhs.w.x;
        out.x.y = x.x * rhs.x.y + x.y * rhs.y.y + x.z * rhs.z.y + x.y * rhs.w.y;
        out.x.z = x.x * rhs.x.z + x.y * rhs.y.z + x.z * rhs.z.z + x.z * rhs.w.z;
        out.x.w = 0.0f;

        out.y.x = y.x * rhs.x.x + y.y * rhs.y.x + y.z * rhs.z.x + y.w * rhs.w.x;
        out.y.y = y.x * rhs.x.y + y.y * rhs.y.y + y.z * rhs.z.y + y.w * rhs.w.y;
        out.y.z = y.x * rhs.x.z + y.y * rhs.y.z + y.z * rhs.z.z + y.w * rhs.w.z;
        out.y.w = 0.0f;

        out.z.x = z.x * rhs.x.x + z.y * rhs.y.x + z.z * rhs.z.x + z.w * rhs.w.x;
        out.z.y = z.x * rhs.x.y + z.y * rhs.y.y + z.z * rhs.z.y + z.w * rhs.w.y;
        out.z.z = z.x * rhs.x.z + z.y * rhs.y.z + z.z * rhs.z.z + z.w * rhs.w.z;
        out.z.w = 0.0f;

        out.w.x = w.x * rhs.x.x + w.y * rhs.y.x + w.z * rhs.z.x + w.w * rhs.w.x;
        out.w.y = w.x * rhs.x.y + w.y * rhs.y.y + w.z * rhs.z.y + w.w * rhs.w.y;
        out.w.z = w.x * rhs.x.z + w.y * rhs.y.z + w.z * rhs.z.z + w.w * rhs.w.z;
        out.w.w = 1.0f;

        return out;
    }

    mat4x4f mat4x4f::operator*(const mat4x4f& rhs) const {
        mat4x4f out;

        out.x.x = x.x * rhs.x.x + x.y * rhs.y.x + x.z * rhs.z.x + x.w * rhs.w.x;
        out.x.y = x.x * rhs.x.y + x.y * rhs.y.y + x.z * rhs.z.y + x.y * rhs.w.y;
        out.x.z = x.x * rhs.x.z + x.y * rhs.y.z + x.z * rhs.z.z + x.z * rhs.w.z;
        out.x.w = x.x * rhs.x.w + x.y * rhs.y.w + x.z * rhs.z.w + x.w * rhs.w.w;

        out.y.x = y.x * rhs.x.x + y.y * rhs.y.x + y.z * rhs.z.x + y.w * rhs.w.x;
        out.y.y = y.x * rhs.x.y + y.y * rhs.y.y + y.z * rhs.z.y + y.w * rhs.w.y;
        out.y.z = y.x * rhs.x.z + y.y * rhs.y.z + y.z * rhs.z.z + y.w * rhs.w.z;
        out.y.w = y.x * rhs.x.w + y.y * rhs.y.w + y.z * rhs.z.w + y.w * rhs.w.w;

        out.z.x = z.x * rhs.x.x + z.y * rhs.y.x + z.z * rhs.z.x + z.w * rhs.w.x;
        out.z.y = z.x * rhs.x.y + z.y * rhs.y.y + z.z * rhs.z.y + z.w * rhs.w.y;
        out.z.z = z.x * rhs.x.z + z.y * rhs.y.z + z.z * rhs.z.z + z.w * rhs.w.z;
        out.z.w = z.x * rhs.x.w + z.y * rhs.y.w + z.z * rhs.z.w + z.w * rhs.w.w;

        out.w.x = w.x * rhs.x.x + w.y * rhs.y.x + w.z * rhs.z.x + w.w * rhs.w.x;
        out.w.y = w.x * rhs.x.y + w.y * rhs.y.y + w.z * rhs.z.y + w.w * rhs.w.y;
        out.w.z = w.x * rhs.x.z + w.y * rhs.y.z + w.z * rhs.z.z + w.w * rhs.w.z;
        out.w.w = w.x * rhs.x.w + w.y * rhs.y.w + w.z * rhs.z.w + w.w * rhs.w.w;

        return out;
    }
    
    mat4x4f mat4x4f::operator*=(const mat3x4f& rhs) {
        return *this = *this * rhs;
    }

    mat4x4f mat4x4f::operator*=(const mat4x4f& rhs) {
        return *this = *this * rhs;
    }
    
    mat3x4f mat4x4f::to3x4() const {
        return {
            { x.x, x.y, x.z },
            { y.x, y.y, y.z },
            { z.x, z.y, z.z },
            { w.x, w.y, w.z }
        };
    }
    
    mat3x3f mat4x4f::basis() const {
        return {
            { x.x, x.y, x.z },
            { y.x, y.y, y.z },
            { z.x, z.y, z.z }
        };
    }

    mat4x4f mat4x4f::transposed() const {
        return {
            { x.x, y.x, z.x, w.x },
            { x.y, y.y, z.y, w.y },
            { x.z, y.z, z.z, w.z },
            { x.w, y.w, z.w, w.w }
        };
    }
};