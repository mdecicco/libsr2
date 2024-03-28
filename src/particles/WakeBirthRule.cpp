#include <libsr2/particles/WakeBirthRule.h>
#include <libsr2/gfx/gfxTexture.h>

namespace sr2 {
    WakeBirthRule::WakeBirthRule() {
        texture = gfxTexture::None;
        textureFrame.start = 0;
        textureFrame.end = 0;
        textureFrame.dimensions = { 1.0f, 1.0f };
        textureName[0] = 0;
        unk1 = 0;

        for (u32 i = 0;i < 16;i++) unk0[i].zero();
    }

    WakeBirthRule::~WakeBirthRule() {
    }
};