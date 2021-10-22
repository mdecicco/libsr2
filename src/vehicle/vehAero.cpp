#include <libsr2/vehicle/vehAero.h>
#include <libsr2/io/datParser.h>

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

    datParserNode* vehAero::prep_parser(datParser* parser) {
        parser->add(PARSE_TYPE::VEC3F, "AngCDamp", &angular_c_damping, 1, nullptr);
        parser->add(PARSE_TYPE::VEC3F, "AngVelDamp", &angular_velocity_damping, 1, nullptr);
        parser->add(PARSE_TYPE::VEC3F, "AngVel2Damp", &angular_velocity_damping_2, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "Drag", &drag, 1, nullptr);
        return parser->add(PARSE_TYPE::FLOAT, "Down", &down, 1, nullptr);
    }

    const char* vehAero::file_type() {
        return "vehAero";
    }
};