#include <libsr2/math/math.h>
#include <math.h>
#include <stdlib.h>

namespace sr2 {
    namespace math {
        f32 RealCubic(f32 a, f32 b, f32 c, f32 d, f32* result_0, f32* result_1, f32* result_2) {
            undefined4 uVar1;
            double __x;
            float fVar2;
            float fVar3;
            float fVar4;
            float fVar5;
            float fVar6;

            fVar2 = 0.3333333f;
            fVar3 = b * 0.3333333f - a * a * 0.1111111f;
            fVar4 = (c * -0.5 + b * 0.1666667f * a) - a * a * 0.037037f * a;
            fVar6 = fVar4 * fVar4;
            fVar3 = fVar3 * fVar3 * fVar3 + fVar6;
            if (fVar6 * d < fVar3) {
                fVar2 = CubeRoot(fVar4 + sqrtf(fVar3));
                fVar3 = CubeRoot(fVar4 - sqrtf(fVar3));
                uVar1 = 1;
                *result_0 = a * -0.3333333f + fVar2 + fVar3;
            } else {
                fVar5 = a * -0.3333333f;
                if (-(fVar6 * d) <= fVar3) {
                    uVar1 = 1;
                    if (fVar4 == 0.0f) {
                        *result_0 = fVar5;
                    } else {
                        fVar2 = CubeRoot(fVar4);
                        uVar1 = 2;
                        *result_0 = fVar5 + fVar2 + fVar2;
                        *result_1 = fVar5 - fVar2;
                    }
                }
                else {
                    if (fVar4 == 0.0f) {
                        *result_0 = fVar5;
                        fVar2 = CubeRoot(sqrtf(-fVar3));
                        uVar1 = 3;
                        *result_1 = fVar5 - fVar2 * 1.732051f;
                        *result_2 = fVar5 + fVar2 * 1.732051f;
                    } else {
                        fVar4 = atan2f(sqrtf(-fVar3), fVar4);
                        fVar4 = -sinf(fVar4 * fVar2);
                        fVar2 = fVar4;
                        cosf(__x);
                        fVar3 = CubeRoot(sqrtf(fVar6 - fVar3));
                        fVar2 = fVar2 * fVar3;
                        uVar1 = 3;
                        fVar4 = fVar4 * fVar3 * 1.732051f;
                        *result_0 = fVar5 + fVar2 + fVar2;
                        *result_1 = (fVar5 - fVar2) - fVar4;
                        *result_2 = (fVar5 - fVar2) + fVar4;
                    }
                }
            }

            return uVar1;
        }

        f32 CubeRoot(f32 x) {
            if (x > 0.0f) return powf(x, 0.3333333f);
            else if (x < 0.0f) return -powf(-x, 0.3333333f);
            return 0.0f;
        }

        f32 randf() {
            return f32(rand()) * 3.051758e-05f;
        }
    };
};