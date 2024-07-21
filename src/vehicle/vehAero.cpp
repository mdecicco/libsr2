#include <libsr2/vehicle/vehAero.h>
#include <libsr2/vehicle/vehCarSimBase.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/io/datParser.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/managers/datTimeManager.h>

#include <math.h>

namespace sr2 {
    vehAero::vehAero() {
        angular_c_damping = { 2.0f, 1.0f, 2.0f };
        angular_velocity_damping = { 0.0f, 0.0f, 0.0f };
        angular_velocity_damping_2 = { 1.0f, 0.8f, 1.0f };
        drag = 0.8f;
        down = 2.5f;
        field_0x10 = 1;
        field_0xc = 1.0f;
    }

    vehAero::~vehAero() {
    }

    void vehAero::init(vehCarSimBase* veh) {
        vehicle = veh;
    }

    void vehAero::prepParser(datParser* parser) {
        parser->add(PARSE_TYPE::VEC3F, "AngCDamp", &angular_c_damping, 1, nullptr);
        parser->add(PARSE_TYPE::VEC3F, "AngVelDamp", &angular_velocity_damping, 1, nullptr);
        parser->add(PARSE_TYPE::VEC3F, "AngVel2Damp", &angular_velocity_damping_2, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "Drag", &drag, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "Down", &down, 1, nullptr);
    }

    const char* vehAero::getFileType() {
        return "vehAero";
    }

    void vehAero::reset() {
        field_0xc = 1.0f;
    }

    void vehAero::update() {
        phInertialCS *phys = vehicle->ics;

        if ((field_0x10 & 1) != 0) {
            if (field_0xc > 0.0f) {
                vec3f unk;
                math::mult_transposed(unk, phys->world_transform, phys->angular_velocity);

                vec3f absUnk = { fabsf(unk.x), fabsf(unk.y), fabsf(unk.z) };

                if (absUnk.x > 0.1f) {
                    f32 unk1 = 0.0f;

                    if (unk.x < 0.0f) unk1 = -1.0f;
                    else if (unk.x > 0.0f) unk1 = 1.0f;
                    else unk1 = 0.0f;

                    unk1 = (-unk1 * angular_c_damping.x - unk.x * angular_velocity_damping.x) - absUnk.x * unk.x * angular_velocity_damping_2.x;
                    if (absUnk.x < fabsf(unk1) * datTimeManager::Seconds) unk1 = -unk.x * datTimeManager::InvSeconds;
                    if (absUnk.x < 1.0f) unk1 = unk1 * absUnk.x;
                    if (field_0xc != 1.0f) unk1 = unk1 * field_0xc;

                    unk1 *= phys->ang_inertia.x;

                    vec3f force;
                    math::mult(force, phys->world_transform.x, unk1);
                    math::add(phys->torque, force);
                }

                if (absUnk.y > 0.1f) {
                    f32 unk1 = 0.0f;

                    if (unk.y < 0.0f) unk1 = 1.0f;
                    else if (unk.y > 0.0f) unk1 = -1.0f;
                    else unk1 = -0.0f;

                    unk1 = (unk1 * angular_c_damping.y - unk.y * angular_velocity_damping.y) - absUnk.y * unk.y * angular_velocity_damping_2.y;
                    if (absUnk.y < fabsf(unk1) * datTimeManager::Seconds) unk1 = -unk.y * datTimeManager::InvSeconds;
                    if (absUnk.y < 1.0) unk1 = unk1 * absUnk.y;
                    if (field_0xc != 1.0) unk1 = unk1 * field_0xc;

                    unk1 *= phys->ang_inertia.y;

                    vec3f force;
                    math::mult(force, phys->world_transform.y, unk1);
                    math::add(phys->torque, force);
                }

                if (absUnk.z > 0.1f) {
                    f32 unk1 = 0.0f;

                    if (unk.z < 0.0f) unk1 = 1.0f;
                    else if (unk.z > 0.0f) unk1 = -1.0f;
                    else unk1 = -0.0f;

                    unk1 = (unk1 * angular_c_damping.z - unk.z * angular_velocity_damping.z) - absUnk.z * unk.z * angular_velocity_damping_2.z;
                    if (absUnk.z < fabsf(unk1) * datTimeManager::Seconds) unk1 = -unk.z * datTimeManager::InvSeconds;
                    if (absUnk.z < 1.0f) unk1 = unk1 * absUnk.z;
                    if (field_0xc != 1.0f) unk1 = unk1 * field_0xc;

                    unk1 *= phys->ang_inertia.z;

                    vec3f force;
                    math::mult(force, phys->world_transform.z, unk1);
                    math::add(phys->torque, force);
                }
            }

            if (vehicle->velocity_mag > 15.0f) {
                vec3f force;
                math::mult(force, phys->world_transform.y, -down * vehicle->velocity_mag * vehicle->velocity_mag);

                i32 wheelCount = vehicle->getBottomedOutWheelCount();
                if (wheelCount != 0) {
                    math::mult(force, 1.0f - (f32(wheelCount) / f32(vehicle->wheel_count)));
                }

                math::add(phys->force, force);
            }

            if (vehicle->velocity_mag > 10.0f) {
                math::mult(phys->force, -drag * vehicle->velocity_mag);
            }
        }
    }
};