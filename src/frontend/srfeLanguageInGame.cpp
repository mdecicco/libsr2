#include <libsr2/frontend/srfeLanguageInGame.h>

namespace sr2 {
    srfeLanguageInGame::srfeLanguageInGame(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen3(name, master) {
    }

    srfeLanguageInGame::~srfeLanguageInGame() {
    }
};