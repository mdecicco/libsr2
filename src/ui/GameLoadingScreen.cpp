#include <libsr2/ui/GameLoadingScreen.h>

namespace sr2 {
    GameLoadingScreen* GameLoadingScreen::instance = nullptr;

    GameLoadingScreen::GameLoadingScreen() {
    }

    GameLoadingScreen::~GameLoadingScreen() {
    }

    void GameLoadingScreen::create() {
        if (!instance) instance = new GameLoadingScreen();
    }

    void GameLoadingScreen::destroy() {
        if (instance) delete instance;
        instance = nullptr;
    }

    GameLoadingScreen* GameLoadingScreen::get() {
        return instance;
    }

    void GameLoadingScreen::FUN_001c54e8(i32 unk0) {
    }

    void GameLoadingScreen::FUN_001c54f0(f32 unk0) {
    }
    
    bool GameLoadingScreen::FUN_001c5540() {
        return false;
    }

    void GameLoadingScreen::FUN_001c57c0() {
    }
};