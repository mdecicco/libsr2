#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phPolygon.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/math.h>
#include <libsr2/managers/datTimeManager.h>
#include <math.h>

namespace sr2 {
    phInertialCS::phInertialCS() {
        init(1.0f, 1.0f, 1.0f, 1.0f);

        max_velocity = 500.0;                           
        max_ang_velocity = { 5.0f, 5.0f, 5.0f };

        math::zero(last_push);
        math::zero(push);
        math::zero(turn);
        math::zero(applied_push);

        next = nullptr;
        last = nullptr;

        zero();
    }

    phInertialCS::~phInertialCS() {
    }



    /*
     * Virtual
     */
    void phInertialCS::resetTransform() {
        zero();
    }

    void phInertialCS::zero() {
        math::identity(world_transform);
        freeze();

        math::zero(last_push);
        pushed_last_frame = false;
    }

    void phInertialCS::freeze() {
        math::zero(momentum);
        math::zero(angular_momentum);
        math::zero(world_velocity);
        math::zero(angular_velocity);
        zeroForces();
    }

    void phInertialCS::init(f32 _mass, f32 Ixx, f32 Iyy, f32 Izz) { 
        f32 fVar1;
        f32 fVar2;

        active = true;
        mass = _mass;

        if (0.0f < mass) fVar1 = 1.0f / mass;
        else fVar1 = 3.402823e+38f;

        inv_mass = fVar1;
        ang_inertia.x = Ixx;
        ang_inertia.z = Izz;
        ang_inertia.y = Iyy;
        fVar1 = ang_inertia.x;
        if (0.0f < fVar1) {
            fVar1 = 1.0f / fVar1;
        }
        else {
            fVar1 = 3.402823e+38f;
        }
        fVar2 = ang_inertia.y;
        inv_ang_inertia.x = fVar1;
        if (0.0 < fVar2) {
            fVar2 = 1.0 / fVar2;
        }
        else {
            fVar2 = 3.402823e+38f;
        }
        fVar1 = ang_inertia.z;
        inv_ang_inertia.y = fVar2;
        if (0.0f < fVar1) {
            fVar1 = 1.0f / fVar1;
        }
        else {
            fVar1 = 3.402823e+38f;
        }
        inv_ang_inertia.z = fVar1;
    }

    // todo
    void phInertialCS::initFromValues(f32 mass, f32 invMass, const vec3f& angInertia, const vec3f& invAngInertia) {

    }

    // todo
    void phInertialCS::update() {

    }

    // todo
    void phInertialCS::updateInner(f32 dt) {

    }

    // todo
    void phInertialCS::updateImplicit() {

    }

    // todo
    void phInertialCS::method_0x40() {
    }

    void phInertialCS::calcNetPush(const vec3f& p1, const vec3f& p2) {
        math::FUN_0031b2f8(push, p1);
    }
    
    // todo
    void phInertialCS::moveICS() {
    }

    void phInertialCS::getLocalVelocity(vec3f& param_1, vec3f& param_2) {
        vec3f d;
        math::sub(d, param_1, world_transform.w);
        math::copy(param_2, d);

        vec3f x0;
        math::cross(x0, angular_velocity, d);

        math::add(param_2, x0, world_velocity);
    }

    void phInertialCS::getLocalAcceleration(vec3f* param_1, vec3f* param_2) {
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
            inv_ang_inertia.x * math::dot(x0, world_transform.x),
            inv_ang_inertia.y * math::dot(x0, world_transform.y),
            inv_ang_inertia.z * math::dot(x0, world_transform.z)
        };

