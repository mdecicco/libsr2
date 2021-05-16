#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phPolygon.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/math.h>
#include <libsr2/utilities/datTimeManager.h>
#include <math.h>

namespace sr2 {
    phInertialCS::phInertialCS() {
        Init(1.0f, 1.0f, 1.0f, 1.0f);

        field_0x30 = 5.0f;
        max_velocity = 500.0f;
        field_0x38 = 5.0f;
        field_0x34 = 5.0f;
        field_0x11c = 0;
        field_0x120 = 0.0f;
        field_0x118 = 0;
        pos_delta.y = 0.0f;
        pos_delta.z = 0.0f;
        pos_delta.x = 0.0f;
        field_0x100.y = 0.0f;
        field_0x100.z = 0.0f;
        field_0x100.x = 0.0f;
        field_0x10c.y = 0.0f;
        field_0x10c.z = 0.0f;
        field_0x10c.x = 0.0f;
        field_0x4 = 0;
        field_0x8 = 0;

        InitMatrix();
    }

    phInertialCS::~phInertialCS() {
    }

    void phInertialCS::Init(f32 _mass, f32 Ixx, f32 Iyy, f32 Izz) { 
        f32 fVar1;
        f32 fVar2;

        field_0x0 = 1.401298e-45f;
        mass = _mass;

        if (0.0f < mass) fVar1 = 1.0f / mass;
        else fVar1 = 3.402823e+38f;

        inv_mass = fVar1;
        inertia_tensor.x = Ixx;
        inertia_tensor.z = Izz;
        inertia_tensor.y = Iyy;
        fVar1 = inertia_tensor.x;
        if (0.0f < fVar1) {
            fVar1 = 1.0f / fVar1;
        }
        else {
            fVar1 = 3.402823e+38f;
        }
        fVar2 = inertia_tensor.y;
        inv_inertia_tensor.x = fVar1;
        if (0.0 < fVar2) {
            fVar2 = 1.0 / fVar2;
        }
        else {
            fVar2 = 3.402823e+38f;
        }
        fVar1 = inertia_tensor.z;
        inv_inertia_tensor.y = fVar2;
        if (0.0f < fVar1) {
            fVar1 = 1.0f / fVar1;
        }
        else {
            fVar1 = 3.402823e+38f;
        }
        inv_inertia_tensor.z = fVar1;
    }

