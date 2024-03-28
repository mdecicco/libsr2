#include <libsr2/vehicle/vehDrivetrain.h>
#include <libsr2/vehicle/vehCarSimBase.h>
#include <libsr2/vehicle/vehEngine.h>
#include <libsr2/vehicle/vehWheel.h>
#include <libsr2/vehicle/vehTransmission.h>
#include <libsr2/managers/datTimeManager.h>
#include <math.h>

namespace sr2 {
    f32 vehDrivetrain::diffRatioMax = 1.25f;
    f32 vehDrivetrain::diffRatioMaxHighSpeed = 1.03f;
    f32 vehDrivetrain::diffRatioHighSpeedLevel = 50.0f;
    f32 vehDrivetrain::dRHSLinV = 0.02f;

    vehDrivetrain::vehDrivetrain() {
        vehicle = nullptr;
        engine = nullptr;
        transmission = nullptr;
        wheel_count = 0;
        angular_inertia = 50.0f;
        brake_damping_coefficient = 1.0f;
        brake_static_coefficient = 1.2f;

        reset();
    }

    vehDrivetrain::~vehDrivetrain() {

    }

    void vehDrivetrain::init(vehCarSimBase* veh, vehTransmission* trans, vehEngine* eng) {
        vehicle = veh;
        transmission = trans;
        engine = eng;
    }

    void vehDrivetrain::reset() {
        some_rps = 0.0f;
        field7_0x38 = 1.0f;
    }
    
    void vehDrivetrain::attach() {
        engine = vehicle->engine;
    }

    void vehDrivetrain::detach() {
        engine = nullptr;
    }

    bool vehDrivetrain::addWheel(vehWheel* wheel) {
        if (wheel_count < 6) {
            wheels[wheel_count++] = wheel;
            return true;
        }

        return false;
    }