        math::mult(x0, world_transform, x0);
        math::cross(x0, x0, d);
        math::add(*param_2, x0);
    }

    // todo
    void phInertialCS::method_0x68() {
    }

    // todo
    void phInertialCS::getInvMassMatrix2(const vec3f& unk, mat3x4f& out) {
    }



    /*
     * Non-Virtual
     */

    void phInertialCS::initBoxMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z) {
        init(
            mass,
            (mass * (inertia_box_y * inertia_box_y + inertia_box_z * inertia_box_z)) / 12.0f,
            (mass * (inertia_box_x * inertia_box_x + inertia_box_z * inertia_box_z)) / 12.0f,
            (mass * (inertia_box_x * inertia_box_x + inertia_box_y * inertia_box_y)) / 12.0f
        );
    }

    void phInertialCS::initBoxDensity(f32 density, f32 width, f32 height, f32 length) {
        initBoxMass(density * width * length * height * 1000.0f, width, height, length);
    }

    void phInertialCS::initCylinderMass(f32 mass, f32 inertia_box_x, f32 inertia_box_y, f32 inertia_box_z, char axis) {
        f32 fVar2;
        f32 fVar3;
        f32 fVar4;

        fVar4 = mass * inertia_box_x * inertia_box_x * 0.5f;
        fVar2 = fVar4 * 0.5f + (mass * inertia_box_y * inertia_box_y) / 12.0f;

        if (axis == 'y') fVar3 = fVar2;
        else {
            if ('y' < axis) {
                if (axis != 'z') return;
                init(mass, fVar2, fVar2, fVar4);
                return;
            }

            if (axis != 'x') return;
            fVar3 = fVar4;
            fVar4 = fVar2;
        }

        init(mass, fVar3, fVar4, fVar2);
    }

    void phInertialCS::initCylinderDensity(f32 density, f32 param_2, f32 param_3, f32 param_4, char axis) {
        initCylinderMass(param_2 * 3.141593f * param_2 * param_3 * density * 1000.0f, param_2, param_3, param_4, axis);
    }

    void phInertialCS::initHotdogMass(f32 mass, f32 param_2, f32 param_3, char axis) {                             
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
                if (axis == 'z') init(mass, fVar3, fVar3, fVar4);
                return;
            }
            if (axis != 'x') return;
            fVar2 = fVar4;
            fVar4 = fVar3;
        }

        init(mass, fVar2, fVar4, fVar3);
    }

    void phInertialCS::initHotdogDensity(f32 param_1, f32 param_2, f32 param_3, char axis) {
        initHotdogMass(
            (param_2 * 1.333333f + param_3) * 3.141593f * param_2 * param_2 * param_1 * 1000.0f,
            param_2,
            param_3,
            axis
        );
    }

    void phInertialCS::initSphereMass(f32 param_1, f32 param_2) {
        f32 I = param_1 * 0.4f * param_2 * param_2;
        init(param_1, I, I, I);
    }

    void phInertialCS::initSphereDensity(f32 density, f32 param_2) {
        initSphereMass(param_2 * 4.18878f * param_2 * param_2 * density * 1000.0f, param_2);
    }

    void phInertialCS::initFromGeometry(f32 mass, vec3f* vertices, phPolygon* polygons, i32 poly_count, mat3x4f* param_6) {
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

                f32 fVar3 = math::dot(polygons[i].normal, tri[0]);
                tetrahedronAngInertia(tri, &to0, &to1, &to2, &to3, &to4, &to5);

                fVar10 += polygons[i].normalMag * fVar3;
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
                    tetrahedronAngInertia(tri, &to0, &to1, &to2, &to3, &to4, &to5);

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

        init(fabsf(fVar10) * fVar9 * 0.3333333f, Ixx, Iyy, Izz);

        if (param_6) {
            fVar6 *= fVar9;
            fVar8 *= fVar9;
            fVar7 *= fVar9;
            if (Iyy <= Ixx) Ixx = Iyy;
            if (Izz < Ixx) Ixx = Izz;

            fVar2 = fVar8;
            if (fVar8 <= fVar7) fVar2 = fVar7;
            if (fVar2 < fVar6) fVar2 = fVar6;

            if (Ixx < fVar2 * 0.05f) findPrincipalAxes(param_6, fVar8, fVar7, fVar6);
        }
    }
    
    void phInertialCS::zeroForces() {
        math::zero(force);
        math::zero(torque);
        math::zero(oversampleForce);
        math::zero(oversampleTorque);
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

    mat3x4f* phInertialCS::addInertia(mat3x4f* param_1, f32 param_2, vec3f* param_4, vec3f* param_5) {
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

            init(
                mass + param_2,
                ang_inertia.x + param_4->x + param_2 * (local_a0.x.y * local_a0.x.y + local_a0.x.z * local_a0.x.z) + mass * (local_a0.y.z * local_a0.y.z + local_a0.z.x * local_a0.z.x),
                ang_inertia.y + param_4->y + param_2 * (local_a0.x.x * local_a0.x.x + local_a0.x.z * local_a0.x.z) + mass * (local_a0.y.y * local_a0.y.y + local_a0.z.x * local_a0.z.x),
                ang_inertia.z + param_4->z + param_2 * (local_a0.x.x * local_a0.x.x + local_a0.x.y * local_a0.x.y) + mass * (local_a0.y.y * local_a0.y.y + local_a0.y.z * local_a0.y.z)
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
            } else findPrincipalAxes(param_1, fVar2, fVar1, fVar3);
        }

        return param_1;
    }

    mat3x4f* phInertialCS::addInertia(mat3x4f *param_1, f32 param_2, vec3f *param_4, mat3x4f *param_5) {
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

            init(
                mass + param_2,
                ang_inertia.x + local_110.x.x + param_2 * (local_140.x.y * local_140.x.y + local_140.x.z * local_140.x.z) + mass * (local_140.y.z * local_140.y.z + local_140.z.x * local_140.z.x),
                ang_inertia.y + local_110.y.y + param_2 * (local_140.x.x * local_140.x.x + local_140.x.z * local_140.x.z) + mass * (local_140.y.y * local_140.y.y + local_140.z.x * local_140.z.x),
                ang_inertia.z + local_110.z.z + param_2 * (local_140.x.x * local_140.x.x + local_140.x.y * local_140.x.y) + mass * (local_140.y.y * local_140.y.y + local_140.y.z * local_140.y.z)
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
                findPrincipalAxes(param_1, fVar3, fVar2, fVar1);
            }
        }

        return param_1;
    }

    void phInertialCS::applyContactForce(const vec3f& force, const vec3f& maybeContactPoint, const mat3x4f& param_3, vec3f* param_4) {
        vec3f local_80;
        mat3x4f local_70;

        field_0x128 = 1;
        math::add(oversampleForce, force);

        if (param_4 == nullptr) {
            param_4 = &local_80;
            math::sub(local_80, maybeContactPoint, world_transform.w);
        }

        needs_oversampling = true;
        vec3f u0;
        math::cross(u0, *param_4, force);
        math::add(oversampleTorque, u0);

        math::add(field_0x130.x, param_3.x);
        math::add(field_0x130.y, param_3.y);
        math::add(field_0x130.z, param_3.z);
        math::add(field_0x130.w, param_3.w);

        local_70.x.x =  0.0f;
        local_70.x.y = -param_4->z;
        local_70.x.z =  param_4->y;

        local_70.y.x =  param_4->z;
        local_70.y.y =  0.0f;
        local_70.y.z = -param_4->x;

        local_70.z.x = -param_4->y;
        local_70.z.y =  param_4->x;
        local_70.z.z =  0.0f;

        math::mult(local_70, param_3);
        //mat3x4f::addEq(&field_0x160, &local_70);
        //mat3x4f::Dot3x3CrossProdTranspose(&local_70, param_4);
        //mat3x4f::addEq(&field_0x190 ,&local_70);
    }

    mat3x4f* phInertialCS::findPrincipalAxes(mat3x4f* param_2, f32 param_3, f32 param_4, f32 param_5) {
        bool bVar1;
        i32 iVar2;
        f32 fVar3;
        f32 fVar4;
        f32 fVar5;
        mat3x4f out_p2;
        vec3f local_b0;
        mat3x4f local_a0;
        vec3f local_70;

        f32 p3sq = param_3 * param_3;
        f32 p4sq = param_4 * param_4;
        f32 p5sq = param_5 * param_5;

        iVar2 = math::RealCubic(
            (-ang_inertia.x - ang_inertia.y) - ang_inertia.z,
            (((ang_inertia.x * ang_inertia.y + ang_inertia.x * ang_inertia.z + ang_inertia.y * ang_inertia.z) - p3sq) - p4sq) - p5sq,
            ((ang_inertia.x * p5sq + ang_inertia.y * p4sq + ang_inertia.z * p3sq) - (param_3 + param_3) * param_4 * param_5) - ang_inertia.x * ang_inertia.y * ang_inertia.z,
            0.001f,
            &local_70.x,
            &local_70.y,
            &local_70.z
        );

        if (iVar2 == 1) {
            ang_inertia.x = local_70.x;
            ang_inertia.z = local_70.x;
            ang_inertia.y = local_70.x;
            math::identity(*param_2);
            return param_2;
        }

        if (iVar2 >= 2) {
            if (iVar2 == 2) {
                vec3f xAxis;
                principalAxis(local_70.x, param_3, param_4, param_5, &xAxis);
                out_p2.x = xAxis;

                if (xAxis.x >= 0.5773503f) {
                    ang_inertia = { local_70.x, local_70.y, local_70.y };
                    f32 fac = 1.0f / sqrtf(xAxis.x * xAxis.x + xAxis.y * xAxis.y);
                    out_p2.y.x = -xAxis.y * fac;
                    out_p2.y.y =  xAxis.x * fac;
                    out_p2.y.z =  0.0f    * fac;
                    math::cross(out_p2.z, xAxis, out_p2.y);
                } else {
                    if (xAxis.y < 0.0f) {
                        out_p2.x.x = -xAxis.x;
                        xAxis.y = -xAxis.y;
                        out_p2.x.z = -xAxis.z;
                    }

                    if (xAxis.y >= 0.5773503f) {
                        out_p2.y.x =  out_p2.x.x;
                        out_p2.x.y = -out_p2.x.x;
                        out_p2.y.z =  out_p2.x.z;
                        ang_inertia.x = local_70.y;
                        ang_inertia.z = local_70.y;
                        ang_inertia.y = local_70.x;
                        fVar4 = 1.0f / sqrtf(xAxis.y * xAxis.y + out_p2.x.y * out_p2.x.y + 0.0);
                        fVar3 = fVar4 * 0.0f;
                        out_p2.x.y = out_p2.x.y * fVar4;
                        fVar4 = xAxis.y * fVar4;

                        out_p2.z.x = out_p2.x.y * out_p2.x.z - fVar3 * xAxis.y;
                        out_p2.z.y = fVar3 * out_p2.x.x - fVar4 * out_p2.x.z;
                        out_p2.z.z = fVar4 * xAxis.y - out_p2.x.y * out_p2.x.x;

                        out_p2.x.x = fVar4;
                        out_p2.x.z = fVar3;
                        out_p2.y.y = xAxis.y;
                    } else {
                        out_p2.z.x = out_p2.x.x;
                        out_p2.z.y = xAxis.y;
                        out_p2.z.z = out_p2.x.z;

                        ang_inertia.x = local_70.y;
                        ang_inertia.z = local_70.x;
                        ang_inertia.y = local_70.y;

                        if (out_p2.x.z < 0.0f) math::negate(out_p2.z);

                        out_p2.x = { out_p2.z.z, 0.0f, -out_p2.z.x };
                        math::normalize(out_p2.x);
                        math::cross(out_p2.y, out_p2.z, out_p2.x);
                    }
                }
            } else if (iVar2 == 3) {
                principalAxis(local_70.x, param_3, param_4, param_5, &out_p2.x);
                principalAxis(local_70.y, param_3, param_4, param_5, &out_p2.y);
                math::copy(out_p2.z, out_p2.x);

                fVar3 = math::dot(out_p2.x, out_p2.y);
                if (fVar3 != 0.0f) {
                    out_p2.y.x -= fVar3 * out_p2.x.x;
                    out_p2.y.y -= fVar3 * out_p2.x.y;
                    out_p2.y.z -= fVar3 * out_p2.x.z;
                    math::normalize(out_p2.y);
                }

                vec3f fVar345 = out_p2.y;
                if (out_p2.x.x >= 0.5773503f) {
                    if (out_p2.y.y < 0.0) math::negate(out_p2.y);

                    if (0.5773503f <= out_p2.y.y) {
                        math::cross(out_p2.z, out_p2.x, out_p2.y);
                        ang_inertia.x = local_70.x;
                        ang_inertia.y = local_70.y;
                        ang_inertia.z = local_70.z;
                    } else {
                        math::copy(out_p2.z, out_p2.y);
                        if (out_p2.y.z < 0.0f) math::negate(out_p2.z, out_p2.y);
                        ang_inertia.x = local_70.x;
                        ang_inertia.y = local_70.z;
                        ang_inertia.z = local_70.y;
                        math::cross(out_p2.y, out_p2.z, out_p2.x);
                    }
                } else {
                    if (out_p2.y.x >= 0.5773503f) {
                        bVar1 = out_p2.x.z < 0.0;
                        math::copy(out_p2.x, out_p2.y);
                        if (bVar1) math::negate(out_p2.z);

                        if (out_p2.z.z >= 0.5773503f) {
                            math::mult(fVar345, out_p2.z, { out_p2.y.z, out_p2.y.x, out_p2.y.x });
                            math::cross(
                                out_p2.y,
                                { out_p2.z.x, out_p2.z.y, fVar345.z },
                                { fVar345.x, out_p2.y.y, out_p2.y.z }
                            );
                            local_70.y = local_70.x;
                            ang_inertia.x = local_70.y;
                            ang_inertia.z = local_70.y;
                            ang_inertia.y = local_70.z;
                        } else {
                            math::copy(out_p2.y, out_p2.z);
                            if (out_p2.z.y < 0.0f) math::negate(out_p2.y);

                            ang_inertia.x = local_70.y;
                            ang_inertia.y = local_70.x;
                            ang_inertia.z = local_70.z;
                            math::cross(out_p2.z, fVar345, out_p2.y);
                            local_70.y = local_70.x;
                        }
                    } else {
                        if (fabsf(out_p2.x.y) >= 0.5773503f) {
                            math::copy(out_p2.z, out_p2.y);
                            math::copy(out_p2.y, out_p2.x);

                            if (out_p2.x.y < 0.0f) math::negate(out_p2.y);
                            if (out_p2.y.z < 0.0f) math::negate(out_p2.z);

                            if (out_p2.z.z >= 0.5773503f) {
                                math::cross(out_p2.x, out_p2.y, out_p2.z);
                                ang_inertia.x = local_70.y;
                                ang_inertia.y = local_70.x;
                                ang_inertia.z = local_70.y;
                            } else {
                                math::copy(out_p2.x, out_p2.z);
                                if (out_p2.z.x < 0.0f) math::negate(out_p2.x, out_p2.z);

                                ang_inertia.x = local_70.y;
                                ang_inertia.y = local_70.x;
                                ang_inertia.z = local_70.z;

                                math::cross(out_p2.z, out_p2.x, out_p2.y);
                            }
                        } else {
                            if (out_p2.x.z < 0.0f) math::negate(out_p2.z);
                            if (out_p2.y.y < 0.0f) math::negate(out_p2.y);

                            if (out_p2.y.y > 0.5773503f) {
                                ang_inertia.x = local_70.z;
                                ang_inertia.y = local_70.y;
                                ang_inertia.z = local_70.x;

                                math::cross(out_p2.x, out_p2.y, out_p2.z);
                            }
                        }
                    }
                }
            }
        }

        math::zero(out_p2.w);
        math::mult(local_a0, out_p2, world_transform);

        math::normalize(local_a0.x);
        math::normalize(local_a0.y);
        math::normalize(local_a0.z);
        math::copy(*param_2, out_p2);

        return param_2;
    }

    void phInertialCS::getCMFilteredVelocity(vec3f* velocity) {
        math::copy(*velocity, world_velocity);
        velocity->x += datTimeManager::InvSeconds * last_push.x;
        velocity->y += datTimeManager::InvSeconds * last_push.y;
        velocity->z += datTimeManager::InvSeconds * last_push.z;
    }
    
    void phInertialCS::getLocalFilteredVelocity2(vec3f& out0, vec3f& vel) {
        getLocalVelocity(out0, vel);

        f32 lastPushMagSq = math::magnitudeSq(last_push);
        if (lastPushMagSq > 0.0001f) {
            f32 velDotPush = math::dot(vel, last_push);
            if (velDotPush < 0.0f) {
                f32 fVar3 = lastPushMagSq * datTimeManager::InvSeconds * datTimeManager::InvSeconds;
                f32 fVar2 = -(velDotPush * datTimeManager::InvSeconds);

                vec3f add;
                if (fVar3 < fVar2) math::mult(add, last_push, datTimeManager::InvSeconds);
                else math::mult(add, last_push, datTimeManager::InvSeconds * (fVar2 / fVar3));

                math::add(vel, add);
            }
        }
    }

    void phInertialCS::getForce(f32 param_1, vec3f* f) {
        f->x = force.x;
        f->y = force.y;
        f->z = force.z;

        if (param_1 > 0.0f) {
            f->x += param_1 * impulse.x;
            f->y += param_1 * impulse.y;
            f->z += param_1 * impulse.z;
        }
    }

    void phInertialCS::getInertiaMatrix(mat3x4f* inertia) {
        inertia->x.x = world_transform.x.x * ang_inertia.x;
        inertia->x.y = world_transform.y.x * ang_inertia.y;
        inertia->x.z = world_transform.z.x * ang_inertia.z;

        inertia->y.x = world_transform.x.y * ang_inertia.x;
        inertia->y.y = world_transform.y.y * ang_inertia.y;
        inertia->y.z = world_transform.z.y * ang_inertia.z;

        inertia->z.x = world_transform.x.z * ang_inertia.x;
        inertia->z.y = world_transform.y.z * ang_inertia.y;
        inertia->z.z = world_transform.z.z * ang_inertia.z;

        inertia->w.y = 0.0f;
        inertia->w.z = 0.0f;
        inertia->w.x = 0.0f;

        math::mult(*inertia, world_transform);
    }

    void phInertialCS::getInverseMassMatrix(const vec3f& unk, mat3x4f& out) {
        vec3f dp;
        math::sub(dp, unk, world_transform.w);

        mat3x4f mat0;
        math::copy(mat0, out);
        math::dot3x3CrossProdMtx(mat0, dp);

        mat3x4f mat1;
        math::copy(mat1, mat0);
        math::mult(mat1.x, inv_ang_inertia.x);
        math::mult(mat1.y, inv_ang_inertia.y);
        math::mult(mat1.z, inv_ang_inertia.z);

        math::transpose(mat0);
        math::mult(out, mat0, mat1);

        out.x.x += inv_mass;
        out.y.y += inv_mass;
        out.z.z += inv_mass;
        math::zero(out.w);
    }

    void phInertialCS::getInverseInertiaMatrix(mat3x4f* inertia) {
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

        tmp1.x.x = world_transform.x.x * inv_ang_inertia.x;
        tmp1.x.y = world_transform.x.y * inv_ang_inertia.x;
        tmp1.x.z = world_transform.x.z * inv_ang_inertia.x;

        tmp1.y.x = world_transform.y.x * inv_ang_inertia.y;
        tmp1.y.y = world_transform.y.y * inv_ang_inertia.y;
        tmp1.y.z = world_transform.y.z * inv_ang_inertia.y;

        tmp1.z.x = world_transform.z.x * inv_ang_inertia.z;
        tmp1.z.y = world_transform.z.y * inv_ang_inertia.z;
        tmp1.z.z = world_transform.z.z * inv_ang_inertia.z;

        math::transpose(tmp);
        math::mult(*inertia, tmp, tmp1);
    }

    vec3f* phInertialCS::principalAxis(f32 param_2, f32 param_3, f32 param_4, f32 param_5, vec3f* param_6) {
        f32 fVar2 = 1.0f / (param_2 - ang_inertia.z);
        f32 fVar3 = (param_3 + param_4 * param_5 * fVar2) * (1.0f / ((param_2 - ang_inertia.y) - param_5 * param_5 * fVar2));
        fVar2 = fVar2 * (param_4 + param_5 * fVar3);
        f32 fVar1 = 1.0f / sqrtf(fVar3 * fVar3 + 1.0f + fVar2 * fVar2);
        param_6->x = fVar1;
        param_6->y = fVar3 * fVar1;
        param_6->z = fVar2 * fVar1;
        return param_6;
    }

    mat3x4f* phInertialCS::rejuvinate() {
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

                    field_0x1c4 = math::frand() * 0.5f * 512.0f;
                }

                return &world_transform;
            }
        }

        math::identity_3x3(world_transform);
        field_0x1c4 = math::frand() * 0.5f * 512.0f;
        return nullptr;
    }

    void phInertialCS::setVelocity(vec3f* velocity) {
        math::copy(world_velocity, *velocity);
        momentum.x = velocity->x * mass;
        momentum.y = velocity->y * mass;
        momentum.z = velocity->z * mass;
    }

    void phInertialCS::tetrahedronAngInertia(vec3f* verts, f32* param_2, f32* param_3, f32* param_4, f32* param_5, f32* param_6, f32* param_7) {
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

    f32 phInertialCS::calcCollisionNoFriction(f32 p1, const vec3f& p2, const vec3f& p3) {
        if (p1 >= 0.0f) return 0.0f;
        mat3x4f imm;
        getInverseMassMatrix(p3, imm);

        vec3f x;
        math::mult(x, imm, p2);

        return p1 / math::dot(x, p2);
    }
};