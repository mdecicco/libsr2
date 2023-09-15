#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>

#include <math.h>

namespace sr2 {
    namespace math {
        void identity(mat3x4f& m) {
            m.x = { 1.0f, 0.0f, 0.0f };
            m.y = { 0.0f, 1.0f, 0.0f };
            m.z = { 0.0f, 0.0f, 1.0f };
            m.w = { 0.0f, 0.0f, 0.0f };
        }

        void identity_3x3(mat3x4f& m) {
            m.x = { 1.0f, 0.0f, 0.0f };
            m.y = { 0.0f, 1.0f, 0.0f };
            m.z = { 0.0f, 0.0f, 1.0f };
        }
        
        void zero(mat3x4f& m) { 
            m.x = { 0.0f, 0.0f, 0.0f };
            m.y = { 0.0f, 0.0f, 0.0f };
            m.z = { 0.0f, 0.0f, 0.0f };
            m.w = { 0.0f, 0.0f, 0.0f };
        }

        void copy(mat3x4f& to, const mat3x4f& from) {
            to.x = { from.x.x, from.x.y, from.x.z };
            to.y = { from.y.x, from.y.y, from.y.z };
            to.z = { from.z.x, from.z.y, from.z.z };
            to.w = { from.w.x, from.w.y, from.w.z };
        }

        void transpose(mat3x4f& m) {
            mat3x4f t;
            copy(t, m);
            m.x = { t.x.x, t.y.x, t.z.x };
            m.y = { t.x.y, t.y.y, t.z.y };
            m.z = { t.x.z, t.y.z, t.z.z };
        }

        void transposed(mat3x4f& out, const mat3x4f& in) {
            out.x = { in.x.x, in.y.x, in.z.x };
            out.y = { in.x.y, in.y.y, in.z.y };
            out.z = { in.x.z, in.y.z, in.z.z };
        }

        void mult(mat3x4f& lhs, const mat3x4f& rhs) {
            f32 xx = lhs.x.x; f32 xy = lhs.x.y; f32 xz = lhs.x.z;
            f32 yx = lhs.y.x; f32 yy = lhs.y.y; f32 yz = lhs.y.z;
            f32 zx = lhs.z.x; f32 zy = lhs.z.y; f32 zz = lhs.z.z;

            lhs.x.x = xx * rhs.x.x + xy * rhs.y.x + xz * rhs.z.x;
            lhs.x.y = xx * rhs.x.y + xy * rhs.y.y + xz * rhs.z.y;
            lhs.x.z = xx * rhs.x.z + xy * rhs.y.z + xz * rhs.z.z;
            lhs.y.x = yx * rhs.x.x + yy * rhs.y.x + yz * rhs.z.x;
            lhs.y.y = yx * rhs.x.y + yy * rhs.y.y + yz * rhs.z.y;
            lhs.y.z = yx * rhs.x.z + yy * rhs.y.z + yz * rhs.z.z;
            lhs.z.x = zx * rhs.x.x + zy * rhs.y.x + zz * rhs.z.x;
            lhs.z.y = zx * rhs.x.y + zy * rhs.y.y + zz * rhs.z.y;
            lhs.z.z = zx * rhs.x.z + zy * rhs.y.z + zz * rhs.z.z;
        }

        void mult(mat3x4f& out, const mat3x4f& lhs, const mat3x4f& rhs) { 
            f32 xx = lhs.x.x; f32 xy = lhs.x.y; f32 xz = lhs.x.z;
            f32 yx = lhs.y.x; f32 yy = lhs.y.y; f32 yz = lhs.y.z;
            f32 zx = lhs.z.x; f32 zy = lhs.z.y; f32 zz = lhs.z.z;
            out.x.x = xx * rhs.x.x + xy * rhs.y.x + xz * rhs.z.x;
            out.x.y = xx * rhs.x.y + xy * rhs.y.y + xz * rhs.z.y;
            out.x.z = xx * rhs.x.z + xy * rhs.y.z + xz * rhs.z.z;
            out.y.x = yx * rhs.x.x + yy * rhs.y.x + yz * rhs.z.x;
            out.y.y = yx * rhs.x.y + yy * rhs.y.y + yz * rhs.z.y;
            out.y.z = yx * rhs.x.z + yy * rhs.y.z + yz * rhs.z.z;
            out.z.x = zx * rhs.x.x + zy * rhs.y.x + zz * rhs.z.x;
            out.z.y = zx * rhs.x.y + zy * rhs.y.y + zz * rhs.z.y;
            out.z.z = zx * rhs.x.z + zy * rhs.y.z + zz * rhs.z.z;
        }

