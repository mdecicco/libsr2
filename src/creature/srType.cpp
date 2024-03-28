#include <libsr2/creature/srType.h>
#include <libsr2/creature/ragdoll.h>

namespace sr2 {
    srType::srType() {
    }

    srType::~srType() {
    }

    void srType::load(srCreatureGroup* group) {
    }

    void srType::initData() {
    }

    void srType::FUN_001ad528(const char* name) {
    }

    dgRagdoll* srType::FUN_001ae018() {
        if (!lastEle) return nullptr;

        return lastEle->getRagdoll();
    }
};