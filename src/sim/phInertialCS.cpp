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

        max_velocity = 500.0;                           
        max_ang_velocity = { 5.0f, 5.0f, 5.0f };

        math::zero(last_push);
        math::zero(push);
        math::zero(turn);
        math::zero(applied_push);

        next = nullptr;
        last = nullptr;

        Zero();
    }

    phInertialCS::~phInertialCS() {
    }

    void phInertialCS::Init(f32 _mass, f32 Ixx, f32 Iyy, f32 Izz) { 
        f32 fVar1;
        f32 fVar2;

        active = true;
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
        Init(
            mass,
            (mass * (inertia_box_y * inertia_box_y + inertia_box_z * inertia_box_z)) / 12.0f,
            (mass * (inertia_box_x * inertia_box_x + inertia_box_z * inertia_box_z)) / 12.0f,
            (mass * (inertia_box_x * inertia_box_x + inertia_box_y * inertia_box_y)) / 12.0f
        );
    }

    void phInertialCS::InitBoxDensity(f32 density, f32 width, f32 height, f32 length) {
        InitBoxMass(density * width * length * height * 1000.0f, width, height, length);
    }

    void phInertialCS::InitCylinderMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z, char axis) {
        f32 fVar2;
        f32 fVar3;
        f32 fVar4;

        fVar4 = mass * inertia_box_x * inertia_box_x * 0.5f;
        fVar2 = fVar4 * 0.5f + (mass * inertia_box_y * inertia_box_y) / 12.0f;

        if (axis == 'y') fVar3 = fVar2;
        else {
            if ('y' < axis) {
                if (axis != 'z') return;
                Init(mass, fVar2, fVar2, fVar4);
                return;
            }

            if (axis != 'x') return;
            fVar3 = fVar4;
            fVar4 = fVar2;
        }

        Init(mass, fVar3, fVar4, fVar2);
    }

    void phInertialCS::InitCylinderDensity(f32 density, f32 param_2, f32 param_3, f32 param_4, char axis) {
        InitCylinderMass(param_2 * 3.141593f * param_2 * param_3 * density * 1000.0f, param_2, param_3, param_4, axis);
    }

    void phInertialCS::InitHotdogMass(f32 mass, f32 param_2, f32 param_3, char axis) {                             
        f32 fVar2;
        f32 fVar3;
        f32 fVar4;

        fVar2 = param_2 * param_2;
        fVar3 = 1.0f / (param_3 + param_2 * 1.333333f);
        fVar4 = mass * 0.5f * fVar2 * (param_3 + param_2 * 1.066667f) * fVar3;
        fVar3 = mass * 0.3333333f * (param_3 * 0.25f * param_3 * param_3 + param_2 * param_3 * param_3 + param_3 * 2.25f * fVar2 + param_2 * 1.6f * fVar2) * fVar3;

        if (axis == 'y') fVar2 = fVar3;
        else {
            if ('y' < axis) {
                if (axis == 'z') Init(mass, fVar3, fVar3, fVar4);
                return;
            }
            if (axis != 'x') return;
            fVar2 = fVar4;
            fVar4 = fVar3;
        }

        Init(mass, fVar2, fVar4, fVar3);
    }

    void phInertialCS::InitHotdogDensity(f32 param_1, f32 param_2, f32 param_3, char axis) {
        InitHotdogMass(
            (param_2 * 1.333333f + param_3) * 3.141593f * param_2 * param_2 * param_1 * 1000.0f,
            param_2,
            param_3,
            axis
        );
    }

    void phInertialCS::InitSphereMass(f32 param_1, f32 param_2) {
        f32 I = param_1 * 0.4f * param_2 * param_2;
        Init(param_1, I, I, I);
    }

    void phInertialCS::InitSphereDensity(f32 density, f32 param_2) {
        InitSphereMass(param_2 * 4.18878f * param_2 * param_2 * density * 1000.0f, param_2);
    }

    void phInertialCS::InitFromGeometry(f32 mass, vec3f* vertices, phPolygon* polygons, i32 poly_count, mat3x4f* param_6) {
        f32 fVar5 = 0.0f;
        f32 fVar2 = 0.0f;
        f32 fVar10 = 0.0f;
        f32 fVar7 = 0.0f;
        f32 fVar4 = 0.0f;
        f32 fVar8 = 0.0f;
        f32 fVar6 = 0.0f;

        if (poly_count > 0) {
            vec3f tri[3];
            for (u32 i = 0;i < poly_count;i++) {
                f32 to0;
                f32 to1;
                f32 to2;
                f32 to3;
                f32 to4;
                f32 to5;

                math::copy(tri[0], vertices[polygons[i].indices[0]]);
                math::copy(tri[1], vertices[polygons[i].indices[1]]);
                math::copy(tri[2], vertices[polygons[i].indices[2]]);

                f32 fVar3 = math::dot(polygons[i].field_0x0, tri[0]);
                TetrahedronAngInertia(tri, &to0, &to1, &to2, &to3, &to4, &to5);

                fVar10 += polygons[i].field_0xc * fVar3;
                fVar5 += to0 * fVar3;
                fVar4 += to1 * fVar3;
                fVar2 += to2 * fVar3;
                fVar8 += to3 * fVar3;
                fVar7 += to4 * fVar3;
                fVar6 += to5 * fVar3;

                if (polygons[i].indices[3]) {
                    math::copy(tri[0], vertices[polygons[i].indices[0]]);
                    math::copy(tri[1], vertices[polygons[i].indices[2]]);
                    math::copy(tri[2], vertices[polygons[i].indices[3]]);
                    TetrahedronAngInertia(tri, &to0, &to1, &to2, &to3, &to4, &to5);

                    fVar5 += to0 * fVar3;
                    fVar4 += to1 * fVar3;
                    fVar2 += to2 * fVar3;
                    fVar8 += to3 * fVar3;
                    fVar7 += to4 * fVar3;
                    fVar6 += to5 * fVar3;
                }
            }
        }

        f32 fVar9 = mass * 1000.0f;
        f32 Ixx = fabsf(fVar5) * fVar9;
        f32 Iyy = fabsf(fVar4) * fVar9;
        f32 Izz = fabsf(fVar2) * fVar9;

        Init(fabsf(fVar10) * fVar9 * 0.3333333f, Ixx, Iyy, Izz);

        if (param_6) {
            fVar6 *= fVar9;
            fVar8 *= fVar9;
            fVar7 *= fVar9;
            if (Iyy <= Ixx) Ixx = Iyy;
            if (Izz < Ixx) Ixx = Izz;

            fVar2 = fVar8;
            if (fVar8 <= fVar7) fVar2 = fVar7;
            if (fVar2 < fVar6) fVar2 = fVar6;

            if (Ixx < fVar2 * 0.05f) FindPrincipalAxes(param_6, fVar8, fVar7, fVar6);
        }
    }

    void phInertialCS::Zero() {
        math::identity(world_transform);
        Freeze();

        math::zero(last_push);
        pushed_last_frame = false;
    }

    void phInertialCS::Freeze() {
        math::zero(momentum);
        math::zero(angular_momentum);
        math::zero(world_velocity);
        math::zero(angular_velocity);
        ZeroForces();
    }

    void phInertialCS::ZeroForces() {
        math::zero(force);
        math::zero(torque);
        math::zero(oversample_force);
        math::zero(field_0xcc);
        math::zero(impulse);
        math::zero(angular_impulse);
        math::zero(field_0x130);
        math::zero(field_0x160);
        math::zero(field_0x190);
        math::zero(turn);

        math::add(last_push, applied_push, push);

        needs_oversampling = false;
        if (last_push.x == 0.0f) {
            if (last_push.y != 0.0f) {
                pushed_last_frame = true;
            } else if (last_push.z == 0.0f) {
                pushed_last_frame = false;
            }
        }

        math::zero(applied_push);
        math::zero(push);
        field_0x128 = 0;
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
        math::add(oversample_force, *param_1);

        if (param_4 == nullptr) {
            param_4 = &local_80;
            math::sub(local_80, *param_2, world_transform.w);
        }

        needs_oversampling = true;
        vec3f u0;
        math::cross(u0, *param_4, *param_1);
        math::add(field_0xcc, u0);

        math::add(field_0x130.x, param_3->x);
        math::add(field_0x130.y, param_3->y);
        math::add(field_0x130.z, param_3->z);
        math::add(field_0x130.w, param_3->w);

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

        iVar2 = math::RealCubic(
            (-inertia_tensor.x - inertia_tensor.y) - inertia_tensor.z,
            (((inertia_tensor.x * inertia_tensor.y + inertia_tensor.x * inertia_tensor.z + inertia_tensor.y * inertia_tensor.z) - p3sq) - p4sq) - p5sq,
            ((inertia_tensor.x * p5sq + inertia_tensor.y * p4sq + inertia_tensor.z * p3sq) - (param_3 + param_3) * param_4 * param_5) - inertia_tensor.x * inertia_tensor.y * inertia_tensor.z,
            0.001f,
            &local_70,
            &local_6c,
            local_68
        );

        if (iVar2 == 1) {
            inertia_tensor.x = local_70;
            inertia_tensor.z = local_70;
            inertia_tensor.y = local_70;
            math::identity(*param_2);
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
        math::copy(*velocity, world_velocity);
        velocity->x += g_datTimeManager.InvSeconds * last_push.x;
        velocity->y += g_datTimeManager.InvSeconds * last_push.y;
        velocity->z += g_datTimeManager.InvSeconds * last_push.z;
    }

    void phInertialCS::GetForce(f32 param_1, vec3f* f) {
        f->x = force.x;
        f->y = force.y;
        f->z = force.z;

        if (param_1 > 0.0f) {
            f->x += param_1 * impulse.x;
            f->y += param_1 * impulse.y;
            f->z += param_1 * impulse.z;
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

        math::cross(*param_2, angular_velocity, d);
        math::cross(*param_2, angular_velocity, *param_2);

        param_2->x += inv_mass * force.x;
        param_2->y += inv_mass * force.y;
        param_2->z += inv_mass * force.z;

        vec3f x0;
        math::cross(x0, angular_momentum, angular_velocity);
        math::add(x0, torque);

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
        math::cross(x0, angular_velocity, d);

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
        bool bVar1;

        if (world_transform.x.x == 0.0f && world_transform.x.y == 0.0f) bVar1 = world_transform.x.z != 0.0f;
        else bVar1 = true;

        if (bVar1) {
            if (world_transform.y.x == 0.0f && world_transform.y.y == 0.0f) bVar1 = world_transform.y.z != 0.0f;
            else bVar1 = true;

            if (bVar1) {
                if ((world_transform.z.x == 0.0) && (world_transform.z.y == 0.0)) bVar1 = world_transform.z.z != 0.0f;
                else bVar1 = true;

                if (bVar1) {
                    math::normalize(world_transform.y);
                    math::cross(world_transform.z, world_transform.x, world_transform.y);
                    math::normalize(world_transform.z);
                    math::cross(world_transform.x, world_transform.y, world_transform.z);

                    field_0x1c4 = math::randf() * 0.5f * 512.0f;
                }

                return &world_transform;
            }
        }

        math::identity_3x3(world_transform);
        field_0x1c4 = math::randf() * 0.5f * 512.0f;
        return nullptr;
    }

    void phInertialCS::SetVelocity(vec3f* velocity) {
        math::copy(world_velocity, *velocity);
        momentum.x = velocity->x * mass;
        momentum.y = velocity->y * mass;
        momentum.z = velocity->z * mass;
    }

    void phInertialCS::TetrahedronAngInertia(vec3f* verts, f32* param_2, f32* param_3, f32* param_4, f32* param_5, f32* param_6, f32* param_7) {
        math::ReOrderVerts(verts, nullptr);

        vec3f d0;
        math::sub(d0, verts[1], verts[0]);
        vec3f d1;
        math::sub(d1, verts[2], verts[0]);

        f32 f0 = math::dot(d1, d0) /  math::magnitudeSq(d0);

        vec3f u0 = {
            verts[2].x - (verts[0].x + f0 * d0.x),
            verts[2].y - (verts[0].y + f0 * d0.y),
            verts[2].z - (verts[0].z + f0 * d0.z)
        };

        f32 f1 = math::magnitude(u0) * math::magnitude(d0) * 0.01666667f;

        f32 f2 = f1 * (
            verts[0].x * verts[0].x +
            verts[1].x * verts[1].x +
            verts[2].x * verts[2].x +
            verts[0].x * verts[1].x +
            verts[0].x * verts[2].x +
            verts[1].x * verts[2].x
        );

        f32 f3 = f1 * (
            verts[0].y * verts[0].y +
            verts[1].y * verts[1].y +
            verts[2].y * verts[2].y +
            verts[0].y * verts[1].y +
            verts[0].y * verts[2].y +
            verts[1].y * verts[2].y
        );

        f32 f4 = f1 * (
            verts[0].z * verts[0].z +
            verts[1].z * verts[1].z +
            verts[2].z * verts[2].z +
            verts[0].z * verts[1].z +
            verts[0].z * verts[2].z +
            verts[1].z * verts[2].z
        );

        f32 f5 = verts[0].y * verts[0].z + verts[1].y * verts[1].z + verts[2].y * verts[2].z;
        f32 f6 = verts[0].x * verts[0].y + verts[1].x * verts[1].y + verts[2].x * verts[2].y;
        f32 f7 = verts[0].x * verts[0].z + verts[1].x * verts[1].z + verts[2].x * verts[2].z;

        f1 *= -0.5f;
        *param_2 = f3 + f4;
        *param_3 = f2 + f4;
        *param_4 = f2 + f3;
        *param_5 = f1 * (f6 + f6 + verts[0].x * verts[2].y + verts[0].y * verts[2].x + verts[0].x * verts[1].y + verts[0].y * verts[1].x + verts[1].x * verts[2].y + verts[1].y * verts[2].x);
        *param_6 = f1 * (f7 + f7 + verts[0].x * verts[2].z + verts[0].z * verts[2].x + verts[0].x * verts[1].z + verts[0].z * verts[1].x + verts[1].x * verts[2].z + verts[1].z * verts[2].x);
        *param_7 = f1 * (f5 + f5 + verts[0].y * verts[2].z + verts[0].z * verts[2].y + verts[0].y * verts[1].z + verts[0].z * verts[1].y + verts[1].y * verts[2].z + verts[1].z * verts[2].y);
    }
};