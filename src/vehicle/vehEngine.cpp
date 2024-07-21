#include <libsr2/vehicle/vehEngine.h>
#include <libsr2/vehicle/vehDrivetrain.h>
#include <libsr2/vehicle/vehTransmission.h>
#include <libsr2/vehicle/vehCarSim.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/math.h>
#include <libsr2/io/datParser.h>

#include <math.h>

namespace sr2 {
    vehEngine::vehEngine() {
        drivetrains = nullptr;
        transmission = nullptr;
        ics = nullptr;
        instance = nullptr;
        pivot_transform = nullptr;
        instance_transform = nullptr;

        torque_at_opt_rps = 0.0f;
        idle_rpm = 750.0f;
        opt_rpm = 5000.0f;
        max_rpm = 8000.0f;
        idle_rps = idle_rpm * math::RPM_TO_RPS;
        opt_rps = opt_rpm * math::RPM_TO_RPS;
        max_rps = max_rpm * math::RPM_TO_RPS;

        gear_change_duration = 0.25f;

        fib = math::FIBONACCI_NUMBER;
        one_minus_fib = 1.0f - math::FIBONACCI_NUMBER;

        boost_duration = 2.0f;
        boost_horsepower = 500.0f;
        max_horsepower = 500.0f;
        horsepower_scale = 1.0f;
        angular_inertia = 1.0f;

        reset();
    }

    vehEngine::~vehEngine() {
    }

    void vehEngine::reset() { 
        changing_gears = true;
        unk_rpm_0_0 = idle_rpm;
        gear_change_time = gear_change_duration;
        field7_0x24 = 1.0f;
        rps = 0.0f;
        rpm = idle_rpm;
        field26_0x6c = 0.0f;
        torque = 0.0f;
        field21_0x5a = 0;
        torque_lerp_factor = 0.0f;
        boost_remaining = 0.0f;
        horsepower_scale = 1.0f;
        jiggle_angle = 0.0f;
        
        if (drivetrains) {
            // Attach(drivetrains);
            // drivetrains->field14_0x34 = 0.0f;
        }
    }

    void vehEngine::init(vehCarSimBase* veh, const char* modelname, const char* partname) {
        bool loaded = false;

        // ics = (veh->veh).sim.col.ics;
        // instance = (veh->veh).sim.col.instance;
        if (modelname && partname) {
            // loaded = GetPivot(&pivot, modelname, partname);
            if (loaded) {
                if (!pivot_transform) {
                    pivot_transform = new mat3x4f;
                    instance_transform = new mat3x4f;
                }

                // math::copy(*instance_transform, pivot);
                // math::copy(*pivot_transform, pivot);
            }
        }
        
        if (!loaded) {
            // instance_transform = &(veh->veh).sim.col.instance->transform;
        }
        
        reset();
        computeConstants();
    }
    
    f32 vehEngine::getMaxHorsepower() {
        if (boost_remaining > 0.0f) return max_horsepower + boost_horsepower;
        return max_horsepower;
    }

    void vehEngine::computeConstants() {
        f32 rps_diff;
        
        max_rps = max_rpm * math::RPM_TO_RPS;
        opt_rps = opt_rpm * math::RPM_TO_RPS;
        idle_rps = idle_rpm * math::RPM_TO_RPS;

        rps_diff = max_rps - opt_rps;
        inv_opt_rps_diff_sq = 1.0f / (rps_diff * rps_diff);
        
        torque_at_opt_rps = calcTorqueAtFullThrottle(opt_rps);
        unk_computed_1 = 746.0f / (opt_rps * opt_rps * opt_rps);
        if (!transmission) {
            // vehTransmission::computeConstants(transmission);
        }
    }

    f32 vehEngine::calcTorqueAtFullThrottle(f32 radiansPerSec) {
        f32 max_hp = getMaxHorsepower();

        f32 term0 = (fib * opt_rps - radiansPerSec);
        f32 term1 = (one_minus_fib * opt_rps + radiansPerSec);
        f32 unk0 = horsepower_scale * max_hp * unk_computed_1 * term0 * term1;

        if (radiansPerSec <= opt_rps) return unk0;


        f32 result = 0.0f;
        if (radiansPerSec <= max_rps) {
            result = unk0 * (max_rps - radiansPerSec) * ((radiansPerSec + max_rps) - (opt_rps + opt_rps)) * inv_opt_rps_diff_sq;
            if (result < 0.0f) result = 0.0f;
        }

        return result;
    }

    f32 vehEngine::calcTorqueAtZeroThrottle() {
        f32 idleRpmFacDiff = idle_rps - rps;
        f32 optRpmFacDiff = opt_rps - idle_rps;
        f32 unknown = (horsepower_scale * getMaxHorsepower() * 746.0f) / opt_rps;
        return (unknown * 0.75f * idleRpmFacDiff) / optRpmFacDiff;
    }

