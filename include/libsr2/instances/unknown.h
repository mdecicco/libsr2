#pragma once
#include <libsr2/types.h>
#include <libsr2/instances/PhysInstance.h>
#include <libsr2/unknown/unknown.h>

#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    class Cls_003a2f30 : public PhysInstance {
        public:
            Cls_003a2f30() {
                flags &= 0xfff9 | 0x401;
            }
    };

    class Cls_003a2e40 : public Cls_003a2f30 {
        public:
    };

    class Cls_00393768 : public Cls_003a2e40 {
        public:
            Cls_00393768() {
                math::identity(unk_mat);
                FUN_001964b0(1);
            }

            void FUN_001964b0(bool unk) {
                if (unk) flags |= 0x10;
                else flags &= 0xffef;
            }
            mat3x4f unk_mat;
    };

    class Cls_00395078 : public Cls_00393768 {
        public:
            f32 unk0;
            undefined4 unk1;
            f32 unk2;
            undefined4 unk3;
            Cls_0039a0f8 unk4;
            undefined4 unk5;

            void FUN_001a6890(undefined4 unk) {
                unk4.unk3 = 1;
                unk5 = unk;
                flags |= 8;
            }
    };
    
    class Cls_00394e58 : public Cls_00393768 {
        public:
    };
};