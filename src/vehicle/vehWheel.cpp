#include <libsr2/vehicle/vehWheel.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/io/datParser.h>
#include <math.h>

namespace sr2 {
    vehWheel::vehWheel() {                     
        steering_limit = 0.39;
        camber_limit = -1.0f;
        suspension_extent = 0.33;
        tire_drag_coeficient_lat = 0.05;
        tire_drag_coeficient_lng = 0.02;
        tire_damping_coefficient_lng = 0.25;
        tire_displacement_limit_lng = 0.15;
        field_0x1c0 = 0.3;
        field_0x1c4 = 0.1;
        optimum_slip_percent = 0.14;
        static_friction = 2.5;
        sliding_friction = 2.2;
        field_0x1c8 = 5000.0f;
        suspension_damping_coefficient = 0.1;
        suspension_factor = 1.0f;
        suspension_limit = 0.33;
        tire_damping_coefficient_lat = 0.25;
        tire_displacement_limit_lat = 0.15;
        phys_obj = NULL;
        vehicle = NULL;
        some_str = NULL;
        steering_offset = 0.0f;
        wobble_limit = 0.0f;
        axle_limit = 0.0f;
        field_0x1b0 = vec3f({ 0.0f, 0.0f, 0.0f });
        field_0x1f4 = 5000.0f;
        field_0x1f0 = 5000.0f;
        brake_coefficient = 1.0f;
        handbrake_coefficient = 1.0f;
        field_0x1bc = 0.0f;
        friction_handling = 1.0f;
    }

    vehWheel::~vehWheel() {
    }

    datParserNode* vehWheel::prepare_parser(datParser* p) {
        p->add(PARSE_TYPE::FLOAT, "SuspensionExtent", &suspension_extent, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "SuspensionLimit", &suspension_limit, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "SuspensionFactor", &suspension_factor, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "SuspensionDampCoef", &suspension_damping_coefficient, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "SteeringLimit", &steering_limit, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "SteeringOffset", &steering_offset, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "BrakeCoef", &brake_coefficient, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "HandbrakeCoef", &handbrake_coefficient, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "CamberLimit", &camber_limit, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "WobbleLimit", &wobble_limit, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "AxleLimit", &axle_limit, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "TireDispLimitLong", &tire_displacement_limit_lng, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "TireDampCoefLong", &tire_damping_coefficient_lng, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "TireDragCoefLong", &tire_drag_coeficient_lng, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "TireDispLimitLat", &tire_displacement_limit_lat, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "TireDampCoefLat", &tire_damping_coefficient_lat, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "TireDragCoefLat", &tire_drag_coeficient_lat, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "OptimumSlipPercent", &optimum_slip_percent, 1, nullptr);
        p->add(PARSE_TYPE::FLOAT, "StaticFric", &static_friction, 1, nullptr);
        return p->add(PARSE_TYPE::FLOAT, "SlidingFric", &sliding_friction, 1, nullptr);
    }

    void vehWheel::AddNormalLoad(f32 load) {
        f32 fVar1;

        fVar1 = field_0x1c8 + load;
        if (fVar1 < 1.0f) fVar1 = 1.0f;
        SetNormalLoad(fVar1);
    }

    void vehWheel::SetNormalLoad(f32 load) {
        f32 fVar1;
        f32 fVar2;
        f32 fVar3;
        f32 fVar4;
        f32 fVar5;

        field_0x1c8 = load;
        if (suspension_factor < 0.75f) {
            suspension_factor = 0.75f;
        }
        fVar1 = suspension_extent;
        fVar2 = field_0x1c8;
        fVar4 = 1.0 / (fVar1 * (suspension_limit + fVar1));
        fVar1 = (suspension_factor * fVar1 + suspension_limit) * fVar2 * fVar4;
        fVar3 = (fVar2 + fVar2) / tire_displacement_limit_lat;
        field_0x20c = fVar1;
        fVar5 = (fVar2 + fVar2) / tire_displacement_limit_lng;
        field_0x214 = (suspension_damping_coefficient + suspension_damping_coefficient) * sqrtf(fVar1 * fVar2);
        field_0x210 = ((suspension_factor - 1.0f) * fVar2 * fVar4) / fVar1;
        //fVar1 = ((vehicle->base).collider.pfio.field_0x30)->gravity;
        field_0x268 = fVar3;
        field_0x1f4 = handbrake_coefficient * fVar2 * field_0x1c0 * static_friction;
        field_0x254 = fVar5;
        field_0x1f0 = brake_coefficient * fVar2 * field_0x1c0 * static_friction;
        fVar1 = -fVar2 / (fVar1 * -9.8f);
        field_0x26c = (tire_damping_coefficient_lat + tire_damping_coefficient_lat) * sqrtf(fVar1 * fVar3);
        field_0x258 = (tire_damping_coefficient_lng + tire_damping_coefficient_lng) * sqrtf(fVar1 * fVar5);
    }

