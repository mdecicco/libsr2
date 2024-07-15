#include <libsr2/frontend/srfeConceptArt.h>

namespace sr2 {
    srfeConceptArt::srfeConceptArt(const char* name, const WidgetRef<ui2Master>& master) : srui2SRScreen(name, master) {
    }

    srfeConceptArt::~srfeConceptArt() {
    }
};