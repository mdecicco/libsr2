#include <libsr2/managers/datTimeManager.h>

namespace sr2 {
    datTimeManager g_datTimeManager = datTimeManager();

    datTimeManager::datTimeManager() {
        InvSeconds = 0.0f;
        Seconds = 0.0f;
    }

    datTimeManager::~datTimeManager() {
    }
};