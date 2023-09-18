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
        
        out.x.x = x.x * rhs.x.x + y.x * rhs.x.y + z.x * rhs.x.z + w.x * rhs.x.w;
        out.x.y = x.y * rhs.x.x + y.y * rhs.x.y + z.y * rhs.x.z + w.y * rhs.x.w;
        out.x.z = x.z * rhs.x.x + y.z * rhs.x.y + z.z * rhs.x.z + w.z * rhs.x.w;
        out.x.w = x.w * rhs.x.x + y.w * rhs.x.y + z.w * rhs.x.z + w.w * rhs.x.w;
        out.y.x = x.x * rhs.y.x + y.x * rhs.y.y + z.x * rhs.y.z + w.x * rhs.y.w;
        out.y.y = x.y * rhs.y.x + y.y * rhs.y.y + z.y * rhs.y.z + w.y * rhs.y.w;
        out.y.z = x.z * rhs.y.x + y.z * rhs.y.y + z.z * rhs.y.z + w.z * rhs.y.w;
        out.y.w = x.w * rhs.y.x + y.w * rhs.y.y + z.w * rhs.y.z + w.w * rhs.y.w;
        out.z.x = x.x * rhs.z.x + y.x * rhs.z.y + z.x * rhs.z.z + w.x * rhs.z.w;
        out.z.y = x.y * rhs.z.x + y.y * rhs.z.y + z.y * rhs.z.z + w.y * rhs.z.w;
        out.z.z = x.z * rhs.z.x + y.z * rhs.z.y + z.z * rhs.z.z + w.z * rhs.z.w;
        out.z.w = x.w * rhs.z.x + y.w * rhs.z.y + z.w * rhs.z.z + w.w * rhs.z.w;
        out.w.x = x.x * rhs.w.x + y.x * rhs.w.y + z.x * rhs.w.z + w.x * rhs.w.w;
        out.w.y = x.y * rhs.w.x + y.y * rhs.w.y + z.y * rhs.w.z + w.y * rhs.w.w;
        out.w.z = x.z * rhs.w.x + y.z * rhs.w.y + z.z * rhs.w.z + w.z * rhs.w.w;
        out.w.w = x.w * rhs.w.x + y.w * rhs.w.y + z.w * rhs.w.z + w.w * rhs.w.w;

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