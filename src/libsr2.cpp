#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/Data.h>
#include <libsr2/states/gameFSM.h>
#include <libsr2/states/GameState.h>
#include <libsr2/libsr2.h>

#include <exception>
#include <string>
#include <filesystem>
#include <stdio.h>

namespace sr2 {
    GameEngine* GameEngine::instance = nullptr;

    void GameEngine::Create(int arg_count, const char** args) {
        GameEngine::instance = new GameEngine(arg_count, args);
    }

    void GameEngine::Destroy() {
        delete GameEngine::instance;
    }

    GameEngine* GameEngine::Instance() {
        return GameEngine::instance;
    }

    GameEngine::GameEngine(int arg_count, const char** args) {
        std::string path = args[0];
        path = path.substr(0, path.find_last_of('/'));
        path = path.substr(0, path.find_last_of('\\'));
        std::filesystem::current_path(path);

        LoadArchives();
        fsm = new gameFSM();
        frame_count = 0;
        should_pause = false;
        just_update = false;
        should_exit = false;

        fsm->change(IN_MENU);
    }

    GameEngine::~GameEngine() {
        delete fsm;
        delete g_Archives;
    }

    void GameEngine::BeginFrame() {
    }

    void GameEngine::EndFrame() {
    }

    void GameEngine::Run() {
        bool exit = false;
        do {
            exit = Update();
        } while (!exit);
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
};



int main(int argc, const char** argv) {
    try {
        sr2::GameEngine::Create(argc, argv);
        sr2::GameEngine::Instance()->Run();
        sr2::GameEngine::Destroy();
    } catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}