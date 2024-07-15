#include <libsr2/frontend/srfeTheater.h>

namespace sr2 {
    srfeTheater::srfeTheater(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen3(name, master) {
    }

    srfeTheater::~srfeTheater() {
    }
};