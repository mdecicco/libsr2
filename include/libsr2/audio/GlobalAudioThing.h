#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class SomeAudioThing6;

    class GlobalAudioThing {
        public:
            struct Unk0 {
                undefined4 field_0x0;
                f32 field_0x4;
                undefined4 field_0x8;
                f32 field_0xc;
                undefined4 field_0x10;
            };

            GlobalAudioThing();
            ~GlobalAudioThing();

            SomeAudioThing6* getSomeAudioThing6(u32 idx);
            SomeAudioThing6* FUN_0032b7b8(const char* search, i32 idx, i32 p3);

            static SomeAudioThing6* getSomeAudioThing6WithOffset(u32 idx);
            static i32 FUN_0032fcb0(const char* search);
            static GlobalAudioThing* get();

            SomeAudioThing6* m_sat6Arr;
            u32 m_someSat6Idx;
            u32 field_0x2c8;
            Unk0 m_unk0[16];
            Unk0 m_unk1[16];
        
        protected:
            static GlobalAudioThing* instance;
    };
};