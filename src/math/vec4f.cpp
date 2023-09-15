#include <libsr2/math/vec4f.h>

namespace sr2 {
    vec4f::vec4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    vec4f::vec4f(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w) {}
	vec4f::vec4f(const vec3f& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(0.0f) {}
    
    vec4f vec4f::operator-() const {
        return { -x, -y, -z, -w };
    }
    vec4f vec4f::operator-(const vec4f& rhs) const {
        return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
    }
    vec4f vec4f::operator+(const vec4f& rhs) const {
        return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
    }
    vec4f vec4f::operator*(const vec4f& rhs) const {
        return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
    }
    vec4f vec4f::operator/(const vec4f& rhs) const {
        return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w };
    }
    vec4f vec4f::operator-(f32 rhs) const {
        return { x - rhs, y - rhs, z - rhs, w - rhs };
    }
    vec4f vec4f::operator+(f32 rhs) const {
        return { x + rhs, y + rhs, z + rhs, w + rhs };
    }
    vec4f vec4f::operator*(f32 rhs) const {
        return { x * rhs, y * rhs, z * rhs, w * rhs };
    }
    vec4f vec4f::operator/(f32 rhs) const {
        return { x / rhs, y / rhs, z / rhs, w / rhs };
    }
    vec4f vec4f::operator-=(const vec4f& rhs) {
        return { x -= rhs.x, y -= rhs.y, z -= rhs.z, w -= rhs.w };
    }
    vec4f vec4f::operator+=(const vec4f& rhs) {
        return { x += rhs.x, y += rhs.y, z += rhs.z, w += rhs.w };
    }
    vec4f vec4f::operator*=(const vec4f& rhs) {
        return { x *= rhs.x, y *= rhs.y, z *= rhs.z, w *= rhs.w };
    }
    vec4f vec4f::operator/=(const vec4f& rhs) {
        return { x /= rhs.x, y /= rhs.y, z /= rhs.z, w /= rhs.w };
    }
    vec4f vec4f::operator-=(f32 rhs) {
        return { x -= rhs, y -= rhs, z -= rhs, w -= rhs };
    }
    vec4f vec4f::operator+=(f32 rhs) {
        return { x += rhs, y += rhs, z += rhs, w += rhs };
    }
    vec4f vec4f::operator*=(f32 rhs) {
        return { x *= rhs, y *= rhs, z *= rhs, w *= rhs };
    }
    vec4f vec4f::operator/=(f32 rhs) {
        return { x /= rhs, y /= rhs, z /= rhs, w /= rhs };
    }
    f32 vec4f::operator[](u8 axis) const {
        return (&x)[axis];
    }
    f32& vec4f::operator[](u8 axis) {
        return (&x)[axis];
    }
};