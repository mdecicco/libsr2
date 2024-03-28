#include <libsr2/libsr2.h>
#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/argParser.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/creature/ragdoll.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/creature/crSkeletonData.h>
#include <libsr2/creature/crBoneData.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/creature/crAnimation.h>
#include <libsr2/creature/crAnimFrame.h>
#include <libsr2/sim/PhysEntity.h>
#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/managers/RagdollManager.h>
#include <libsr2/math/mat3x4f.h>

#include <Draw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <stdio.h>

using namespace sr2;

void debug_testing();

// 0x001001f8 
int main(int argc, char** argv) {
    try {
        // from a higher function, but here is good enough
        datArgParser::Init(argc, argv);

        LoadArchives();

        GameEngine::Create(argc, argv);

        // debug_testing();

        GameEngine::Execute();
        GameEngine::Terminate();
        datArgParser::Kill();
    }
    catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}

void ragdoll_test() {
    crSkeletonData sdata;
    sdata.load("afghan");
    RagdollManager* rm = RagdollManager::get();
    rm->init(10);
    ragUnk1* unk = rm->getOrLoad(&sdata, "afghan");

    crAnimation* anim = crAnimation::getAnim("afghan_for_run", true, 1, nullptr, 0, 0xffffffff);
    if (!anim) return;

    unk->setPose(&sdata, &anim->frames[0]);

    f32 frameDur = 1.0f / 30.0f;
    f32 frameTmr = 0.0f;
    u32 frameIdx = 0;
    while (!Draw::get()->shouldClose()) {
        frameTmr += Draw::get()->getDeltaTime();
        if (frameTmr > frameDur) {
            frameTmr = 0.0f;
            frameIdx++;

            if (frameIdx >= anim->frameCount) frameIdx = 0;
        }

        unk->setPose(&sdata, &anim->frames[frameIdx]);

        crSkeleton s(100);
        s.init(&sdata, nullptr);
        anim->frames[frameIdx].pose(&s, true);
        unk->setPose(&sdata, &anim->frames[frameIdx]);
        s.update();

        Draw::get()->begin();
        Draw::get()->originGrid(100, 100);
        Draw::get()->drawSkeleton(sdata.boneData, unk, &s);
        Draw::get()->end();
    }

    return;
}

void debug_testing() {
    Draw* d = Draw::get();
    if (!d->init()) return;

    DebugDraw::setDrawer(d);

    datArgParser::AddReplace("level", "Afghan");
    datArgParser::AddReplace("humans", "1");
    datArgParser::AddReplace("mission", "Cruise.mp");

    gfxTexture::None = new gfxTexture();

    ragdoll_test();

    DebugDraw::setDrawer(nullptr);
    Draw::destroy();

    return;
}