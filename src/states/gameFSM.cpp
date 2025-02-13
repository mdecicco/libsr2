#include <libsr2/states/gameFSM.h>
#include <libsr2/states/GameLoadState.h>
#include <libsr2/states/InGameState.h>
#include <libsr2/states/InGamePausedState.h>
#include <libsr2/states/InMenuState.h>
#include <libsr2/states/MenuLoadState.h>
#include <libsr2/utilities/datArgParser.h>
#include <libsr2/libsr2.h>

namespace sr2 {
    gameFSM::gameFSM() {
        m_target = GAME_STATE::PRE_GAME;
        m_current = GAME_STATE::PRE_GAME;
        m_states[u32(GAME_STATE::GAME_LOAD)] = new GameLoadState();
        m_states[u32(GAME_STATE::IN_GAME)] = new InGameState();
        m_states[u32(GAME_STATE::IN_GAME_PAUSED)] = new InGamePausedState();
        m_states[u32(GAME_STATE::IN_MENU)] = new InMenuState();
        m_states[u32(GAME_STATE::MENU_LOAD)] = new MenuLoadState();
    }

    gameFSM::~gameFSM() {
        delete m_states[u32(GAME_STATE::GAME_LOAD)];
        delete m_states[u32(GAME_STATE::IN_GAME)];
        delete m_states[u32(GAME_STATE::IN_GAME_PAUSED)];
        delete m_states[u32(GAME_STATE::IN_MENU)];
        delete m_states[u32(GAME_STATE::MENU_LOAD)];
    }

    GameState* gameFSM::current() const {
        if (m_current == GAME_STATE::PRE_GAME) return nullptr;
        return m_states[u32(m_current)];
    }
    
    GameState* gameFSM::get(GAME_STATE state) const {
        if (state == GAME_STATE::PRE_GAME) return nullptr;
        return m_states[u32(state)];
    }

    void gameFSM::change(GAME_STATE state) {
        if (m_current != GAME_STATE::PRE_GAME) {
            m_states[u32(m_current)]->Exit();
        }

        // FUN_00100d08(param_1, &local_30);

        m_states[u32(state)]->Enter(GameEngine::Instance());
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