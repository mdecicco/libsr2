#include <libsr2/utilities/datTimeManager.h>

namespace sr2 {
    datTimeManager g_datTimeManager = datTimeManager();

    datTimeManager::datTimeManager() {
        InvSeconds = 0.0f;
        Seconds = 0.0f;
    }

    datTimeManager::~datTimeManager() {
    }
};