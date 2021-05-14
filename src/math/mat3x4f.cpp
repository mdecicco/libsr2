#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    void load_identity(mat3x4f& m) {
        m.x = { 1.0f, 0.0f, 0.0f };
        m.y = { 0.0f, 1.0f, 0.0f };
        m.z = { 0.0f, 0.0f, 1.0f };
        m.w = { 0.0f, 0.0f, 0.0f };
    }
};