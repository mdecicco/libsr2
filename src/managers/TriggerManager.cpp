#include <libsr2/managers/TriggerManager.h>

namespace sr2 {
    TriggerTarget::TriggerTarget() {

    }

    TriggerTarget::~TriggerTarget() {

    }



    Trigger::Trigger() {

    }

    Trigger::~Trigger() {

    }



    TriggerMgr* TriggerMgr::instance = nullptr;
    void TriggerMgr::Create() {
        if (instance) return;
        instance = new TriggerMgr();
    }

    void TriggerMgr::Destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }

    TriggerMgr* TriggerMgr::Get() {
        return instance;
    }

    TriggerMgr::TriggerMgr() {
        player_capacity = 0;
        ambient_capacity = 0;
        local_player_count = 0;
        total_player_count = 0;
        player_count = 0;
        opponent_count = 0;
        ambient_count = 0;
        players = nullptr;
        opponents = nullptr;
        ambients = nullptr;
        trigger_list_a = nullptr;
        trigger_list_b = nullptr;
        mission_name[0] = 0;
    }

    TriggerMgr::~TriggerMgr() {

    }
};