        void mult_with_translation(mat3x4f& lhs, const mat3x4f& rhs) {
            f32 xx1 = lhs.x.x; f32 xy1 = lhs.x.y; f32 xz1 = lhs.x.z;
            f32 yx1 = lhs.y.x; f32 yy1 = lhs.y.y; f32 yz1 = lhs.y.z;
            f32 zx1 = lhs.z.x; f32 zy1 = lhs.z.y; f32 zz1 = lhs.z.z;
            f32 wx1 = lhs.w.x; f32 wy1 = lhs.w.y; f32 wz1 = lhs.w.z;

            f32 xx2 = rhs.x.x; f32 xy2 = rhs.x.y; f32 xz2 = rhs.x.z;
            f32 yx2 = rhs.y.x; f32 yy2 = rhs.y.y; f32 yz2 = rhs.y.z;
            f32 zx2 = rhs.z.x; f32 zy2 = rhs.z.y; f32 zz2 = rhs.z.z;
            f32 wx2 = rhs.w.x; f32 wy2 = rhs.w.y; f32 wz2 = rhs.w.z;

            lhs.x.x = xx1 * xx2 + xy1 * yx2 + xz1 * zx2;
            lhs.x.z = xx1 * xz2 + xy1 * yz2 + xz1 * zz2;
            lhs.y.x = yx1 * xx2 + yy1 * yx2 + yz1 * zx2;
            lhs.x.y = xx1 * xy2 + xy1 * yy2 + xz1 * zy2;
            lhs.y.z = yx1 * xz2 + yy1 * yz2 + yz1 * zz2;
            lhs.y.y = yx1 * xy2 + yy1 * yy2 + yz1 * zy2;
            lhs.z.x = zx1 * xx2 + zy1 * yx2 + zz1 * zx2;
            lhs.z.y = zx1 * xy2 + zy1 * yy2 + zz1 * zy2;
            lhs.z.z = zx1 * xz2 + zy1 * yz2 + zz1 * zz2;
            lhs.w.x = wx1 * xx2 + wy1 * yx2 + wz1 * zx2 + wx2;
            lhs.w.y = wx1 * xy2 + wy1 * yy2 + wz1 * zy2 + wy2;
            lhs.w.z = wx1 * xz2 + wy1 * yz2 + wz1 * zz2 + wz2;
        }

        void mult_with_translation(mat3x4f& out, const mat3x4f& lhs, const mat3x4f& rhs) {
            f32 xx1 = lhs.x.x; f32 xy1 = lhs.x.y; f32 xz1 = lhs.x.z;
            f32 yx1 = lhs.y.x; f32 yy1 = lhs.y.y; f32 yz1 = lhs.y.z;
            f32 zx1 = lhs.z.x; f32 zy1 = lhs.z.y; f32 zz1 = lhs.z.z;
            f32 wx1 = lhs.w.x; f32 wy1 = lhs.w.y; f32 wz1 = lhs.w.z;

            f32 xx2 = rhs.x.x; f32 xy2 = rhs.x.y; f32 xz2 = rhs.x.z;
            f32 yx2 = rhs.y.x; f32 yy2 = rhs.y.y; f32 yz2 = rhs.y.z;
            f32 zx2 = rhs.z.x; f32 zy2 = rhs.z.y; f32 zz2 = rhs.z.z;
            f32 wx2 = rhs.w.x; f32 wy2 = rhs.w.y; f32 wz2 = rhs.w.z;

            mat3x4f ret;
            ret.x.x = xx1 * xx2 + xy1 * yx2 + xz1 * zx2;
            ret.x.z = xx1 * xz2 + xy1 * yz2 + xz1 * zz2;
            ret.y.x = yx1 * xx2 + yy1 * yx2 + yz1 * zx2;
            ret.x.y = xx1 * xy2 + xy1 * yy2 + xz1 * zy2;
            ret.y.z = yx1 * xz2 + yy1 * yz2 + yz1 * zz2;
            ret.y.y = yx1 * xy2 + yy1 * yy2 + yz1 * zy2;
            ret.z.x = zx1 * xx2 + zy1 * yx2 + zz1 * zx2;
            ret.z.y = zx1 * xy2 + zy1 * yy2 + zz1 * zy2;
            ret.z.z = zx1 * xz2 + zy1 * yz2 + zz1 * zz2;
            ret.w.x = wx1 * xx2 + wy1 * yx2 + wz1 * zx2 + wx2;
            ret.w.y = wx1 * xy2 + wy1 * yy2 + wz1 * zy2 + wy2;
            ret.w.z = wx1 * xz2 + wy1 * yz2 + wz1 * zz2 + wz2;

            out = ret;
        }