    void vehWheel::CalcDispAndDamp(f32 param_1, f32 param_2, f32 param_3, f32* param_4, f32* param_5, i32* param_6) { 
        f32 fVar1;

        fVar1 = *param_4;
        if (0.0f <= param_2) {
            if (param_1 < fVar1 + param_2) {
                fVar1 = fVar1 - param_3;
                *param_4 = param_1 > fVar1 ? param_1 : fVar1;
                *param_5 = 0.0f;
                *param_6 = 0;
                return;
            }
            *param_4 = fVar1 + param_2;
        }
        else {
            if (fVar1 + param_2 < param_1) {
                fVar1 = fVar1 + param_3;
                *param_4 = param_1 < fVar1 ? param_1 : fVar1;
                *param_5 = 0.0f;
                *param_6 = 0;
                return;
            }
            *param_4 = fVar1 + param_2;
        }
        *param_5 = param_2;
        *param_6 = 1.0f;
    }

    void vehWheel::ComputeConstants() {
        bool bVar1;
        f32 fVar2;
        f32 fVar3;
        vehCarSim* veh;

        fVar2 = field_0x1b0.x;
        if (field_0x1c4 < fabsf(fVar2)) {
            if (0.0f < fVar2) {
                fVar3 = 1.0f;
            }
            else {
                fVar3 = 0.0f;
                if (fVar2 < 0.0f) {
                    fVar3 = -1.0f;
                }
            }
            field_0x1bc = fVar3 * 0.5f * field_0x1c4;
        }
        else {
            field_0x1bc = 0.0f;
        }
        veh = vehicle;
        inv_optimum_slip_percent_squared = 1.0f / (optimum_slip_percent * optimum_slip_percent);

        if (veh == nullptr) {
            fVar2 = fabsf(field_0x1b0.z);
            bVar1 = 0.01f < fabsf(field_0x1b0.x);
            // Yes, this is in the decompilation. Not sure why they allowed the dereferencing of a null pointer
            // fVar2 = -((veh->base).collider.pfio.field_0x30)->gravity * -9.8f * phys_obj->mass * fVar2) / (fVar2 + fVar2);
        }
        else {
            fVar2 = fabsf(field_0x1b0.z);
            bVar1 = field_0x1c4 < fabsf(field_0x1b0.x);
            // fVar2 = (fabsf(field_0x1b8 - -(veh->base).collider.center_of_mass.z) / (fVar2 + fVar2)) * -((veh->base).collider.pfio.field_0x30)->gravity * -9.8f * ((veh->base).collider.pfio.solver)->mass;
        }

        if (bVar1) fVar2 = fVar2 * 0.5f;

        SetNormalLoad(fVar2);
        return;
    }

    f32 vehWheel::ComputeFriction(f32 slip_percent, f32* friction) {
        f32 fVar1;
        f32 osp;
        f32 fVar2;
        f32 fVar3;

        fVar3 = fabsf(slip_percent);
        osp = optimum_slip_percent;
        fVar2 = field_0x1d4 * sliding_friction;
        fVar1 = field_0x1d4 * static_friction * inv_optimum_slip_percent_squared * fVar3 * ((osp + osp) - fVar3);

        if (fVar3 <= osp) {
            *friction = (fVar3 * 0.5f) / osp;
            return fVar1;
        }

        *friction = ((fVar3 - osp) * 0.5f) / (1.0f - osp) + 0.5f;

        if (fVar2 < fVar1) fVar2 = fVar1;

        return fVar2;
    }

