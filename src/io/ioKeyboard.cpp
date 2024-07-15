#include <libsr2/io/ioKeyboard.h>
#include <utils/robin_hood.h>

namespace sr2 {
    ioKeyboard ioKeyboard::gKeyboard = ioKeyboard();

    static robin_hood::unordered_map<utils::KeyboardKey, KEY_CODE> gKeyMap = {
        { utils::KeyboardKey::KEY_ENTER, KEY_CODE::UNK28 },
        { utils::KeyboardKey::KEY_ESCAPE, KEY_CODE::UNK14 },
        { utils::KeyboardKey::KEY_UP, KEY_CODE::UNK200 },
        { utils::KeyboardKey::KEY_DOWN, KEY_CODE::UNK208 },
        { utils::KeyboardKey::KEY_LEFT, KEY_CODE::UNK203 },
        { utils::KeyboardKey::KEY_RIGHT, KEY_CODE::UNK205 }
    };

    ioKeyboard::ioKeyboard() {
        isActive = true;
    }

    ioKeyboard::~ioKeyboard() {
    }
    
    void ioKeyboard::onKeyDown(utils::KeyboardKey key) {
        auto it = gKeyMap.find(key);
        if (it == gKeyMap.end()) return;

        keys.inactive[it->second] = true;
    }
    
    void ioKeyboard::onKeyUp(utils::KeyboardKey key) {
        auto it = gKeyMap.find(key);
        if (it == gKeyMap.end()) return;

        keys.inactive[it->second] = false;
    }

    void ioKeyboard::Update() {
        for (u32 i = 0;i < 256;i++) {
            gKeyboard.keys.active[i] = gKeyboard.keys.inactive[i];
        }
    }
};