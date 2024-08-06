#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/types.h>

namespace sr2 {

    class Cls_003310e0 {
        public:
            struct Unk0 {
                u32 field_0x0;
                char field_0x4[64];
                f32 field_0x44;
                f32 field_0x48;
                f32 field_0x4c;
                f32 field_0x50;
                f32 field_0x54;
            };

            struct Unk1 {
                u32 sfxIndex;
                u32 flags;
            };

            Cls_003310e0(undefined4 p1, undefined4 p2);
            ~Cls_003310e0();

            i32 parseBanks(u32 p1, const char* path);
            u32 loadBank(const char* path);
        
        protected:
            friend class AudioManager;

            // CSV helpers
            void processLine(char* line);
            i32 getPathTokenIndex(PathTokenType ptt, const char* line);

            u32 m_unk0Count;
            u32 m_unk1Count;
            Unk0 m_unk0[256];
            Unk1 m_unk1[256];
            char m_bankFilenames[32][64];

            i32 field_0x0;
            char field_0xc[128];
            undefined4 field_0x8c;
            undefined4 field_0x90;
    };
};