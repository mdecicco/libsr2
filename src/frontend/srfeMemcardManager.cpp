#include <libsr2/frontend/srfeMemcardManager.h>

namespace sr2 {
    srfeMemcardManager::srfeMemcardManager(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen4(name, master) {
    }

    srfeMemcardManager::~srfeMemcardManager() {
    }
};