        void mult_inverse_with_translation(mat3x4f& lhs, const mat3x4f& rhs) {
            f32 xx = lhs.x.x; f32 xy = lhs.x.y; f32 xz = lhs.x.z;
            f32 yx = lhs.y.x; f32 yy = lhs.y.y; f32 yz = lhs.y.z;
            f32 zx = lhs.z.x; f32 zy = lhs.z.y; f32 zz = lhs.z.z;

            lhs.x.x = lhs.x.x * rhs.x.x + lhs.x.y * rhs.x.y + lhs.x.z * rhs.x.z;
            lhs.x.y = lhs.x.x * rhs.y.x + lhs.x.y * rhs.y.y + lhs.x.z * rhs.y.z;
            lhs.x.z = lhs.x.x * rhs.z.x + lhs.x.y * rhs.z.y + lhs.x.z * rhs.z.z;

            lhs.y.x = lhs.y.x * rhs.x.x + lhs.y.y * rhs.x.y + lhs.y.z * rhs.x.z;
            lhs.y.y = lhs.y.x * rhs.y.x + lhs.y.y * rhs.y.y + lhs.y.z * rhs.y.z;
            lhs.y.z = lhs.y.x * rhs.z.x + lhs.y.y * rhs.z.y + lhs.y.z * rhs.z.z;

            lhs.z.x = lhs.z.x * rhs.x.x + lhs.z.y * rhs.x.y + lhs.z.z * rhs.x.z;
            lhs.z.y = lhs.z.x * rhs.y.x + lhs.z.y * rhs.y.y + lhs.z.z * rhs.y.z;
            lhs.z.z = lhs.z.x * rhs.z.x + lhs.z.y * rhs.z.y + lhs.z.z * rhs.z.z;

            lhs.w.x -= rhs.w.x;
            lhs.w.y -= rhs.w.y;
            lhs.w.z -= rhs.w.z;

            lhs.w.x = lhs.w.x * rhs.x.x + lhs.w.y * rhs.x.y + lhs.w.z * rhs.x.z;
            lhs.w.y = lhs.w.x * rhs.y.x + lhs.w.y * rhs.y.y + lhs.w.z * rhs.y.z;
            lhs.w.z = lhs.w.x * rhs.z.x + lhs.w.y * rhs.z.y + lhs.w.z * rhs.z.z;
        }

        void mult(vec3f& out, const mat3x4f& lhs, const vec3f& rhs) {
            out.x = lhs.x.x * rhs.x + rhs.y * lhs.y.x + rhs.z * lhs.z.x;
            out.y = lhs.x.y * rhs.x + rhs.y * lhs.y.y + rhs.z * lhs.z.y;
            out.z = lhs.x.z * rhs.x + rhs.y * lhs.y.z + rhs.z * lhs.z.z;
        }

        void mult_transposed(vec3f& out, const mat3x4f& lhs, const vec3f& rhs) {
            out.x = lhs.x.x * rhs.x + rhs.y * lhs.x.y + rhs.z * lhs.x.z;
            out.y = lhs.y.x * rhs.x + rhs.y * lhs.y.y + rhs.z * lhs.y.z;
            out.z = lhs.z.x * rhs.x + rhs.y * lhs.z.y + rhs.z * lhs.z.z;
        }

        void mult_transposed(mat3x4f& out, const mat3x4f& lhs, const mat3x4f& rhs) { 
            out.x.x = lhs.x.x * rhs.x.x + lhs.x.y * rhs.x.y + lhs.x.z * rhs.x.z;
            out.x.y = lhs.x.x * rhs.y.x + lhs.x.y * rhs.y.y + lhs.x.z * rhs.y.z;
            out.x.z = lhs.x.x * rhs.z.x + lhs.x.y * rhs.z.y + lhs.x.z * rhs.z.z;
            out.y.x = lhs.y.x * rhs.x.x + lhs.y.y * rhs.x.y + lhs.y.z * rhs.x.z;
            out.y.y = lhs.y.x * rhs.y.x + lhs.y.y * rhs.y.y + lhs.y.z * rhs.y.z;
            out.y.z = lhs.y.x * rhs.z.x + lhs.y.y * rhs.z.y + lhs.y.z * rhs.z.z;
            out.z.x = lhs.z.x * rhs.x.x + lhs.z.y * rhs.x.y + lhs.z.z * rhs.x.z;
            out.z.y = lhs.z.x * rhs.y.x + lhs.z.y * rhs.y.y + lhs.z.z * rhs.y.z;
            out.z.z = lhs.z.x * rhs.z.x + lhs.z.y * rhs.z.y + lhs.z.z * rhs.z.z;
        }

