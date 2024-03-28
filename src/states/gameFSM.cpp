#include <libsr2/states/gameFSM.h>
#include <libsr2/states/GameLoadState.h>
#include <libsr2/states/InGameState.h>
#include <libsr2/states/InGamePausedState.h>
#include <libsr2/states/InMenuState.h>
#include <libsr2/states/MenuLoadState.h>
#include <libsr2/utilities/argParser.h>
#include <libsr2/libsr2.h>

namespace sr2 {
    gameFSM::gameFSM() {
        m_target = PRE_GAME;
        m_current = PRE_GAME;
        m_states[GAME_LOAD] = new GameLoadState();
        m_states[IN_GAME] = new InGameState();
        m_states[IN_GAME_PAUSED] = new InGamePausedState();
        m_states[IN_MENU] = new InMenuState();
        m_states[MENU_LOAD] = new MenuLoadState();
    }

    gameFSM::~gameFSM() {
        delete m_states[GAME_LOAD];
        delete m_states[IN_GAME];
        delete m_states[IN_GAME_PAUSED];
        delete m_states[IN_MENU];
        delete m_states[MENU_LOAD];
    }

    GameState* gameFSM::current() const {
        if (m_current == PRE_GAME) return nullptr;
        return m_states[m_current];
    }
    
    GameState* gameFSM::get(GAME_STATE state) const {
        if (state == PRE_GAME) return nullptr;
        return m_states[state];
    }

    void gameFSM::change(GAME_STATE state) {
        if (m_current != PRE_GAME) {
            m_states[m_current]->Exit();
        }

        // FUN_00100d08(param_1, &local_30);

        m_states[state]->Enter(GameEngine::Instance());
        m_target = state;
        m_current = state;
    }

    void gameFSM::deferred_change(GAME_STATE state) {
        m_target = state;
    }

    void gameFSM::update() {
        if (m_target != m_current) change(m_target);
    }
};