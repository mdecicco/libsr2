#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class dgLinkData {
        public:
            dgLinkData();
            ~dgLinkData();

            void getXAxis(vec3f& out);

            quat_transform* unk0;
            quat_transform* unk1;
            quat_transform init;
            quat_transform rotated;
            quat_transform unk2;
            quat_transform center;
            vec3f unk3;
            vec3f velocity;
            bool unk5;
    };
};