        void rotation_x(mat3x4f& out, f32 angle) {
            f32 cosTheta = cosf(angle);
            f32 sinTheta = sinf(angle);

            out.x = { 1.0f, 0.0f, 0.0f };
            out.y = { 0.0f, cosTheta, sinTheta };
            out.z = { 0.0f, -sinTheta, cosTheta };
        }

        void rotation_y(mat3x4f& out, f32 angle) {
            f32 cosTheta = cosf(angle);
            f32 sinTheta = sinf(angle);

            out.x = { cosTheta, 0.0f, -sinTheta };
            out.y = { 0.0f, 1.0f, 0.0f };
            out.z = { sinTheta, 0.0f, cosTheta };
        }

        void rotation_z(mat3x4f& out, f32 angle) {
            f32 cosTheta = cosf(angle);
            f32 sinTheta = sinf(angle);

            out.x = { cosTheta, sinTheta, 0.0f };
            out.y = { -sinTheta, cosTheta, 0.0f };
            out.z = { 0.0f, 0.0f, 1.0f };
        }

        void rotation_axis_angle(mat3x4f& out, const vec3f& axis, f32 angle) {
            f32 cosTheta = cosf(angle);
            f32 sinTheta = sinf(angle);
            f32 invCosTheta = 1.0f - cosTheta;

            out.x.x = invCosTheta * axis.x * axis.x + cosTheta;
            out.y.y = invCosTheta * axis.y * axis.y + cosTheta;
            out.z.z = invCosTheta * axis.z * axis.z + cosTheta;
            out.x.y = invCosTheta * axis.x * axis.y + sinTheta * axis.z;
            out.y.x = invCosTheta * axis.x * axis.y - sinTheta * axis.z;
            out.x.z = invCosTheta * axis.x * axis.z - sinTheta * axis.y;
            out.z.x = invCosTheta * axis.x * axis.z + sinTheta * axis.y;
            out.y.z = invCosTheta * axis.y * axis.z + sinTheta * axis.x;
            out.z.y = invCosTheta * axis.y * axis.z - sinTheta * axis.x;
        }

        void make_rotation(mat3x4f& out, const vec3f& axis, f32 angle) {
            if (angle == 0.0f) {
                identity_3x3(out);
                return;
            }

            if (axis.x == 0.0f) {
                if (axis.y == 0.0f) {
                    if (axis.z <= 0.0f) {
                        rotation_z(out, -angle);
                        return;
                    }

                    rotation_z(out, angle);
                    return;
                }

                if (axis.z == 0.0) {
                    if (axis.y <= 0.0) {
                        rotation_y(out, -angle);
                        return;
                    }

                    rotation_y(out, angle);
                    return;
                }
            } else {
                if ((axis.y == 0.0) && (axis.z == 0.0)) {
                    if (axis.x <= 0.0) {
                        rotation_x(out, -angle);
                        return;
                    }

                    rotation_x(out, angle);
                    return;
                }
            }

            vec3f normalizedAxis = axis;
            normalize(normalizedAxis);
            rotation_axis_angle(out, normalizedAxis, angle);
        }

        void from_eulers_zxy(mat3x4f& out, const vec3f& eulers) {
            vec3f cosTheta = { 0.0f, 0.0f, 0.0f };
            vec3f sinTheta = { 0.0f, 0.0f, 0.0f };

            if (eulers.x == 0.0) {
                sinTheta.x = 0.0;
                cosTheta.x = 1.0;
            } else {
                sinTheta.x = sinf(eulers.x);
                cosTheta.x = cosf(eulers.x);
            }

            if (eulers.y == 0.0) {
                sinTheta.y = 0.0;
                cosTheta.y = 1.0;
            }
            else {
                sinTheta.y = sinf(eulers.y);
                cosTheta.y = cosf(eulers.y);
            }

            if (eulers.z == 0.0) {
                sinTheta.z = 0.0;
                cosTheta.z = 1.0;
            }
            else {
                sinTheta.z = sinf(eulers.z);
                cosTheta.z = cosf(eulers.z);
            }
            out.x.x = cosTheta.z * cosTheta.y + sinTheta.z * sinTheta.x * sinTheta.y;
            out.x.z = -cosTheta.z * sinTheta.y + sinTheta.z * sinTheta.x * cosTheta.y;
            out.y.x = -sinTheta.z * cosTheta.y + cosTheta.z * sinTheta.x * sinTheta.y;
            out.x.y = sinTheta.z * cosTheta.x;
            out.y.z = sinTheta.z * sinTheta.y + cosTheta.z * sinTheta.x * cosTheta.y;
            out.y.y = cosTheta.z * cosTheta.x;
            out.z.x = cosTheta.x * sinTheta.y;
            out.z.y = -sinTheta.x;
            out.z.z = cosTheta.x * cosTheta.y;
        }

