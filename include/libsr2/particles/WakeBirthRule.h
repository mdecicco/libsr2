#pragma once
#include <libsr2/types.h>
#include <libsr2/particles/ParticleBirthRule.h>

namespace sr2 {
    class gfxTexture;

    class WakeBirthRule : public ParticleBirthRule {
        public:
            WakeBirthRule();
            ~WakeBirthRule();

            char version[20];
            char name[256];
            char textureName[256];
            gfxTexture* texture;
            vec4f unk0[16];
            undefined4 unk1;
            struct {
                u32 start;
                u32 end;
                vec2f dimensions;
            } textureFrame;
    };
};