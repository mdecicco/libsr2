#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class TaggedStream;

    struct UnkMissionObj0 {
        UnkMissionObj0();
        void setUnk2(undefined4 value);

        f32 unk0;
        undefined4 unk1;
        undefined4 unk2;
    };

    class Mission {
        public:
            Mission();
            virtual ~Mission();

            undefined4 b_unk0;
            UnkMissionObj0* b_unk1;
            struct {
                vec3f position;
                f32 rot_y;
            } vehicle_positions[16];
            undefined4 b_unk3[16];
            f32 b_unk4[16];
            undefined4 b_unk5;
            char b_unk6[32];

            virtual bool load(TaggedStream* tsv) = 0;
            virtual void method_0x40() = 0;
            virtual void method_0x50() = 0;
            virtual void onImpact(f32 p1, f32 p2, u8 p3, const mat3x4f& p4);
            void FUN_0019c698();


            void debugPrintBaseState();
            virtual void debugPrintState();
    };

};