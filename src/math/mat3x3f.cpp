#include <libsr2/types.h>

namespace sr2 {
    vec3f mat3x3f::operator*(const vec3f& rhs) const {
        return {
            x.x * rhs.x + rhs.y * y.x + rhs.z * z.x,
            x.y * rhs.x + rhs.y * y.y + rhs.z * z.y,
            x.z * rhs.x + rhs.y * y.z + rhs.z * z.z
        };
    }
	mat3x3f mat3x3f::inverse() const {
        f32 det = x.x * (y.y * z.z - z.y * y.z) - x.y * (y.x * z.z - y.z * z.x) + x.z * (y.x * z.y - y.y * z.x);
        f32 invdet = 1.0f / det;

        mat3x3f out;
        out.x.x = (y.y * z.z - z.y * y.z) * invdet;
        out.x.y = (x.z * z.y - x.y * z.z) * invdet;
        out.x.z = (x.y * y.z - x.z * y.y) * invdet;
        out.y.x = (y.z * z.x - y.x * z.z) * invdet;
        out.y.y = (x.x * z.z - x.z * z.x) * invdet;
        out.y.z = (y.x * x.z - x.x * y.z) * invdet;
        out.z.x = (y.x * z.y - z.x * y.y) * invdet;
        out.z.y = (z.x * x.y - x.x * z.y) * invdet;
        out.z.z = (x.x * y.y - y.x * x.y) * invdet;
        return out;
    }
    mat3x3f mat3x3f::transposed() const {
        return {
            { x.x, y.x, z.x },
            { x.y, y.y, z.y },
            { x.z, y.z, z.z }
        };
    }
    vec3f mat3x3f::operator[](u8 axis) const {
        return (&x)[axis];
    }
    vec3f& mat3x3f::operator[](u8 axis) {
        return (&x)[axis];
    }
}