#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Unknown0;

    class phInst {
        public:
            phInst();
            virtual ~phInst();

            virtual mat3x4f* method_0x10();
            virtual void getLocalVelocity(vec3f& p1, vec3f& p2);
            virtual i32 method_0x20();
            virtual i32 method_0x28();
            virtual void method_0x30() = 0;

            mat3x4f transform;
            Unknown0* unk;
    };
};