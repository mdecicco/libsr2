#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehCarSim;
    class vehWheel;

    class vehAxle : public parFileIO {
        public:
            vehAxle();
            ~vehAxle();

            void Init(vehCarSim* vehicle, char* modelname, char* partname, vehWheel* wheel0, vehWheel* wheel1, f32 unk0);

            virtual datParserNode* prep_parser(datParser* parser);
            virtual const char* file_type();

            mat3x4f* unk_mat_0;
            mat3x4f unk_mat_1;
            f32 torque_coefficient;
            f32 damping_coefficient;
            vehCarSim* vehicle;
            vehWheel* wheel_0;
            vehWheel* wheel_1;
            f32 field_0x54;
            f32 field_0x58;
            f32 field_0x5c;
            f32 field_0x60;
    };
};