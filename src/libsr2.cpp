#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/Data.h>
#include <libsr2/utilities/argParser.h>
#include <libsr2/states/gameFSM.h>
#include <libsr2/states/GameState.h>
#include <libsr2/gfx/gfx.h>
#include <libsr2/libsr2.h>

#include <string>
#include <filesystem>
#include <stdio.h>

namespace sr2 {
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
        bool success;

        do {
            success = GameEngine::instance->Update();
        } while (success);
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
    }

    void GameEngine::EndFrame() {
    }

    bool GameEngine::Update() {
        BeginFrame();

        GameState* state = fsm->current();
        if (just_update) {
            // dispatch messages
            state->Update();
        } else {
            state->PreUpdate();
            state->Input();
            // dispatch messages
            if (state->CanUpdateTime()) {
                // datTimeManager::Update();
                // gfxTextureMovie::UpdateAll(datTimeManager::Seconds);
            }

            state->Update();
            state->Draw();
            state->PostUpdate();
        }

        EndFrame();

        if (should_pause) fsm->deferred_change(IN_GAME_PAUSED);

        /*
        if (FUN_001c5540((int)&DAT_0035e7b0)) {
            fsm->deferred_change(MENU_LOAD);
            just_update = false;
        }
        */

        fsm->update();
        return !state->Done() && !should_exit;
    }

    void GameEngine::ChangeState(GAME_STATE state) {
        fsm->change(state);
    }
};