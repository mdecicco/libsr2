#include <libsr2/vehicle/vehFluid.h>
#include <libsr2/vehicle/vehCarSimBase.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/math/math.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/io/datParser.h>

namespace sr2 {
    vehFluid::vehFluid() {
        flags = 0;
        field1_0x4 = 0;
        field3_0xc = { 0.0f, 0.0f, 0.0f };
        vehicle = nullptr;
        damping = 0.02;
        buoyancy = 0.5;
        current = 2.0;
        reset();
    }

    vehFluid::~vehFluid() {
    }

    void vehFluid::reset() {
        flags &= 0xfffffffe;
    }
    
    void vehFluid::init(vehCarSimBase* veh, const vec3f& min, const vec3f& max) {
        vehicle = veh;

        for (u32 i = 0;i < 4;i++) {
            for (u32 j = 0;j < 4;j++) {
                for (u32 k = 0;k < 4;k++) {
                    vec3f& cell = grid[i][j][k];
                    /*
                     * This is actually in the decompilation... But the resulting value is immediately overwritten
                     *
                        cell = {
                            math::frand() - 0.5f,
                            math::frand() - 0.5f,
                            math::frand() - 0.5f
                        };
                        math::normalize(cell);
                        math::mult(cell, 1.5f);
                     */
                    cell = max;
                    math::sub(cell, min);
                    math::mult(cell, { f32(i) / 3.0f, f32(j) / 3.0f, f32(k) / 3.0f });
                    math::add(cell, min);
                }
            }
        }
    }

    void vehFluid::update() {
        if ((flags & 1) == 0) return;

        phInertialCS* ics = vehicle->ics;
        vec3f& com = vehicle->center_of_mass;           
        mat3x4f& it = vehicle->instance->transform;    

        for (u32 i = 0;i < 4;i++) {
            for (u32 j = 0;j < 4;j++) {
                for (u32 k = 0;k < 4;k++) {
                    vec3f& cell = grid[i][j][k];

                    vec3f a;
                    math::mult(a, vehicle->instance->transform, cell);

                    if (a.y <= field1_0x4) {
                        vec3f d;
                        math::sub(d, a, ics->world_transform.w);

                        vec3f unk;
                        math::cross(unk, ics->angular_velocity, d);
                        math::add(unk, ics->world_velocity);
                        math::sub(unk, field3_0xc);
                        unk = {
                            0.0f                 - damping * unk.x * ics->mass,
                            buoyancy * ics->mass - damping * unk.y * ics->mass,
                            0.0f                 - damping * unk.z * ics->mass
                        };

                        math::add(ics->force, unk);           

                        vec3f torque;
                        math::mult(torque, vehicle->instance->transform, vehicle->center_of_mass);
                        math::cross(torque, torque, unk);
                        math::add(ics->torque, torque);
                    }
                }
            }
        }
    }

    void vehFluid::prepParser(datParser* parser) {
        parser->add(PARSE_TYPE::FLOAT, "Damp", &damping, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "Buoyancy", &buoyancy, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "Current", &current, 1, nullptr);
    }

    u32 vehFluid::FUN_0025fa58() {
        return flags & 1;
    }

    void vehFluid::FUN_0025fa68() {
        flags |= 1;
    }
    
    void vehFluid::FUN_0025fa78() {
        flags &= 0xfffffffe;
    }

    void vehFluid::FUN_0025fa90(const vec3f& unk) {
        math::mult(field3_0xc, unk, current);
    }
};