    void vehWheel::ComputeGM() {
        u32 uVar1;
        u32 uVar2;
        u64 *puVar3;
        u32 uVar4;
        vec3f *pvVar5;
        u64 uVar6;
        f32 fVar7;
        f32 fVar8;
        f32 fVar9;
        f32 fVar10;
        f32 fVar11;
        f32 fVar12;
        f32 fVar13;

        fVar7 = field_0x1c0 * 0.2;
        fVar8 = (field_0x1c0 * 0.05 * field_0x200) / field_0x1c8;
        fVar7 = fVar8 < fVar7 ? fVar8 : fVar7;
        fVar7 = suspension_value - (fVar7 > 0.0f ? fVar7 : 0.0f);
        (world_transform).w.x = (world_transform).w.x + fVar7 * (world_transform).y.x;
        fVar8 = (world_transform).w.z;
        (world_transform).w.y = (world_transform).w.y + fVar7 * (world_transform).y.y;
        (world_transform).w.z = fVar8 + fVar7 * (world_transform).y.z;

        if (0.0f < axle_limit) {
            if (field_0x16c == 0) {
                fVar7 = field_0x218;
            } else {
                /*
                    // nonsense
                    uVar4 = (i32)&(world_transform_1).z.y + 3;
                    uVar1 = uVar4 & 7;
                    pvVar5 = &(world_transform_1).z;
                    uVar2 = (uint)pvVar5 & 7;
                    uVar6 = (*(long *)(uVar4 - uVar1) << (7 - uVar1) * 8 | (long)(i32)field_0x16c & 0xffffffffffffffffU >> (uVar1 + 1) * 8) &
                            -1 << (8 - uVar2) * 8 | *(ulong *)((i32)pvVar5 - uVar2) >> uVar2 * 8;
                    fVar7 = (world_transform_1).z.z;
                    uVar4 = (i32)&(world_transform).z.y + 3;
                    uVar1 = uVar4 & 7;
                    puVar3 = (ulong *)(uVar4 - uVar1);
                    *puVar3 = *puVar3 & -1 << (uVar1 + 1) * 8 | uVar6 >> (7 - uVar1) * 8;
                    pvVar5 = &(world_transform).z;
                    uVar4 = (uint)pvVar5 & 7;
                    pvVar5 = (vec3f *)((i32)pvVar5 - uVar4);
                    *(ulong *)pvVar5 = uVar6 << uVar4 * 8 | *(ulong *)pvVar5 & 0xffffffffffffffffU >> (8 - uVar4) * 8;
                */

                (world_transform).z.z = fVar7;
                fVar12 = (world_transform).z.y;
                fVar8 = (world_transform).y.y;
                fVar7 = (world_transform).y.x;
                fVar11 = (world_transform).z.x;
                fVar13 = (world_transform).z.z;
                fVar9 = (world_transform).y.z;
                fVar10 = fVar9 * fVar11 - fVar7 * fVar13;
                (world_transform).x.z = fVar7 * fVar12 - fVar8 * fVar11;
                (world_transform).x.x = fVar8 * fVar13 - fVar9 * fVar12;
                (world_transform).x.y = fVar10;
                fVar7 = (world_transform).x.z;
                fVar9 = (world_transform).x.x;
                fVar8 = 1.0f / sqrtf(fVar9 * fVar9 + fVar10 * fVar10 + fVar7 * fVar7);
                (world_transform).x.x = fVar9 * fVar8;
                fVar7 = (world_transform).x.y * fVar8;
                fVar8 = (world_transform).x.z * fVar8;
                (world_transform).x.y = fVar7;
                (world_transform).x.z = fVar8;
                fVar9 = (world_transform).z.y;
                fVar13 = (world_transform).x.x;
                fVar12 = (world_transform).z.x;
                fVar10 = (world_transform).z.z;
                fVar11 = fVar10 * fVar13 - fVar12 * fVar8;
                (world_transform).y.z = fVar12 * fVar7 - fVar9 * fVar13;
                (world_transform).y.x = fVar9 * fVar8 - fVar10 * fVar7;
                (world_transform).y.y = fVar11;
                fVar7 = (world_transform).y.z;
                fVar8 = (world_transform).y.x;
                fVar9 = 1.0f / sqrtf(fVar8 * fVar8 + fVar11 * fVar11 + fVar7 * fVar7);
                (world_transform).y.x = fVar8 * fVar9;
                fVar7 = (world_transform).y.z;
                (world_transform).y.y = (world_transform).y.y * fVar9;
                (world_transform).y.z = fVar7 * fVar9;
            }
        } else {
            //mat3x4f::rotate_axis_angle_notrans(axle_value,&world_transform,(vec3f *)&world_transform);
        }

        if (field_0x218 != 0.0) {
            //mat3x4f::rotate_axis_angle_notrans(fVar7,&world_transform,&(world_transform).z);
        }
        return;
    }

    void vehWheel::ComputeSlipPercent(f32* slip_percent, f32 param_3, f32 synchronous_speed) {
        f32 fVar1;

        fVar1 = 0.0f;
        if (param_3 == 0.0f) {
            *slip_percent = 0.0f;
            return;
        }
        if (fabsf(param_3) < fabsf(synchronous_speed)) {
            fVar1 = param_3 / fabsf(synchronous_speed);
        }
        else {
            if (0.0f < param_3) {
                fVar1 = 1.0f;
            }
            else {
                if (param_3 < 0.0f) {
                    fVar1 = -1.0f;
                }
            }
        }
        *slip_percent = fVar1;
    }

