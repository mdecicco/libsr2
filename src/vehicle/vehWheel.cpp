#include <libsr2/vehicle/vehWheel.h>
#include <libsr2/vehicle/vehCarSimBase.h>
#include <libsr2/sim/phBound.h>
#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phMaterial.h>
#include <libsr2/math/math.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/managers/phMaterialMgr.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/io/datParser.h>
#include <math.h>

namespace sr2 {
    f32 vehWheel::WeatherFriction = 1.0f;



    vehWheel::vehWheel() {
        steering_limit = 0.39;
        camber_limit = -1.0f;
        suspension_extent = 0.33;
        tire_drag_coeficient_lat = 0.05;
        tire_drag_coeficient_lng = 0.02;
        tire_damping_coefficient_lng = 0.25;
        tire_displacement_limit_lng = 0.15;
        radius = 0.3;
        width = 0.1;
        optimum_slip_percent = 0.14;
        static_friction = 2.5;
        sliding_friction = 2.2;
        normal_load = 5000.0f;
        suspension_damping_coefficient = 0.1;
        suspension_factor = 1.0f;
        suspension_limit = 0.33;
        tire_damping_coefficient_lat = 0.25;
        tire_displacement_limit_lat = 0.15;
        ics = NULL;
        vehicle = NULL;
        some_str = NULL;
        steering_offset = 0.0f;
        wobble_limit = 0.0f;
        axle_limit = 0.0f;
        some_position = vec3f({ 0.0f, 0.0f, 0.0f });
        some_handbrake_factor = 5000.0f;
        some_brake_factor = 5000.0f;
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

    const char* vehWheel::getFileType() {
        return "vehWheel";
    }

    void vehWheel::addNormalLoad(f32 load) {
        setNormalLoad(math::max(normal_load + load, 1.0f));
    }

    void vehWheel::setNormalLoad(f32 loadInKilograms) {
        normal_load = loadInKilograms;
        if (suspension_factor < 0.75f) suspension_factor = 0.75f;

        f32 fVar4 = 1.0 / (suspension_extent * (suspension_limit + suspension_extent));
        some_suspension_thing = (suspension_factor * suspension_extent + suspension_limit) * normal_load * fVar4;
        some_suspension_damping_factor = (suspension_damping_coefficient + suspension_damping_coefficient) * sqrtf(some_suspension_thing * normal_load);
        field_0x210 = ((suspension_factor - 1.0f) * normal_load * fVar4) / some_suspension_thing;
        
        some_handbrake_factor = handbrake_coefficient * normal_load * radius * static_friction;
        some_brake_factor     = brake_coefficient     * normal_load * radius * static_friction;

        some_tire_disp_lat_factor = (normal_load + normal_load) / tire_displacement_limit_lat;
        some_tire_disp_lng_factor = (normal_load + normal_load) / tire_displacement_limit_lng;

        f32 fVar1 = -normal_load / (vehicle->bound->gravityMultiplier * -9.8f);
        something_to_do_with_tire_damp_coef_lat_and_gravity = (tire_damping_coefficient_lat + tire_damping_coefficient_lat) * sqrtf(fVar1 * some_tire_disp_lat_factor);
        something_to_do_with_tire_damp_coef_lng_and_gravity = (tire_damping_coefficient_lng + tire_damping_coefficient_lng) * sqrtf(fVar1 * some_tire_disp_lng_factor);
    }

    void vehWheel::calcDispAndDamp(f32 param_1, f32 param_2, f32 param_3, f32* param_4, f32* param_5, i32* param_6) { 
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
    
    void vehWheel::calcSuspensionForce(f32 suspVal, f32 p2, f32 dispPerSec, bool overrideSuspVal) {
        is_bottomed_out = false;
        f32 last_sus_val = suspension_value;
        bool bVar4 = false;

        if (overrideSuspVal) {
            suspension_value = suspVal;
            if (suspVal < -suspension_extent) {
                suspension_value = -suspension_extent;
                dispPerSec = 0.0f;
                bVar4 = true;
            } else bVar4 = false;
        } else {
            bVar4 = true;
            suspension_value = -suspension_extent;
        }

        suspension_velocity = (suspension_value - last_sus_val) * datTimeManager::InvSeconds;
        f32 fVar9 = field_0x210 * suspension_value + 1.0f;
        field_0x200 = normal_load + (
            some_suspension_thing * suspension_value +
            some_suspension_damping_factor * dispPerSec +
            some_suspension_damping_factor * suspension_velocity
        ) * fVar9;
        
        if (field_0x200 < 0.0f) {
            field_0x208 = 0.0f;
            f32 fVar5 = some_suspension_thing * datTimeManager::Seconds;
            field_0x200 = 0.0f;
            suspension_value = (some_suspension_damping_factor * last_sus_val - suspension_extent * fVar5) / (fVar5 + some_suspension_damping_factor);
            suspension_velocity = (suspension_value - last_sus_val) * datTimeManager::InvSeconds;
        } else {
            f32 fVar5 = 0.0f;
            bool bVar3 = false;
            bool bVar2 = false;
            f32 fVar10 = 0.0f;
            if (suspension_limit < suspension_value) {
                f32 fVar6 = ics->calcCollisionNoFriction(field_0x178, world_transform_1.y, ground_contact_pt.contactPoint);
                bVar2 = fVar6 > 0.0f;
                if (fVar6 > 0.0f) {
                    fVar10 = fVar6 * 0.25f * datTimeManager::InvSeconds;
                    fVar5 = (-suspension_damping_coefficient * fVar10) / field_0x178;
                }

                constexpr i32 DAT_00363660 = 0;
                if (DAT_00363660 != 0) {
                    vec3f unk0;
                    math::mult(unk0, world_transform_1.y, (suspension_value - suspension_limit) * p2);
                    ics->calcNetPush(unk0, vec3f());
                }

                bVar4 = true;
                bVar3 = true;
                is_bottomed_out = true;
                suspension_value = suspension_limit;
            } else if (suspension_value < -suspension_extent) {
                suspension_value = -suspension_extent;
                bVar3 = true;
                bVar4 = true;
            }

            if (bVar3) {
                last_sus_val = (suspension_value - last_sus_val) * datTimeManager::InvSeconds;
                fVar9 = field_0x210 * suspension_value + 1.0f;
                suspension_velocity = last_sus_val;
                field_0x200 = normal_load + (some_suspension_thing * suspension_value + some_suspension_damping_factor * last_sus_val) * fVar9;
            }

            if (bVar4) field_0x208 = 0.0f;
            else field_0x208 = some_suspension_damping_factor * fVar9 + (some_suspension_thing * datTimeManager::Seconds) / p2;

            if (bVar2) {
                field_0x200 += fVar10;
                field_0x208 += fVar5;
            }
        }
    }

    f32 vehWheel::getBumpDisplacement(f32 unk) {
        if (!material || material->field_0x38 == 0.0f) return 0.0f;

        field_0x1e4 += unk * datTimeManager::Seconds * (math::frand() + 0.618f);
        if (material->width == 0.0f) field_0x1e4 = 0.0f;
        else field_0x1e4 -= (f32(i32(field_0x1e4 / material->width)) * material->width);

        return material->field_0x38 * sinf(((field_0x1e4 + field_0x1e4) * 3.141593f) / material->width);
    }

    void vehWheel::computeConstants() {
        bool bVar1;
        f32 fVar2;
        f32 fVar3;

        fVar2 = some_position.x;
        if (width < fabsf(fVar2)) {
            if (0.0f < fVar2) {
                fVar3 = 1.0f;
            }
            else {
                fVar3 = 0.0f;
                if (fVar2 < 0.0f) {
                    fVar3 = -1.0f;
                }
            }
            field_0x1bc = fVar3 * 0.5f * width;
        }
        else {
            field_0x1bc = 0.0f;
        }
        inv_optimum_slip_percent_squared = 1.0f / (optimum_slip_percent * optimum_slip_percent);

        if (vehicle == nullptr) {
            fVar2 = fabsf(some_position.z);
            bVar1 = 0.01f < fabsf(some_position.x);
            // Yes, this is in the decompilation. Not sure why they allowed the dereferencing of a null pointer
            // fVar2 = -(vehicle.collider.pfio.field_0x30)->gravity * -9.8f * ics->mass * fVar2) / (fVar2 + fVar2);
        }
        else {
            fVar2 = fabsf(some_position.z);
            bVar1 = width < fabsf(some_position.x);
            // fVar2 = (fabsf(field_0x1b8 - -vehicle.collider.center_of_mass.z) / (fVar2 + fVar2)) * -(vehicle.collider.pfio.field_0x30)->gravity * -9.8f * (vehicle.collider.pfio.solver)->mass;
        }

        if (bVar1) fVar2 = fVar2 * 0.5f;

        setNormalLoad(fVar2);
        return;
    }

    f32 vehWheel::computeFriction(f32 slip_percent, f32* friction) {
        f32 fVar1;
        f32 osp;
        f32 fVar2;
        f32 fVar3;

        fVar3 = fabsf(slip_percent);
        osp = optimum_slip_percent;
        fVar2 = material_friction * sliding_friction;
        fVar1 = material_friction * static_friction * inv_optimum_slip_percent_squared * fVar3 * ((osp + osp) - fVar3);

        if (fVar3 <= osp) {
            *friction = (fVar3 * 0.5f) / osp;
            return fVar1;
        }

        *friction = ((fVar3 - osp) * 0.5f) / (1.0f - osp) + 0.5f;

        if (fVar2 < fVar1) fVar2 = fVar1;

        return fVar2;
    }

    void vehWheel::computeGM() {
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

        fVar7 = radius * 0.2;
        fVar8 = (radius * 0.05 * field_0x200) / normal_load;
        fVar7 = fVar8 < fVar7 ? fVar8 : fVar7;
        fVar7 = suspension_value - (fVar7 > 0.0f ? fVar7 : 0.0f);
        (world_transform).w.x = (world_transform).w.x + fVar7 * (world_transform).y.x;
        fVar8 = (world_transform).w.z;
        (world_transform).w.y = (world_transform).w.y + fVar7 * (world_transform).y.y;
        (world_transform).w.z = fVar8 + fVar7 * (world_transform).y.z;

        if (0.0f < axle_limit) {
            if (has_intersection == 0) {
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

    void vehWheel::computeSlipPercent(f32* slip_percent, f32 param_3, f32 synchronous_speed) {
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
    
    void vehWheel::computeDwtdw(f32 p1, f32* out0, f32* out1, f32* out2) {
        mat3x4f* tform = vehicle ? &vehicle->instance->transform : &ics->world_transform;

        if (!has_intersection) {
            calcSuspensionForce(-suspension_extent, 0.0f, 0.0f, 0);
            *out0 = *out2 = 0.0f;

            f32 sign = 0.0f;
            if (p1 > 0.0f) sign = 1.0f;
            else if (p1 < 0.0f) sign = -1.0f;
            *out1 = -sign * 1e10f;

            math::copy(world_transform_1, world_transform);
            vec3f unk;
            math::mult(unk, world_transform_1.w, radius);
            math::sub(world_transform_1.w, unk);
            return;
        }

        f32 fVar20 = math::dot(ground_contact_pt.normal, tform->y);
        bool bVar6 = false;
        if (fVar20 <= 0.02f) {
            world_transform_1.w = ground_contact_pt.contactPoint;
            world_transform_1.y = ground_contact_pt.normal;
            math::cross(world_transform_1.z, world_transform.x, world_transform_1.y);

            f32 magSq = math::magnitudeSq(world_transform_1.z);
            if (magSq >= 0.02f) {
                f32 invMag = 1.0f / sqrtf(magSq);
                math::mult(world_transform_1.z, invMag);
                math::cross(world_transform_1.x, world_transform_1.y, world_transform_1.z);

                bVar6 = fabsf(ground_contact_pt.normal.y) < 0.001f;
            } else has_intersection = false;
        } else has_intersection = false;

        if (!has_intersection) {
            calcSuspensionForce(-suspension_extent, 0.0f, 0.0f, 0);
            *out0 = *out2 = 0.0f;

            f32 sign = 0.0f;
            if (p1 > 0.0f) sign = 1.0f;
            else if (p1 < 0.0f) sign = -1.0f;
            *out1 = -sign * 1e10f;

            math::copy(world_transform_1, world_transform);
            vec3f unk;
            math::mult(unk, world_transform_1.w, radius);
            math::sub(world_transform_1.w, unk);
            return;
        }

        vec3f velocity;
        vec3f velocity2;
        ics->getLocalFilteredVelocity2(ground_contact_pt.contactPoint, velocity);

        if (!ground_contact_pt.field9_0x2c && ground_contact_pt.someCellIdx) {
            phInst* inst = nullptr; // SpatialPartitioner::Instance->getPhInst(some_cell_idx);
            // inst->getLocalVelocity(ground_contact_pt.contactPoint, velocity2);
        } else {
            phInertialCS* ics = ground_contact_pt.field9_0x2c->ics;
            
            if (ics) ics->getLocalVelocity(ground_contact_pt.contactPoint, velocity2);
            else math::zero(velocity2);
        }

        math::sub(velocity, velocity2);

        field_0x170 = math::dot(world_transform_1.x, velocity);
        field_0x174 = math::dot(world_transform_1.z, velocity);
        field_0x178 = math::dot(world_transform_1.y, velocity);
        field_0x17c = field_0x174 + radius * some_rps;

        i32 iVar9 = 0;
        if (!ground_contact_pt.field10_0x30) material = phMaterialMgr::getDefaultMaterial();
        else {
            if (!field_0x12c) iVar9 = field_0x130;
            // else iVar9 = *(byte*)(field_0x127 + 0xc);

            if (iVar9 < 0) material = phMaterialMgr::getDefaultMaterial();
            else {
                // Too convoluted. needs more RE. See 0x00262980 -> 0x002629b0
            }
        }
        
        f32 dispPerSecond = 0.0f;
        if (!material) {
            bump_displacement = 0.0f;
            mtrl_width = 1.0f;
            mtrl_field_0x30 = 0.0f;
            material_friction = material->friction;
            field_0x1d8 = 0.0f;
            mtrl_field_0x38 = 0;
        } else {
            mtrl_field_0x30 = material->field_0x30;
            mtrl_field_0x38 = material->field_0x38;
            mtrl_width = material->width;

            f32 oldDisp = bump_displacement;
            f32 newDisp = getBumpDisplacement(sqrtf(field_0x170 * field_0x170 + field_0x174 * field_0x174));
            bump_displacement = newDisp;
            
            dispPerSecond = (newDisp - oldDisp) * datTimeManager::InvSeconds;

            f32 tenthOfArcLengthPerSec = fabsf(some_rps) * 0.1f * radius;
            if (field_0x1d8 < material->field_0x3c) {
                f32 x = field_0x1d8 + (tenthOfArcLengthPerSec * datTimeManager::Seconds);
                field_0x1d8 = material->field_0x3c < x ? material->field_0x3c : x;
            } else if (field_0x1d8 > material->field_0x3c) {
                f32 x = field_0x1d8 - (tenthOfArcLengthPerSec * datTimeManager::Seconds);
                field_0x1d8 = material->field_0x3c > x ? material->field_0x3c : x;
            }
        }

        if (bVar6) material_friction = 0.0f;

        if (vehicle && material_friction < 1.0f) {
            if (friction_handling < 1.0f) material_friction += (1.0f - friction_handling) * (1.0f - material_friction);
            else material_friction /= friction_handling;
        }

        material_friction *= vehWheel::WeatherFriction;

        calcSuspensionForce(
            calcSuspensionValue(ground_contact_pt.field4_0x20, math::dot(world_transform_1.z, world_transform.z)),
            fVar20, dispPerSecond, true
        );

        f32 a = radius * 0.2f;
        f32 b = (radius * 0.05f * field_0x200) / normal_load;

        vec3f verticalDisplacement;
        math::mult(verticalDisplacement, world_transform.y, (suspension_value - radius) - math::max(math::min(a, b), 0.0f));
        math::add(world_transform_1.w, verticalDisplacement);

        f32 radSq = radius * radius;
        f32 unk0 = radSq * (some_tire_disp_lng_factor + something_to_do_with_tire_damp_coef_lng_and_gravity * datTimeManager::InvSeconds);
        f32 unk1 = (((radSq / fabsf(field_0x174)) * (material_friction * static_friction)) / optimum_slip_percent) * field_0x200;

        bool bVar7 = true;
        if (fabsf(field_0x174) >= 1e07f && unk1 < unk0) {
            unk0 = unk1;

            f32 fVar16 = -field_0x174 / radius;
            f32 fVar20 = (1.0f - optimum_slip_percent) * fVar16;
            f32 fVar17 = (optimum_slip_percent + 1.0f) * fVar16;
            f32 fVar14 = (1.0f - math::max(math::min(optimum_slip_percent, slip_percent_lng), -optimum_slip_percent)) * fVar16;
            
            if (fVar17 < fVar20) {
                a = math::max(fVar14, fVar16);
                b = math::min(fVar14, fVar16);
            } else {
                a = math::min(fVar14, fVar16);
                b = math::max(fVar14, fVar16);
            }

            if (p1 < 0.0f) {
                if (field_0x174 > 0.0f) {
                    if (some_rps < b) {
                        *out1 = b;
                        *out2 = unk1;
                    }
                    else if (some_rps < fVar20) {
                        *out1 = fVar20;
                        *out2 = 0.0f;
                    } else {
                        *out1 = 1e+11f;
                        *out2 = unk1;
                    }
                } else if (some_rps < a) {
                    *out1 = a;
                    *out2 = unk1;
                } else {
                    if (some_rps < fVar17) {
                        *out1 = fVar17;
                        *out2 = 0.0f;
                    } else {
                        *out1 = 1e+11f;
                        *out2 = unk1;
                    }
                }
            } else {
                if (field_0x174 > 0.0f) {
                    if (some_rps <= a) {
                        if (fVar17 < fVar16) {
                            *out1 = fVar17;
                            *out2 = 0.0f;
                        } else {
                            *out1 = -1e+11f;
                            *out2 = unk1;
                        }
                    } else {
                        *out1 = a;
                        *out2 = unk1;
                    }
                } else if (b < some_rps) {
                    *out1 = b;
                    *out2 = unk1;
                } else {
                    if (fVar20 < some_rps) {
                        *out1 = fVar20;
                        *out2 = 0.0f;
                    } else {
                        *out1 = -1e+11f;
                        *out2 = unk1;
                    }
                }
            }
        } else {
            f32 unk2 = (field_0x200 * (material_friction * static_friction) * datTimeManager::InvSeconds) / (some_tire_disp_lng_factor * radius);
            if (p1 < 0.0f) {
                if (some_rps < -unk2) {
                    *out1 = -unk2;
                    *out2 = unk0;
                }
                else if (some_rps < unk2) {
                    *out1 = unk2;
                    *out2 = 0.0f;
                } else {
                    *out1 = 1e+08f;
                    *out2 = unk0;
                }
            } else if (some_rps < unk2) {
                if (some_rps > -unk2) {
                    *out1 = -unk2;
                    *out2 = 0.0f;
                } else {
                    *out1 = -1e+09f;
                    *out2 = unk0;
                }
            } else {
                *out1 = unk2;
                *out2 = unk0;
            }
        }

        if (*out2 != 0.0f) *out0 = 0.0f;
        else *out0 = unk0;
    }
    
    f32 vehWheel::calcSuspensionValue(f32 p1, f32 p2) {
        f32 padded_radius = radius + 0.5f;
        f32 limit = suspension_limit + 0.2f;
        f32 max = radius / p2;
        if (padded_radius > max) padded_radius = max;

        return (padded_radius + limit) - ((limit + suspension_extent + radius + 0.5f) * p1 + field_0x1d8);
    }

    void vehWheel::copyFrom(vehWheel* from) {
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
        radius = from->radius;
        width = from->width;
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

    f32 vehWheel::getVisualDispLat() {
        f32 min;
        min = tire_displacement_limit_lat < field_0x234 ? tire_displacement_limit_lat : field_0x234;
        return -tire_displacement_limit_lat > min ? -tire_displacement_limit_lat : min;
    }

    f32 vehWheel::getVisualDispLng() {
        f32 min;
        min = tire_displacement_limit_lng < field_0x238 ? tire_displacement_limit_lng : field_0x238;
        return -tire_displacement_limit_lng > min ? -tire_displacement_limit_lng : min;
    }

    f32 vehWheel::getVisualDispVert() {
        f32 fVar1;
        f32 fVar2;

        fVar2 = radius * 0.2f;
        fVar1 = (radius * 0.05f * field_0x200) / normal_load;
        fVar1 = fVar1 < fVar2 ? fVar1 : fVar2;
        return fVar1 > 0.0f ? fVar1 : 0.0f;
    }

    void vehWheel::setBrake(f32 param_1) {
        brake_factor_from_setbrake = param_1 * normal_load * radius * static_friction;
    }

    void vehWheel::setInputs(f32 param_1, f32 param_2, f32 param_3) {
        f32 fVar1;
        f32 fVar2;

        field_0x1f8 = -param_1 * steering_limit;
        if (0.0f < some_position.x) {
            fVar1 = 1.0f;
            fVar2 = steering_offset;
        }
        else {
            fVar1 = 0.0f;
            if (some_position.x < 0.0f) {
                fVar1 = -1.0f;
            }
            fVar2 = steering_offset;
        }
        brake_factor_from_setbrake = param_2 * some_brake_factor + param_3 * some_handbrake_factor;
        field_0x1f8 = field_0x1f8 * (1.0f - fVar1 * fVar2 * field_0x1f8);
        return;
    }

    void vehWheel::update() {
        
    }

    void vehWheel::reset() {
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
        brake_factor_from_setbrake = 0.0f;
        some_rps = 0.0f;
        field_0x1e4 = 0;
        field_0x234 = 0.0f;
        field_0x238 = 0.0f;
        field_0x240 = 0.0f;
        field_0x23c = 0.0f;
        field_0x200 = 0.0f;
        field_0x220 = 0.0f;
        field_0x228 = 0;
        has_intersection = 0;
        field_0x168 = 0;
        material_friction = 1.0f;
        field_0x17c = 0.0f;
        ground_contact_pt.instance = nullptr;
        bump_displacement = 0;
        mtrl_field_0x30 = 0.0f;
        field_0x1d8 = 0.0f;
        mtrl_field_0x38 = 0;
        mtrl_width = 0.0f;
        field_0x170 = 0.0f;
        field_0x174 = 0.0f;
        field_0x178 = 0.0f;
    }
};