    void vehEngine::startBoosting(f32 duration) {
        if (duration <= 0.0f) boost_remaining = boost_duration;
        else boost_remaining = duration;
    }

    void vehEngine::stopBoosting() {
        boost_remaining = 0.0f;
        // transmission->clutch = 0;
    }

    void vehEngine::update() {
        if (transmission->field5_0x14 && !changing_gears) {
            unk_rpm_0_0 = rpm;
            gear_change_time = gear_change_duration;
            changing_gears = true;
        }

        f32 maxTorque = calcTorqueAtFullThrottle(rps);
        f32 minTorque = calcTorqueAtZeroThrottle();
        torque = torque_lerp_factor * maxTorque + (1.0f - torque_lerp_factor) * minTorque;

        f32 gearRatio = 0.0f;
        if (transmission->mode == tm_manual) gearRatio = transmission->manual_gear_ratios[transmission->current_gear];
        else gearRatio = transmission->auto_gear_ratios[transmission->current_gear];

        if (gearRatio == 0.0 || field21_0x5a || transmission->clutch) {
            if (drivetrains->engine) drivetrains->detach();
        } else {
            bool cond0 = fabsf(drivetrains->some_rps * gearRatio) < fabsf(idle_rps * 0.75f);
            bool cond1 = !transmission->current_gear || transmission->current_gear == 2;
            if (cond0 && cond1 && torque_lerp_factor < 0.1f) {
                if (drivetrains->engine) drivetrains->detach();
            } else if (gearRatio != 0.0) {
                if (!transmission->clutch) {
                    cond0 = rps <= idle_rps * 1.25f;
                    cond1 = fabsf(drivetrains->some_rps * gearRatio) <= fabsf(idle_rps * 1.25f);
                    if ((!cond0 || !cond1) && !drivetrains->engine) drivetrains->attach();
                }
            }
        }
    
        if (!drivetrains->engine) {
            if (field21_0x5a == 0) {
                rps += (torque / angular_inertia) * datTimeManager::Seconds;

                if (rps > max_rps) rps = max_rps;
                else if (rps < 0.0f) rps = 0.0f;
            } else {
                rps = 0.0f;
                torque = 0.0f;
            }
        }

        rpm = rps * math::RPS_TO_RPM;
        if (changing_gears) {
            if (gear_change_time > 0.0f) {
                torque = 0.0f;

                f32 gearChangeTimeElapsed = gear_change_duration - gear_change_time;
                f32 newRpm = rps * math::RPS_TO_RPM;
                rpm = (gearChangeTimeElapsed * newRpm + gear_change_time * unk_rpm_0_0) / gear_change_duration;
                gear_change_time -= datTimeManager::Seconds;
            } else {
                changing_gears = 0;
                transmission->setGearChangeFlag(0);
                rps = rpm * math::RPM_TO_RPS;
            }
        }

        field26_0x6c = torque * rps * math::NMS_TO_HP;
        if (pivot_transform && instance) {
            f32 unk = angular_inertia * (torque / torque_at_opt_rps) * 0.05f;

            if (jiggle_angle < unk) {
                jiggle_angle += datTimeManager::Seconds * 0.5f;
                if (jiggle_angle > unk) jiggle_angle = unk;
            } else if (jiggle_angle > unk) {
                jiggle_angle -= datTimeManager::Seconds * 0.5f;
                if (jiggle_angle < unk) jiggle_angle = unk;
            }

            if (field21_0x5a == 0) {
                jiggle_angle += math::frand() * (math::DEG_TO_RAD * 2.0f) - math::DEG_TO_RAD;
            }

            mat3x4f rotation;
            math::copy(rotation, *pivot_transform);
            math::rotate(rotation, pivot_transform->z, jiggle_angle);
            math::mult(*instance_transform, rotation, instance->transform);
        }

        if (transmission->current_gear || transmission->clutch) {
            f32 unk = -angular_inertia * angular_inertia * (torque / torque_at_opt_rps) * (ics->ang_inertia).z;
            ics->torque.x += unk * ics->world_transform.z.x;
            ics->torque.y += unk * ics->world_transform.z.y;
            ics->torque.z += unk * ics->world_transform.z.z;
        }

        if (boost_remaining > 0.0f) {
            boost_remaining -= datTimeManager::Seconds;
            if (boost_remaining <= 0.0f) stopBoosting();
        }
    }

    void vehEngine::prepParser(datParser* parser) {
        parser->add(PARSE_TYPE::FLOAT, "AngInertia", &angular_inertia, 1, nullptr);                                                                                  
        parser->add(PARSE_TYPE::FLOAT, "MaxHorsePower", &max_horsepower, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "IdleRPM", &idle_rpm, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "OptRPM", &opt_rpm, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "MaxRPM", &max_rpm, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "GCL", &gear_change_duration, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "BoostDuration", &boost_duration, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "BoostHP", &boost_horsepower, 1, nullptr);
    }

    const char* vehEngine::getFileType() {
        return "vehEngine";
    }
};