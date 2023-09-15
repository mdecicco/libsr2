#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phBound;
    class phInst;
    class phInertialCS;
    class phSleep;

    class PhysEntity {
        public:
            PhysEntity();
            ~PhysEntity();

            void setFlag(u32 flag, bool state);
            void setOtherFlag(u32 flag, bool state);

            phBound* bound;
            u32 flags;
            u32 otherFlags;
            f32 mass;
            f32 invMass;
            vec3f angInertia;
            vec3f invAngInertia;

            f32 unk0;
            undefined* unk1;
            vec3f unk2;
    };
};