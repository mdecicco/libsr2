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
    // undefined auStack32 [16];
    try {
        // from a higher function, but here is good enough
        datArgParser::Init(argc, argv);

        // QuickTicks calls are likely a relic from profiling code which was
        // not compiled in the game's build. Function simply returns the value
        // of the Count register and is totally ineffectual.

        // Utils::Timer::QuickTicks();
        LoadArchives();
        // Utils::Timer::QuickTicks();
        
        // Utils::Timer::QuickTicks();
        // FUN_00101958((long)(int)auStack32); <- Constructs some weird object with a virtual function that creates the game engine, is totally unused
        GameEngine::Create(argc, argv);
        // Utils::Timer::QuickTicks();

        debug_testing();

        // Utils::Timer::QuickTicks();
        GameEngine::Execute();
        GameEngine::Terminate();
        datArgParser::Kill();
    } catch (const std::exception& e) {
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

    glPointSize(5.0f);

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