    void phInertialCS::InitBoxMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z) {
    }

    void phInertialCS::InitCylinderMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z, char axis) {
    }

    void phInertialCS::InitHotdogMass(f32 param_1, f32 param_2, f32 param_3, char axis) {
    }

    void phInertialCS::InitSphereMass(f32 param_1, f32 param_2) {
    }

    void phInertialCS::InitFromGeometry(f32 mass, vec3f* vertices, phPolygon* polygons, i32 poly_count, mat3x4f* param_6) {
    }

    void phInertialCS::InitHotdogDensity(f32 param_1, f32 param_2, f32 param_3, char axis) {
    }

    void phInertialCS::InitMatrix() {
        math::identity(world_transform);
        Freeze();

        field_0x11c = 0;
        field_0x120 = 0.0f;
        field_0x124 = 0;
        field_0x118 = 0;
    }

    void phInertialCS::Freeze() { 
        veolocity_related.y = 0.0f;
        veolocity_related.z = 0.0f;
        veolocity_related.x = 0.0f;
        field_0x48.y = 0.0f;
        field_0x48.z = 0.0f;
        field_0x48.x = 0.0f;
        world_velocity.y = 0.0f;
        world_velocity.z = 0.0f;
        world_velocity.x = 0.0f;
        field_0x9c.y = 0.0f;
        field_0x9c.z = 0.0f;
        field_0x9c.x = 0.0f;
        ZeroForces();
    }

    void phInertialCS::ZeroForces() {

        u32 uVar1;
        u32 uVar2;
        u64 *puVar3;
        u32 uVar4;
        undefined4 uVar5;
        u64 uVar6;
        float fVar7;
        float fVar8;

        velocity_related_0.y = 0.0f;
        velocity_related_0.z = 0.0f;
        velocity_related_0.x = 0.0f;
        field_0xb4.y = 0.0f;
        field_0xb4.z = 0.0f;
        field_0xb4.x = 0.0f;
        field_0xc0.y = 0.0f;
        field_0xc0.z = 0.0f;
        field_0xc0.x = 0.0f;
        field_0xcc.y = 0.0f;
        field_0xcc.z = 0.0f;
        field_0xcc.x = 0.0f;
        field_0xd8 = 0;
        velocity_related_1.y = 0.0f;
        velocity_related_1.z = 0.0f;
        velocity_related_1.x = 0.0f;
        field_0xe8.y = 0.0f;
        field_0xe8.z = 0.0f;
        field_0xe8.x = 0.0f;

        /*
        uVar4 = (i32)&field_0x10c.y + 3;
        uVar1 = uVar4 & 7;
        uVar2 = (u32)&field_0x10c & 7;
        uVar6 = (*(long *)(uVar4 - uVar1) << (7 - uVar1) * 8 | (long)(int)&field_0xc0 & 0xffffffffffffffffU >> (uVar1 + 1) * 8) &
            -1 << (8 - uVar2) * 8 | *(ulong *)((int)&field_0x10c - uVar2) >> uVar2 * 8;
        fVar7 = field_0x10c.z;
        uVar4 = (int)&field_0x11c + 3;
        uVar1 = uVar4 & 7;
        puVar3 = (ulong *)(uVar4 - uVar1);
        *puVar3 = *puVar3 & -1 << (uVar1 + 1) * 8 | uVar6 >> (7 - uVar1) * 8;
        uVar4 = (uint)&field_0x118 & 7;
        puVar3 = (ulong *)((int)&field_0x118 - uVar4);
        *puVar3 = uVar6 << uVar4 * 8 | *puVar3 & 0xffffffffffffffffU >> (8 - uVar4) * 8;
        field_0x120 = fVar7;
        */

        field_0x118 = field_0x118 + pos_delta.x;
        fVar7 = field_0x11c + pos_delta.y;
        field_0x11c = fVar7;
        fVar8 = field_0x120 + pos_delta.z;
        field_0x120 = fVar8;
        uVar5 = 1;

        if (field_0x118 == 0.0f) {
            if (fVar7 != 0.0f) {
                field_0x124 = 1;
            } else if (fVar8 == 0.0f) {
                uVar5 = 0;
                field_0x124 = uVar5;
            }
        }

        field_0x10c.y = 0.0f;
        field_0x10c.z = 0.0f;
        field_0x10c.x = 0.0f;
        pos_delta.y = 0.0f;
        pos_delta.z = 0.0f;
        pos_delta.x = 0.0f;
        field_0x100.y = 0.0f;
        field_0x100.z = 0.0f;
        field_0x100.x = 0.0f;
        field_0x128 = 0;
        math::zero(field_0x130);
        math::zero(field_0x160);
        math::zero(field_0x190);
        field_0x1c4 = 0;
        maybe_last_time = 0.0f;
    }

    mat3x4f* phInertialCS::AddInertia(mat3x4f* param_1, f32 param_2, vec3f* param_4, vec3f* param_5) {
        mat3x4f local_a0;
        mat3x4f local_70;

        if ((mass + param_2) == 0.0f) {
            math::zero(local_a0);
            math::zero(*param_1);
        } else {
            if (mass > 0.0f) {
                local_a0.x.x = param_5->x - world_transform.w.x;
                local_a0.x.y = param_5->y - world_transform.w.y;
                local_a0.x.z = param_5->z - world_transform.w.z;
            } else {
                local_a0.x.x = param_5->x;
                local_a0.x.y = param_5->y;
                local_a0.x.z = param_5->z;
            }

            local_a0.z.x = param_2 / (mass + param_2);
            local_a0.y.y = local_a0.x.x * local_a0.z.x;
            local_a0.y.z = local_a0.x.y * local_a0.z.x;
            local_a0.z.x = local_a0.x.z * local_a0.z.x;

            if (mass > 0.0f) {
                world_transform.w.x = world_transform.w.x + local_a0.y.y;
                world_transform.w.y = world_transform.w.y + local_a0.y.z;
                world_transform.w.z = world_transform.w.z + local_a0.z.x;
            } else {
                world_transform.w.x = local_a0.y.y;
                world_transform.w.y = local_a0.y.z;
                world_transform.w.z = local_a0.z.x;
            }

            local_a0.x.x = local_a0.x.x - local_a0.y.y;
            local_a0.x.y = local_a0.x.y - local_a0.y.z;
            local_a0.x.z = local_a0.x.z - local_a0.z.x;

            Init(
                mass + param_2,
                inertia_tensor.x + param_4->x + param_2 * (local_a0.x.y * local_a0.x.y + local_a0.x.z * local_a0.x.z) + mass * (local_a0.y.z * local_a0.y.z + local_a0.z.x * local_a0.z.x),
                inertia_tensor.y + param_4->y + param_2 * (local_a0.x.x * local_a0.x.x + local_a0.x.z * local_a0.x.z) + mass * (local_a0.y.y * local_a0.y.y + local_a0.z.x * local_a0.z.x),
                inertia_tensor.z + param_4->z + param_2 * (local_a0.x.x * local_a0.x.x + local_a0.x.y * local_a0.x.y) + mass * (local_a0.y.y * local_a0.y.y + local_a0.y.z * local_a0.y.z)
            );

            f32 fVar2 = -param_2 * local_a0.x.x * local_a0.x.y - mass * local_a0.y.y * local_a0.y.z;
            f32 fVar3 = -param_2 * local_a0.x.y * local_a0.x.z - mass * local_a0.y.z * local_a0.z.x;
            f32 fVar1 = -param_2 * local_a0.x.x * local_a0.x.z - mass * local_a0.y.y * local_a0.z.x;

            if (fVar2 == 0.0f && fVar1 == 0.0f && fVar3 == 0.0f) {
                if (mass <= 0.0f) math::identity_3x3(world_transform);
                math::identity(local_70);
                math::identity_3x3(*param_1);

                param_1->w.x = -local_a0.y.y;
                param_1->w.y = -local_a0.y.z;
                param_1->w.z = -local_a0.z.x;
            } else FindPrincipalAxes(param_1, fVar2, fVar1, fVar3);
        }

        return param_1;
    }

    mat3x4f* phInertialCS::AddInertia(mat3x4f *param_1, f32 param_2, vec3f *param_4, mat3x4f *param_5) {
        mat3x4f local_140;
        mat3x4f local_110;
        mat3x4f local_e0;
        mat3x4f local_b0;

        if ((mass + param_2) == 0.0f) {
            math::zero(local_140);
            math::zero(*param_1);
        } else {
            if (mass > 0.0f) {
                local_140.x.x = param_5->w.x - world_transform.w.x;
                local_140.x.y = param_5->w.y - world_transform.w.y;
                local_140.x.z = param_5->w.z - world_transform.w.z;
            } else math::copy(local_140.x, param_5->w);

            f32 f = param_2 / (mass + param_2);
            local_140.z.x = local_140.x.z * f;
            local_140.y.y = local_140.x.x * f;
            local_140.y.z = local_140.x.y * f;

            if (mass > 0.0f) {
                world_transform.w.x += local_140.y.y;
                world_transform.w.y += local_140.y.z;
                world_transform.w.z += local_140.z.x;
            } else {
                world_transform.w.x = local_140.y.y;
                world_transform.w.y = local_140.y.z;
                world_transform.w.z = local_140.z.x;
            }

            local_140.x.x -= local_140.y.y;
            local_140.x.y -= local_140.y.z;
            local_140.x.z -= local_140.z.x;

            math::transposed(local_110, *param_5);

            local_e0.x = { param_4->x, 0.0f, 0.0f };
            local_e0.y = { 0.0f, param_4->y, 0.0f };
            local_e0.z = { 0.0f, 0.0f, param_4->z };

            math::mult(local_110, local_e0);
            math::mult(local_110, *param_5);

            Init(
                mass + param_2,
                inertia_tensor.x + local_110.x.x + param_2 * (local_140.x.y * local_140.x.y + local_140.x.z * local_140.x.z) + mass * (local_140.y.z * local_140.y.z + local_140.z.x * local_140.z.x),
                inertia_tensor.y + local_110.y.y + param_2 * (local_140.x.x * local_140.x.x + local_140.x.z * local_140.x.z) + mass * (local_140.y.y * local_140.y.y + local_140.z.x * local_140.z.x),
                inertia_tensor.z + local_110.z.z + param_2 * (local_140.x.x * local_140.x.x + local_140.x.y * local_140.x.y) + mass * (local_140.y.y * local_140.y.y + local_140.y.z * local_140.y.z)
            );

            f32 fVar3 = (local_110.x.y - param_2 * local_140.x.x * local_140.x.y) - mass * local_140.y.y * local_140.y.z;
            f32 fVar1 = (local_110.y.z - param_2 * local_140.x.y * local_140.x.z) - mass * local_140.y.z * local_140.z.x;
            f32 fVar2 = (local_110.x.z - param_2 * local_140.x.x * local_140.x.z) - mass * local_140.y.y * local_140.z.x;

            if (fVar3 == 0.0f && fVar2 == 0.0f && fVar1 == 0.0f) {
                math::identity(local_b0);
                math::identity_3x3(*param_1);

                param_1->w.x = -local_140.y.y;
                param_1->w.y = -local_140.y.z;
                param_1->w.z = -local_140.z.x;
            }
            else {
                FindPrincipalAxes(param_1, fVar3, fVar2, fVar1);
            }
        }

        return param_1;
    }

    void phInertialCS::ApplyContactForce(vec3f* param_1, vec3f* param_2, mat3x4f* param_3, vec3f* param_4) {
        vec3f local_80;
        mat3x4f local_70;

        field_0x128 = 1;
        field_0xc0.x += param_1->x;
        field_0xc0.y += param_1->y;
        field_0xc0.z += param_1->z;

        if (param_4 == nullptr) {
            param_4 = &local_80;
            local_80.x = param_2->x - world_transform.w.x;
            local_80.y = param_2->y - world_transform.w.y;
            local_80.z = param_2->z - world_transform.w.z;
        }

        field_0xd8 = 1;
        field_0xcc.x += (param_4->y * param_1->z - param_4->z * param_1->y);
        field_0xcc.y += (param_4->z * param_1->x - param_4->x * param_1->z);
        field_0xcc.z += (param_4->x * param_1->y - param_4->y * param_1->x);

        field_0x130.x.x += param_3->x.x;
        field_0x130.x.y += param_3->x.y;
        field_0x130.x.z += param_3->x.z;
        field_0x130.y.x += param_3->y.x;
        field_0x130.y.y += param_3->y.y;
        field_0x130.y.z += param_3->y.z;
        field_0x130.z.x += param_3->z.x;
        field_0x130.z.y += param_3->z.y;
        field_0x130.z.z += param_3->z.z;
        field_0x130.w.x += param_3->w.x;
        field_0x130.w.y += param_3->w.y;
        field_0x130.w.z += param_3->w.z;

        local_70.x.x =  0.0f;
        local_70.x.y = -param_4->z;
        local_70.x.z =  param_4->y;

        local_70.y.x =  param_4->z;
        local_70.y.y =  0.0f;
        local_70.y.z = -param_4->x;

        local_70.z.x = -param_4->y;
        local_70.z.y =  param_4->x;
        local_70.z.z =  0.0f;

        math::mult(local_70, *param_3);
        //mat3x4f::addEq(&field_0x160, &local_70);
        //mat3x4f::Dot3x3CrossProdTranspose(&local_70, param_4);
        //mat3x4f::addEq(&field_0x190 ,&local_70);
    }

    mat3x4f* phInertialCS::FindPrincipalAxes(mat3x4f* param_2, f32 param_3, f32 param_4, f32 param_5) {
        bool bVar1;
        i32 iVar2;
        f32 fVar3;
        f32 fVar4;
        f32 fVar5;
        mat3x4f local_e0;
        vec3f local_b0;
        mat3x4f local_a0;
        f32 local_70;
        f32 local_6c;
        f32 local_68[2];

        f32 p3sq = param_3 * param_3;
        f32 p4sq = param_4 * param_4;
        f32 p5sq = param_5 * param_5;

        iVar2 = RealCubic(
            (-inertia_tensor.x - inertia_tensor.y) - inertia_tensor.z,
            (((inertia_tensor.x * inertia_tensor.y + inertia_tensor.x * inertia_tensor.z + inertia_tensor.y * inertia_tensor.z) - p3sq) - p4sq) - p5sq,
            ((inertia_tensor.x * p5sq + inertia_tensor.y * p4sq + inertia_tensor.z * p3sq) - (param_3 + param_3) * param_4 * param_5) - inertia_tensor.x * inertia_tensor.y * inertia_tensor.z,
            0.001,
            &local_70,
            &local_6c,
            local_68
        );

        if (iVar2 == 1) {
            inertia_tensor.x = local_70;
            inertia_tensor.z = local_70;
            inertia_tensor.y = local_70;
            math::identity(local_e0);
            math::copy(*param_2, local_e0);
            return param_2;
        }
        if (iVar2 < 2) goto LAB_00289088;
        if (iVar2 == 2) {
            vec3f xAxis;
            PrincipalAxis(local_70, param_3, param_4, param_5, &xAxis);
            math::copy(local_e0.x, xAxis);

            if (xAxis.x >= 0.5773503f) {
                inertia_tensor = { local_70, local_6c, local_6c };
                local_e0.y.x = -xAxis.y;
                local_e0.y.y = 1.0f / sqrtf(local_e0.y.x * local_e0.y.x + xAxis.x * xAxis.x + 0.0);
                local_e0.y.x = local_e0.y.x * local_e0.y.y;
                local_e0.y.z = local_e0.y.y * 0.0f;
                local_e0.y.y = xAxis.x * local_e0.y.y;
                math::cross(local_e0.z, xAxis, local_e0.y);
            }
            else {
                if (xAxis.y < 0.0f) {
                    local_e0.x.x = -xAxis.x;
                    xAxis.y = -xAxis.y;
                    local_e0.x.z = -xAxis.z;
                }

                if (xAxis.y >= 0.5773503f) {
                    local_e0.y.x =  local_e0.x.x;
                    local_e0.x.y = -local_e0.x.x;
                    local_e0.y.z =  local_e0.x.z;
                    inertia_tensor.x = local_6c;
                    inertia_tensor.z = local_6c;
                    inertia_tensor.y = local_70;
                    fVar4 = 1.0f / sqrtf(xAxis.y * xAxis.y + local_e0.x.y * local_e0.x.y + 0.0);
                    fVar3 = fVar4 * 0.0f;
                    local_e0.x.y = local_e0.x.y * fVar4;
                    fVar4 = xAxis.y * fVar4;

                    local_e0.z.x = local_e0.x.y * local_e0.x.z - fVar3 * xAxis.y;
                    local_e0.z.y = fVar3 * local_e0.x.x - fVar4 * local_e0.x.z;
                    local_e0.z.z = fVar4 * xAxis.y - local_e0.x.y * local_e0.x.x;

                    local_e0.x.x = fVar4;
                    local_e0.x.z = fVar3;
                    local_e0.y.y = xAxis.y;
                } else {
                    local_e0.z.x = local_e0.x.x;
                    local_e0.z.y = xAxis.y;
                    local_e0.z.z = local_e0.x.z;

                    inertia_tensor.x = local_6c;
                    inertia_tensor.z = local_70;
                    inertia_tensor.y = local_6c;

                    if (local_e0.x.z < 0.0f) math::negate(local_e0.z);

                    local_e0.x = { local_e0.z.z, 0.0f, -local_e0.z.x };
                    math::normalize(local_e0.x);
                    math::cross(local_e0.y, local_e0.z, local_e0.x);
                }
            }
            goto LAB_00289088;
        }
        if (iVar2 != 3) goto LAB_00289088;
        PrincipalAxis(local_70, param_3, param_4, param_5, &local_e0.x);
        PrincipalAxis(local_6c, param_3, param_4, param_5, &local_e0.y);
        math::copy(local_e0.z, local_e0.x);

        fVar3 = math::dot(local_e0.x, local_e0.y);
        if (fVar3 != 0.0f) {
            local_e0.y.x -= fVar3 * local_e0.x.x;
            local_e0.y.y -= fVar3 * local_e0.x.y;
            local_e0.y.z -= fVar3 * local_e0.x.z;
            math::normalize(local_e0.y);
        }

        vec3f fVar345 = local_e0.y;
        if (local_e0.x.x >= 0.5773503f) {
            if (local_e0.y.y < 0.0) math::negate(local_e0.y);

            if (0.5773503f <= local_e0.y.y) {
                math::cross(local_e0.z, local_e0.x, local_e0.y);
                inertia_tensor.x = local_70;
                inertia_tensor.z = local_68[0];
            } else {
                math::copy(local_e0.z, local_e0.y);
                if (local_e0.y.z < 0.0f) math::negate(local_e0.z, local_e0.y);
                inertia_tensor.x = local_70;
                inertia_tensor.z = local_6c;
                math::cross(local_e0.y, local_e0.z, local_e0.x);
                local_6c = local_68[0];
            }
        } else {
            if (local_e0.y.x >= 0.5773503f) {
                bVar1 = local_e0.x.z < 0.0;
                math::copy(local_e0.x, local_e0.y);
                if (bVar1) math::negate(local_e0.z);

                if (local_e0.z.z >= 0.5773503f) {
                    math::mult(fVar345, local_e0.z, { local_e0.y.z, local_e0.y.x, local_e0.y.x });
                    math::cross(
                        local_e0.y,
                        { local_e0.z.x, local_e0.z.y, fVar345.z },
                        { fVar345.x, local_e0.y.y, local_e0.y.z }
                    );
                    local_6c = local_70;
                    inertia_tensor.x = local_6c;
                    inertia_tensor.z = local_6c;
                    inertia_tensor.y = local_68[0];
                    goto LAB_00289088;
                }

                math::copy(local_e0.y, local_e0.z);
                if (local_e0.z.y < 0.0f) math::negate(local_e0.y);

                inertia_tensor.x = local_6c;
                inertia_tensor.z = local_68[0];
                math::cross(local_e0.z, fVar345, local_e0.y);
                local_6c = local_70;
            } else {
                if (fabsf(local_e0.x.y) >= 0.5773503f) {
                    math::copy(local_e0.z, local_e0.y);
                    math::copy(local_e0.y, local_e0.x);

                    if (local_e0.x.y < 0.0f) math::negate(local_e0.y);
                    if (local_e0.y.z < 0.0f) math::negate(local_e0.z);

                    if (local_e0.z.z >= 0.5773503f) {
                        math::cross(local_e0.x, local_e0.y, local_e0.z);
                        inertia_tensor.x = local_6c;
                        inertia_tensor.y = local_70;
                        inertia_tensor.z = local_6c;
                        goto LAB_00289088;
                    }

                    math::copy(local_e0.x, local_e0.z);
                    if (local_e0.z.x < 0.0f) math::negate(local_e0.x, local_e0.z);

                    inertia_tensor.x = local_6c;
                    inertia_tensor.z = local_68[0];

                    math::cross(local_e0.z, local_e0.x, local_e0.y);
                    local_6c = local_70;
                } else {
                    if (local_e0.x.z < 0.0f) math::negate(local_e0.z);
                    if (local_e0.y.y < 0.0f) math::negate(local_e0.y);

                    if (local_e0.y.y <= 0.5773503f) goto LAB_00289088;
                    inertia_tensor.x = local_68[0];
                    inertia_tensor.z = local_70;

                    math::cross(local_e0.x, local_e0.y, local_e0.z);
                }
            }
        }
        inertia_tensor.y = local_6c;
    LAB_00289088:
        math::zero(local_e0.w);
        math::mult(local_a0, local_e0, world_transform);

        math::normalize(local_a0.x);
        math::normalize(local_a0.y);
        math::normalize(local_a0.z);
        math::copy(*param_2, local_e0);
        return param_2;
    }

    void phInertialCS::GetCMFilteredVelocity(vec3f* velocity) {
        /*
        u32 uVar1;
        u32 uVar2;
        float fVar3;
        u64 *puVar4;
        u32 uVar5;
        u64 in_v0;
        u64 uVar6;

        uVar5 = (int)&(world_velocity).y + 3;
        uVar1 = uVar5 & 7;
        uVar2 = (uint)&world_velocity & 7;
        uVar6 = (*(long *)(uVar5 - uVar1) << (7 - uVar1) * 8 | in_v0 & 0xffffffffffffffffU >> (uVar1 + 1) * 8) & -1 << (8 - uVar2) * 8 |
            *(ulong *)((int)&world_velocity - uVar2) >> uVar2 * 8;
        fVar3 = (world_velocity).z;
        uVar5 = (int)&param_1->y + 3;
        uVar1 = uVar5 & 7;
        puVar4 = (ulong *)(uVar5 - uVar1);
        *puVar4 = *puVar4 & -1 << (uVar1 + 1) * 8 | uVar6 >> (7 - uVar1) * 8;
        uVar5 = (uint)param_1 & 7;
        *(ulong *)(vec3f *)((int)param_1 - uVar5) =
            uVar6 << uVar5 * 8 | *(ulong *)(vec3f *)((int)param_1 - uVar5) & 0xffffffffffffffffU >> (8 - uVar5) * 8;
        param_1->z = fVar3;
        */

        velocity->x += g_datTimeManager.InvSeconds * field_0x118;
        velocity->y += g_datTimeManager.InvSeconds * field_0x11c;
        velocity->z += g_datTimeManager.InvSeconds * field_0x120;
    }

    void phInertialCS::GetForce(f32 param_1, vec3f* force) {
        force->x = velocity_related_0.x;
        force->y = velocity_related_0.y;
        force->z = velocity_related_0.z;

        if (param_1 > 0.0f) {
            force->x += param_1 * velocity_related_1.x;
            force->y += param_1 * velocity_related_1.y;
            force->z += param_1 * velocity_related_1.z;
        }
    }

    void phInertialCS::GetInertiaMatrix(mat3x4f* inertia) { 
        inertia->x.x = world_transform.x.x * inertia_tensor.x;
        inertia->x.y = world_transform.y.x * inertia_tensor.y;
        inertia->x.z = world_transform.z.x * inertia_tensor.z;

        inertia->y.x = world_transform.x.y * inertia_tensor.x;
        inertia->y.y = world_transform.y.y * inertia_tensor.y;
        inertia->y.z = world_transform.z.y * inertia_tensor.z;

        inertia->z.x = world_transform.x.z * inertia_tensor.x;
        inertia->z.y = world_transform.y.z * inertia_tensor.y;
        inertia->z.z = world_transform.z.z * inertia_tensor.z;

        inertia->w.y = 0.0f;
        inertia->w.z = 0.0f;
        inertia->w.x = 0.0f;

        math::mult(*inertia, world_transform);
    }

    void phInertialCS::GetInverseInertiaMatrix(mat3x4f* inertia) {       
        mat3x4f tmp, tmp1;
        tmp.x.x = world_transform.x.x;
        tmp.x.y = world_transform.x.y;
        tmp.x.z = world_transform.x.z;

        tmp.y.x = world_transform.y.x;
        tmp.y.y = world_transform.y.y;
        tmp.y.z = world_transform.y.z;

        tmp.z.x = world_transform.z.x;
        tmp.z.y = world_transform.z.y;
        tmp.z.z = world_transform.z.z;

        tmp.w.x = world_transform.w.x;
        tmp.w.y = world_transform.w.y;
        tmp.w.z = world_transform.w.z;

        tmp1.x.x = world_transform.x.x * inv_inertia_tensor.x;
        tmp1.x.y = world_transform.x.y * inv_inertia_tensor.x;
        tmp1.x.z = world_transform.x.z * inv_inertia_tensor.x;

        tmp1.y.x = world_transform.y.x * inv_inertia_tensor.y;
        tmp1.y.y = world_transform.y.y * inv_inertia_tensor.y;
        tmp1.y.z = world_transform.y.z * inv_inertia_tensor.y;

        tmp1.z.x = world_transform.z.x * inv_inertia_tensor.z;
        tmp1.z.y = world_transform.z.y * inv_inertia_tensor.z;
        tmp1.z.z = world_transform.z.z * inv_inertia_tensor.z;

        math::transpose(tmp);
        math::mult(*inertia, tmp, tmp1);
    }

    void phInertialCS::GetLocalAcceleration(vec3f* param_1, vec3f* param_2) {
        vec3f d;
        math::sub(d, *param_1, world_transform.w);

        math::cross(*param_2, field_0x9c, d);
        math::cross(*param_2, field_0x9c, *param_2);

        param_2->x += inv_mass * velocity_related_0.x;
        param_2->y += inv_mass * velocity_related_0.y;
        param_2->z += inv_mass * velocity_related_0.z;

        vec3f x0;
        math::cross(x0, field_0x48, field_0x9c);
        math::add(x0, field_0xb4);

        x0 = {
            inv_inertia_tensor.x * math::dot(x0, world_transform.x),
            inv_inertia_tensor.y * math::dot(x0, world_transform.y),
            inv_inertia_tensor.z * math::dot(x0, world_transform.z)
        };

        math::mult(x0, world_transform, x0);
        math::cross(x0, x0, d);
        math::add(*param_2, x0);
    }

    void phInertialCS::GetLocalVelocity(vec3f* param_1, vec3f* param_2) { 
        vec3f d;
        math::sub(d, *param_1, world_transform.w);
        math::copy(*param_2, d);

        vec3f x0;
        math::cross(x0, field_0x9c, d);

        math::add(*param_2, x0, world_velocity);
    }

    vec3f* phInertialCS::PrincipalAxis(f32 param_2, f32 param_3, f32 param_4, f32 param_5, vec3f* param_6) {
        f32 fVar2 = 1.0f / (param_2 - inertia_tensor.z);
        f32 fVar3 = (param_3 + param_4 * param_5 * fVar2) * (1.0f / ((param_2 - inertia_tensor.y) - param_5 * param_5 * fVar2));
        fVar2 = fVar2 * (param_4 + param_5 * fVar3);
        f32 fVar1 = 1.0f / sqrtf(fVar3 * fVar3 + 1.0f + fVar2 * fVar2);
        param_6->x = fVar1;
        param_6->y = fVar3 * fVar1;
        param_6->z = fVar2 * fVar1;
        return param_6;
    }

    mat3x4f* phInertialCS::Rejuvinate() {
        return nullptr;
    }

    void phInertialCS::SetVelocity(vec3f* velocity) {
    }

    void phInertialCS::TetrahedronAngInertia(vec3f* verts, f32* param_2, f32* param_3, f32* param_4, f32* param_5, f32* param_6, f32* param_7) {
    }
};