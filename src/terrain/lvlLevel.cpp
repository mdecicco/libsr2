#include <libsr2/terrain/lvlLevel.h>

namespace sr2 {
    lvlLevel* lvlLevel::instance = nullptr;

    lvlLevel::lvlLevel() {
        lvlLevel::instance = this;
        m_unk = 0;
        m_cull_rooms = nullptr;
        m_room_count = 0;
        m_first_instance = nullptr;
        m_instance_count = 0;
    }

    lvlLevel::~lvlLevel() {
        lvlLevel::instance = nullptr;
    }

    void lvlLevel::moveToRoom(unkInstance0* instance, i16 roomIdx) {
    }
};