    void vehDrivetrain::update() {
        // todo: this ABSOLUTELY needs to be revised / validated...
        // Function starts at 0x0025d520

        f32 gearRatio = 0.0f;
        if (engine && transmission) gearRatio = transmission->getCurrentGearRatio();

        // This block outputs brake_value
        f32 brake_value = 50.0f;
        {
            if (some_rps == 0.0f) {
                for (u32 i = 0;i < wheel_count;i++) brake_value += brake_static_coefficient * wheels[i]->brake_factor_from_setbrake;
            } else {
                for (u32 i = 0;i < wheel_count;i++) brake_value += brake_damping_coefficient * wheels[i]->brake_factor_from_setbrake;
            }
        }

        // This block outputs unk1
        f32 unk1 = 0.0f;
        {
            if (engine) {
                unk1 = engine->torque * gearRatio; // kg*m^2
                unk1 += engine->angular_inertia * gearRatio * (engine->rps + some_rps * gearRatio) * datTimeManager::InvSeconds; // m^2*kg/s? (aka. Joule)
            }

            for (u32 i = 0;i < wheel_count;i++) {
                unk1 -= wheels[i]->field_0x244;
            }
        }
        
        // This block outputs unk2
        // This block updates unk1
        bool unk2 = false;
        {
            if (some_rps == 0.0f) {
                if (unk1 >= 0.0f) unk1 = unk1 > brake_value ? unk1 : brake_value;
                else unk1 = unk1 > brake_value ? brake_value : unk1;
                unk2 = unk1 == 0.0f;
            } else {
                f32 fVar19 = 0.0f;
                if (some_rps > 0.0f) fVar19 = 1.0f;
                else {
                    fVar19 = 0.0f;
                    if (some_rps < 0.0f) fVar19 = -1.0f;
                }
                unk2 = fabsf(unk1) <= fabsf(brake_value * fVar19);
                unk1 += brake_value * fVar19;
            }
        }

        f32 fVar26 = 0.0f;
        if (!engine || !transmission) fVar26 = 10.0f;
        else fVar26 = engine->angular_inertia * gearRatio * gearRatio + 0.02f;

        f32 local_c0[8];
        f32 local_e0[8];
        f32 local_100[8];
        for (u32 i = 0;i < wheel_count;i++) {
            wheels[i]->computeDwtdw(unk1, &local_100[i], &local_e0[i], &local_c0[i]);
        }

        f32 fVar19 = -unk1;
        f32 fVar27 = 1.0f / field7_0x38;
        if (diffRatioMax > 1.0f) {
            if (wheel_count >= 2 && (some_rps >= 0.001f || some_rps <= -0.001f)) {
                f32 fVar25 = fabsf(some_rps);
                f32 fVar21 = diffRatioMaxHighSpeed;
                if (fVar25 < diffRatioHighSpeedLevel) {
                    fVar21 = ((diffRatioHighSpeedLevel - fVar25) * diffRatioMax + fVar25 * diffRatioMaxHighSpeed) * dRHSLinV;
                }
                f32 fVar22 = 0.0f;
                f32 fVar28 = 1.0f / fVar21;
                fVar25 = 0.0f;

                for (u32 i = 0;i < wheel_count - 1;i += 2) {
                    fVar22 += local_100[i] + local_100[i + 1];
                    fVar25 += wheels[i]->field_0x244 - wheels[i + 1]->field_0x244;
                }
                
                fVar27 = field7_0x38 + fVar25 / (some_rps * (fVar22 + angular_inertia));
                if ((fVar27 <= fVar21) && (fVar21 = fVar27, fVar27 < fVar28)) {
                    fVar21 = fVar28;
                }

                fVar21 = (field7_0x38 * 9.0 + fVar21) * 0.1;
                field7_0x38 = fVar21;
                fVar27 = 1.0f / fVar21;
                fVar19 -= (field7_0x38 - fVar21) * some_rps * fVar22;

                for (u32 i = 0;i < wheel_count - 1;i += 2) {
                    local_100[i] *= fVar21;
                    local_100[i + 1] *= fVar27;
                    local_c0[i] *= fVar21;
                    local_c0[i + 1] *= fVar27;
                    local_e0[i] *= fVar27;
                    local_e0[i + 1] *= fVar21;
                }
            } else {
                if (wheel_count >= 2) fVar27 = some_rps;
                fVar27 = 1.0;
                field7_0x38 = 1.0;
            }
        }

        f32 fVar23 = angular_inertia;
        for (u32 i = 0;i < wheel_count;i++) {
            fVar23 += local_100[i];
        }

        i32 iVar14 = -1;
        f32 fVar21 = 0.0f;
        f32 fVar25 = some_rps;
        for (u32 i = 0;i <= wheel_count;i++) {
            f32 fVar22 = 0.0f;
            fVar21 = (fVar19 / (fVar26 + fVar23 * datTimeManager::Seconds)) * datTimeManager::Seconds + fVar25;
            
            if (unk1 < 0.0f) {
                fVar22 = 1e+09f;
                f32 fVar28 = 1e+09f;
                i32 iVar12 = iVar14;
                for (u32 j = 0;j < wheel_count;j++) {
                    fVar22 = local_e0[j];
                    iVar14 = j;
                    if (fVar28 <= local_e0[j]) {
                        fVar22 = fVar28;
                        iVar14 = iVar12;
                    }
                    fVar28 = fVar22;
                    iVar12 = iVar14;
                }

                if (fVar21 <= fVar22) break;
                local_e0[iVar14] = 1e+11f;
            } else {
                fVar22 = -1e+10f;
                f32 fVar28 = -1e+10f;
                i32 iVar12 = iVar14;
                for (u32 j = 0;j < wheel_count;j++) {
                    fVar22 = local_e0[j];
                    iVar14 = j;
                    if (fVar22 <= fVar28) {
                        fVar22 = fVar28;
                        iVar14 = iVar12;
                    }
                    fVar28 = fVar22;
                    iVar12 = iVar14;
                }

                if (fVar22 <= fVar21) break;
                local_e0[iVar14] = -1e+11f;
            }
            fVar19 = fVar19 - (fVar22 - fVar25) * fVar23;
            fVar23 = fVar23 + -local_100[iVar14] + local_c0[iVar14];
            fVar25 = fVar22;
        }

        if (unk2) {
            if (fVar21 < 0.0f && some_rps > 0.0f) fVar21 = 0.0f;
            else if (some_rps < 0.0f && fVar21 > 0.0f) fVar21 = 0.0f;
        }

        some_rps = fVar21;

        if (engine) {
            f32 someRps = engine->idle_rps * 0.5f;
            f32 someOtherRps = -some_rps * gearRatio;

            if (someOtherRps >= someRps) {
                if (engine->max_rps < someOtherRps) some_rps = -engine->max_rps / gearRatio;
                else someRps = someOtherRps;
            }

            if (engine->rps < someRps) {
                someOtherRps = engine->rps + engine->opt_rps * datTimeManager::Seconds;
                engine->rps = someRps < someOtherRps ? someRps : someOtherRps;
            } else if (engine->rps > someRps) {
                someOtherRps = engine->rps - engine->opt_rps * datTimeManager::Seconds;
                engine->rps = someRps < someOtherRps ? someOtherRps : someRps;
            }
        }

        for (u32 i = 0;i < wheel_count - 1;i += 2) {
            wheels[i]->some_rps = some_rps * field7_0x38;
            wheels[i + 1]->some_rps = some_rps * fVar27;
        }

        if (wheel_count != 0) {
            wheels[wheel_count - 1]->some_rps = some_rps;
        }
    }
};