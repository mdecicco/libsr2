#include <libsr2/math/mat3x4f.h>

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

        void mult(vec3f& out, const mat3x4f& lhs, const vec3f& rhs) {
            vec3f o;
            o.x = lhs.x.x * rhs.x + rhs.y * lhs.y.x + rhs.z * lhs.z.x;
            o.y = lhs.x.y * rhs.x + rhs.y * lhs.y.y + rhs.z * lhs.z.y;
            o.z = lhs.x.z * rhs.x + rhs.y * lhs.y.z + rhs.z * lhs.z.z;
        }
    }
};