        void from_eulers_xzy(mat3x4f& out, const vec3f& eulers) {
            f32 sinX, cosX, sinY, cosY, sinZ, cosZ;
            
            if (eulers.x == 0.0f) {
                sinX = 0.0f;
                cosX = 1.0f;
            } else {
                sinX = sinf(eulers.x);
                cosX = cosf(eulers.x);
            }
            if (eulers.y == 0.0f) {
                sinY = 0.0f;
                cosY = 1.0f;
            } else {
                sinY = sinf(eulers.y);
                cosY = cosf(eulers.y);
            }
            if (eulers.z == 0.0f) {
                sinZ = 0.0f;
                cosZ = 1.0f;
            } else {
                sinZ = sinf(eulers.z);
                cosZ = cosf(eulers.z);
            }

            out.x.y = sinZ;
            out.x.z = -cosZ * sinY;
            out.y.x = -cosX * sinZ * cosY + sinX * sinY;
            out.x.x = cosZ * cosY;
            out.y.z = cosX * sinZ * sinY + sinX * cosY;
            out.y.y = cosX * cosZ;
            out.z.x = sinX * sinZ * cosY + cosX * sinY;
            out.z.y = -sinX * cosZ;
            out.z.z = -sinX * sinZ * sinY + cosX * cosY;
        }

        void from_quaternion(mat3x4f& out, const quat& q) {
            f32 ax = q.axis.x * 1.414214f;
            f32 ay = q.axis.y * 1.414214f;
            f32 az = q.axis.z * 1.414214f;
            f32 ang = q.angle * 1.414214f;
            out.y.x = ax * ay - az * ang;
            out.z.x = ax * az + ay * ang;
            out.z.y = ay * az - ax * ang;
            out.z.z = 1.0f - (ay * ay + ax * ax);
            out.x.x = 1.0f - (ay * ay + az * az);
            out.y.y = 1.0f - (az * az + ax * ax);
            out.x.y = ax * ay + az * ang;
            out.x.z = ax * az - ay * ang;
            out.y.z = ay * az + ax * ang;
        }

        void from_position_and_length_encoded(mat3x4f& out, const vec3f& pos, const vec3f& encoded) {
            identity(out);
            
            f32 magSq = math::magnitudeSq(encoded);
            vec3f raxis = encoded;

            if (magSq > 0.0f) {
                f32 mag = sqrtf(magSq);
                raxis.x /= mag;
                raxis.y /= mag;
                raxis.z /= mag;
                make_rotation(out, raxis, mag);
            }

            out.w = pos;
        }

        void rotate(mat3x4f& out, const vec3f& axis, f32 angle) {
            mat3x4f rot;
            make_rotation(rot, axis, angle);
            mult(out, rot);
        }

        void dot3x3CrossProdMtx(mat3x4f& out, const vec3f& unk) {
            math::cross(out.x, out.x, unk);
            math::cross(out.y, out.y, unk);
            math::cross(out.z, out.z, unk);
        }
    }

    vec3f mat3x4f::operator*(const vec3f& rhs) const {
        return {
            x.x * rhs.x + rhs.y * y.x + rhs.z * z.x + w.x,
            x.y * rhs.x + rhs.y * y.y + rhs.z * z.y + w.y,
            x.z * rhs.x + rhs.y * y.z + rhs.z * z.z + w.z
        };
    }

    mat3x4f mat3x4f::operator*(const mat3x4f& rhs) const {
        mat3x4f out;
        math::mult(out, *this, rhs);
        return out;
    }

    mat3x4f mat3x4f::transposed() const {
        return {
            { x.x, y.x, z.x },
            { x.y, y.y, z.y },
            { x.z, y.z, z.z },
            { w.x, w.y, w.z }
        };
    }

    vec3f mat3x4f::operator[](u8 axis) const {
        return (&x)[axis];
    }

    vec3f& mat3x4f::operator[](u8 axis) {
        return (&x)[axis];
    }
};