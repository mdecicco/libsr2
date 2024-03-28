#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehCarSimBase;
    class vehWheel;

    class vehAxle : public parFileIO {
        public:
            vehAxle();
            ~vehAxle();

            void init(vehCarSimBase* veh, const char* modelname, const char* partname, vehWheel* wheel0, vehWheel* wheel1, f32 unk0);

            virtual datParserNode* prepParser(datParser* parser);
            virtual const char* getFileType();
            virtual void reset();
            virtual void update();
            void computeConstants();

            mat3x4f* unk_mat_0;
            mat3x4f world_transform;
            f32 torque_coefficient;
            f32 damping_coefficient;
            vehCarSimBase* vehicle;
            vehWheel* wheels[2];
            f32 field_0x54;
            f32 field_0x58;
            f32 field_0x5c;
            f32 field_0x60;
    };
};