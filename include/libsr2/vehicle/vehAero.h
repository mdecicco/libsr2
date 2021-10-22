#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehCarSim;

    class vehAero : public parFileIO {
        public:
            vehAero();
            ~vehAero();

            virtual datParserNode* prep_parser(datParser* parser);
            virtual const char* file_type();

            f32 field_0xc;
            undefined4 field_0x10;
            vehCarSim* vehicle;
            vec3f angular_c_damping;
            vec3f angular_velocity_damping;
            vec3f angular_velocity_damping_2; // ?
            f32 drag;
            f32 down;
    };
};