    void vehWheel::CopyFrom(vehWheel* from) {
        suspension_damping_coefficient = from->suspension_damping_coefficient;
        suspension_limit = from->suspension_limit;
        suspension_extent = from->suspension_extent;
        suspension_factor = from->suspension_factor;
        steering_limit = from->steering_limit;
        camber_limit = from->camber_limit;
        wobble_limit = from->wobble_limit;
        axle_limit = from->axle_limit;
        steering_offset = from->steering_offset;
        brake_coefficient = from->brake_coefficient;
        handbrake_coefficient = from->handbrake_coefficient;
        field_0x1c0 = from->field_0x1c0;
        field_0x1c4 = from->field_0x1c4;
        tire_displacement_limit_lng = from->tire_displacement_limit_lng;
        tire_damping_coefficient_lng = from->tire_damping_coefficient_lng;
        tire_drag_coeficient_lng = from->tire_drag_coeficient_lng;
        tire_displacement_limit_lat = from->tire_displacement_limit_lat;
        tire_damping_coefficient_lat = from->tire_damping_coefficient_lat;
        tire_drag_coeficient_lat = from->tire_drag_coeficient_lat;
        optimum_slip_percent = from->optimum_slip_percent;
        static_friction = from->static_friction;
        sliding_friction = from->sliding_friction;
    }

    f32 vehWheel::GetVisualDispLat() {
        f32 min;
        min = tire_displacement_limit_lat < field_0x234 ? tire_displacement_limit_lat : field_0x234;
        return -tire_displacement_limit_lat > min ? -tire_displacement_limit_lat : min;
    }

    f32 vehWheel::GetVisualDispLng() {
        f32 min;
        min = tire_displacement_limit_lng < field_0x238 ? tire_displacement_limit_lng : field_0x238;
        return -tire_displacement_limit_lng > min ? -tire_displacement_limit_lng : min;
    }

    f32 vehWheel::GetVisualDispVert() {
        f32 fVar1;
        f32 fVar2;

        fVar2 = field_0x1c0 * 0.2f;
        fVar1 = (field_0x1c0 * 0.05f * field_0x200) / field_0x1c8;
        fVar1 = fVar1 < fVar2 ? fVar1 : fVar2;
        return fVar1 > 0.0f ? fVar1 : 0.0f;
    }

    void vehWheel::SetBrake(f32 param_1) {
        field_0x1ec = param_1 * field_0x1c8 * field_0x1c0 * static_friction;
    }

    void vehWheel::SetInputs(f32 param_1, f32 param_2, f32 param_3) {
        f32 fVar1;
        f32 fVar2;

        field_0x1f8 = -param_1 * steering_limit;
        if (0.0f < field_0x1b0.x) {
            fVar1 = 1.0f;
            fVar2 = steering_offset;
        }
        else {
            fVar1 = 0.0f;
            if (field_0x1b0.x < 0.0f) {
                fVar1 = -1.0f;
            }
            fVar2 = steering_offset;
        }
        field_0x1ec = param_2 * field_0x1f0 + param_3 * field_0x1f4;
        field_0x1f8 = field_0x1f8 * (1.0f - fVar1 * fVar2 * field_0x1f8);
        return;
    }

    void vehWheel::Update() {
        
    }

    void vehWheel::Reset() {
        suspension_value = 0.0f;
        suspension_velocity = 0.0f;
        slip_percent_lat = 0.0f;
        slip_percent_lng = 0.0f;
        position_2 = { 0.0f, 0.0f, 0.0f };
        is_on_ground = false;
        math::identity(world_transform_1);

        field_0x208 = 0.0f;
        field_0x244 = 0.0f;
        field_0x218 = 0;
        field_0x21c = 0.0f;
        axle_value = 0.0f;
        field_0x1f8 = 0.0f;
        field_0x1ec = 0.0f;
        field_0x248 = 0.0f;
        field_0x1e4 = 0;
        field_0x234 = 0.0f;
        field_0x238 = 0.0f;
        field_0x240 = 0.0f;
        field_0x23c = 0.0f;
        field_0x200 = 0.0f;
        field_0x220 = 0.0f;
        field_0x228 = 0;
        field_0x16c = 0;
        field_0x168 = 0;
        field_0x1d4 = 1.0f;
        field_0x17c = 0.0f;
        field_0xb0 = nullptr;
        field_0x1cc = 0;
        field_0x1d0 = 0.0f;
        field_0x1d8 = 0.0f;
        field_0x1dc = 0;
        field_0x1e0 = 0.0f;
        field_0x170 = 0.0f;
        field_0x174 = 0.0f;
        field_0x178 = 0.0f;
    }
};