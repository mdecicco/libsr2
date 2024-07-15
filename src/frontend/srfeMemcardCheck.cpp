#include <libsr2/frontend/srfeMemcardCheck.h>

namespace sr2 {
    srfeMemcardCheck::srfeMemcardCheck(const char* name, const WidgetRef<ui2Master>& master) : srui2SRScreen(name, master) {
    }

    srfeMemcardCheck::~srfeMemcardCheck() {
    }
};