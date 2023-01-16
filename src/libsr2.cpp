#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/Data.h>
#include <libsr2/utilities/argParser.h>
#include <libsr2/utilities/timer.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/states/gameFSM.h>
#include <libsr2/states/GameState.h>
#include <libsr2/gfx/gfx.h>
#include <libsr2/libsr2.h>

#include <string>
#include <filesystem>
#include <thread>
#include <stdio.h>

namespace sr2 {
    u64 g_HostTimer = 1;
    u64 g_FrameTimer = 1;
    bool g_showFrameTime = false;

    GameEngine* GameEngine::instance = nullptr;

    GameEngine* GameEngine::Create(int argc, char** args) {
        // Originally this was created by now, but for now this is fine
        GameEngine::instance = new GameEngine(argc, args);


        // Utils::msgMsgSource::Register(this, 0x84, 0x80);
        GameEngine::instance->ChangeState(IN_MENU);

        if (!gfx::g_SomeViewport) gfx::g_SomeViewport = gfx::pipeline::Viewport;

        if (gfx::g_SomeViewport != gfx::pipeline::Viewport) {
            gfx::pipeline::ForceSetViewport(gfx::g_SomeViewport);
        }

        return GameEngine::instance;
    }

    void GameEngine::Terminate() {
        delete GameEngine::instance;
    }

    void GameEngine::Execute() {
        bool do_continue = true;
        // windows doesn't like sleeping for less time than this
        // otherwise I'd set it to 60 FPS
        f32 target = 1.0f / 20.0f;
        while (do_continue) {
            auto begin = std::chrono::high_resolution_clock::now();

            do_continue = GameEngine::instance->Update();

            auto end = std::chrono::high_resolution_clock::now();

            // this time stuff isn't in the game, it's just to
            // not use 100% CPU while doing nothing.
            f32 seconds = std::chrono::duration<sr2::f32>(end - begin).count();
            if (seconds < target) {
                std::this_thread::sleep_for(std::chrono::duration<sr2::f32>(target - seconds));
            }
        }
    }

    GameEngine* GameEngine::Instance() {
        return GameEngine::instance;
    }

    GameEngine::GameEngine(int argc, char** args) {
        std::string path = args[0];
        path = path.substr(0, path.find_last_of('/'));
        path = path.substr(0, path.find_last_of('\\'));
        std::filesystem::current_path(path);

        fsm = new gameFSM();
        frame_count = 0;
        should_pause = false;
        just_update = false;
        should_exit = false;
    }

    GameEngine::~GameEngine() {
        delete fsm;
        delete g_Archives;
    }

    void GameEngine::BeginFrame() {
        // IO::ioInput::Poll();
        gfx::pipeline::Manage();
        gfx::pipeline::BeginFrame();
        if (just_update) gfx::pipeline::Clear(3, gfx::g_clearColor, 1.0f, 0);
        g_HostTimer = timer::Ticks();
    }

    void GameEngine::EndFrame() {
        u64 t = timer::Ticks() - g_HostTimer;
        gfx::g_HostTime = f32(t) * 3.390842e-06f;

        if (g_showFrameTime) {
            // std::string tm = format("CPU=%5.2fms VU/GS=%5.2fms FRAME=%5.2fms", gfx::g_HostTime, gfx::g_DrawTime, gfx::g_FrameTime);
            // gfx::DrawFont(0x18, 0x18, tm.c_str(), 0x80ffffff);
        }

        gfx::pipeline::EndFrame();
        t = timer::Ticks() - g_FrameTimer;
        gfx::g_FrameTime = f32(t) * 3.390842e-06f;
        g_FrameTimer = timer::Ticks();
    }

    bool GameEngine::Update() {
        BeginFrame();

        GameState* state = fsm->current();
        if (just_update) {
            // msgMsgSource::Dispatch();
            state->Update();
        } else {
            state->PreUpdate();
            state->Input();
            // msgMsgSource::Dispatch();
            if (state->CanUpdateTime()) {
                // datTimeManager::Update();
                // gfxTextureMovie::UpdateAll(datTimeManager::Seconds);
            }

            // There is also a lot of time tracking going on here, but
            // whether or not it has an effect on the game remains to
            // be seen.
            state->Update();
            state->Draw();
            state->PostUpdate();
        }

        EndFrame();

        if (should_pause) fsm->deferred_change(IN_GAME_PAUSED);

        if (just_update) {
            /*
            if (FUN_001c5540((int)&DAT_0035e7b0)) {
                fsm->deferred_change(MENU_LOAD);
                just_update = false;
            }
            */
        }

        fsm->update();
        return !state->Done() && !should_exit;
    }

    void GameEngine::ChangeState(GAME_STATE state) {
        fsm->change(state);
    }
};