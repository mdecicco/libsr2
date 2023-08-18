#include <libsr2/vehicle/vehAxle.h>
#include <libsr2/vehicle/vehWheel.h>
#include <libsr2/vehicle/vehCarSimBase.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/math.h>
#include <libsr2/io/datParser.h>
#include <libsr2/utilities/utils.h>
#include <math.h>

namespace sr2 {
    vehAxle::vehAxle() {
        vehicle = nullptr;
        wheels[0] = wheels[1] = nullptr;
        unk_mat_0 = nullptr;
        math::identity(world_transform);
        field_0x5c = 0.0f;
        field_0x58 = 1.0f;
        field_0x54 = 1.0f;
        field_0x60 = 0.0f;
        torque_coefficient = 0.0f;
        damping_coefficient = 0.0f;
    }

    vehAxle::~vehAxle() {
    }

    void vehAxle::init(vehCarSimBase* veh, char* modelname, char* partname, vehWheel* wheel0, vehWheel* wheel1, f32 unk0) {
        vehicle = veh;
        wheels[0] = wheel0;
        wheels[1] = wheel1;
        
        mat3x4f pivot;
        if (GetPivot(pivot, modelname, partname)) {
            unk_mat_0 = new mat3x4f;
            math::copy(*unk_mat_0, pivot);
        } else math::identity(pivot);

        vec3f unkv, unkv1;
        math::sub(unkv, wheels[0]->some_position, pivot.w);
        math::mult(unkv1, pivot.z, unkv);
        field_0x58 = 1.0f / (unkv1.x + unkv1.y + unkv1.z);

        math::mult(unkv1, pivot.x, unkv);
        field_0x54 = 1.0f / (unkv1.x + unkv1.y + unkv1.z);

        if (!unk_mat_0) math::copy(world_transform.z, vehicle->instance->transform.z);
        else math::mult(world_transform, *unk_mat_0, vehicle->instance->transform);
    }

    datParserNode* vehAxle::prep_parser(datParser* parser) {
        parser->add(PARSE_TYPE::FLOAT, "TorqueCoef", &torque_coefficient, 1, nullptr);
        return parser->add(PARSE_TYPE::FLOAT, "DampCoef", &damping_coefficient, 1, nullptr);
    }

    const char* vehAxle::file_type() {
        return "vehAxle";
    }

    void vehAxle::reset() {
    }

    void vehAxle::update() { 
        phInertialCS *ppVar2;

        f32 val0 = wheels[0]->suspension_value - math::max(0.0f, math::min(wheels[0]->radius * 0.2f, (wheels[0]->radius * 0.05f * wheels[0]->field_0x200) / wheels[0]->normal_load));
        f32 val1 = wheels[1]->suspension_value - math::max(0.0f, math::min(wheels[1]->radius * 0.2f, (wheels[1]->radius * 0.05f * wheels[1]->field_0x200) / wheels[1]->normal_load));
        f32 diff = val0 - val1;
        f32 halfDiff = diff * 0.5f;
        f32 angle = halfDiff * field_0x54;
        if (field_0x5c != 0.0) {
            ppVar2 = wheels[0]->ics;
            diff = -(diff * field_0x5c + (wheels[0]->suspension_velocity - wheels[1]->suspension_velocity) * field_0x60) * (ppVar2->ang_inertia).z;

            vec3f force;
            math::mult(force, wheels[0]->ics->world_transform.z, diff);
            math::add(wheels[0]->ics->torque, force);
        }

        if (!unk_mat_0) world_transform.z = vehicle->instance->transform.z;
        else {
            unk_mat_0->z.y = (val0 + val1) * 0.5f * field_0x58;
            unk_mat_0->x.y = halfDiff * field_0x54;
            
            math::mult(world_transform, *unk_mat_0, vehicle->instance->transform);
        }
        
        if (wheels[0]->camber_limit < 0.0f || wheels[1]->camber_limit < 0.0f) {
            math::rotate(wheels[0]->world_transform, world_transform.z, angle);
            math::rotate(wheels[1]->world_transform, world_transform.z, angle);
        } else {
            if (wheels[0]->camber_limit <= 0.0f && wheels[1]->camber_limit <= 0.0f) return;

            math::rotate(wheels[0]->world_transform, world_transform.z, wheels[0]->field_0x21c);
            math::rotate(wheels[1]->world_transform, world_transform.z, wheels[1]->field_0x21c);
        }
    }

    void vehAxle::computeConstants() {
        field_0x5c = torque_coefficient;
        field_0x60 = (damping_coefficient + damping_coefficient) * sqrtf(torque_coefficient);
    }
};