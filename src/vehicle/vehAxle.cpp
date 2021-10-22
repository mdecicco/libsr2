#include <libsr2/vehicle/vehAxle.h>
#include <libsr2/vehicle/vehWheel.h>
#include <libsr2/vehicle/vehCarSim.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/io/datParser.h>

namespace sr2 {
    vehAxle::vehAxle() {
        vehicle = nullptr;
        wheel_0 = wheel_1 = nullptr;
        unk_mat_0 = nullptr;
        math::identity(unk_mat_1);
        field_0x5c = 0.0f;
        field_0x58 = 1.0f;
        field_0x54 = 1.0f;
        field_0x60 = 0.0f;
        torque_coefficient = 0.0f;
        damping_coefficient = 0.0f;
    }

    vehAxle::~vehAxle() {
    }

    void vehAxle::Init(vehCarSim* _vehicle, char* modelname, char* partname, vehWheel* wheel0, vehWheel* wheel1, f32 unk0) {
        vehicle = _vehicle;
        wheel_0 = wheel0;
        wheel_1 = wheel1;
        
        mat3x4f pivot;
        bool loaded = false; // GetPivot(pivot, modelname, partname);
        if (!loaded) {
            math::identity(pivot);
        } else {
            unk_mat_0 = new mat3x4f;
            math::copy(*unk_mat_0, pivot);
        }

        vec3f unkv, unkv1;
        math::sub(unkv, wheel_0->field_0x1b0, *(vec3f*)&pivot.w);
        math::mult(unkv1, *(vec3f*)&pivot.z, unkv);
        field_0x58 = 1.0f / (unkv1.x + unkv1.y + unkv1.z);

        math::mult(unkv1, *(vec3f*)&pivot.x, unkv);
        field_0x54 = 1.0f / (unkv1.x + unkv1.y + unkv1.z);

        if (!unk_mat_0) {
            // it's unclear if this is actually what's going on
            // unk_mat_1.x = vehicle->veh.sim.unk_matrix_0.z.x;
            // unk_mat_1.y = vehicle->veh.sim.unk_matrix_0.z.y;
            // unk_mat_1.z = vehicle->veh.sim.unk_matrix_0.z.z;
        } else {
            //math::mult(unk_mat_1, *unk_mat_0, vehicle->veh.sim.unk_matrix_0)
        }
    }

    datParserNode* vehAxle::prep_parser(datParser* parser) {
        parser->add(PARSE_TYPE::FLOAT, "TorqueCoef", &torque_coefficient, 1, nullptr);
        return parser->add(PARSE_TYPE::FLOAT, "DampCoef", &damping_coefficient, 1, nullptr);
    }

    const char* vehAxle::file_type() {
        return "vehAxle";
    }
};