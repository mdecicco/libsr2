#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehCarSimBase;

    class vehAero : public parFileIO {
        public:
            vehAero();
            ~vehAero();

            // parFileIO
            virtual datParserNode* prep_parser(datParser* parser);
            virtual const char* file_type();

            // own
            virtual void reset();
            virtual void update();

            void init(vehCarSimBase* veh);

            f32 field_0xc;
            undefined4 field_0x10;
            vehCarSimBase* vehicle;
            vec3f angular_c_damping;
            vec3f angular_velocity_damping;
            vec3f angular_velocity_damping_2; // ?
            f32 drag;
            f32 down;
    };
};