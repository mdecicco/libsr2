#include <libsr2/states/GameState.h>

namespace sr2 {
    GameState::GameState() {
        m_needs_reset = false;
    }

    GameState::~GameState() {
    }

    void GameState::Enter(GameEngine* eng) {
        m_needs_reset = false;
    }

    void GameState::Exit() {
    }

    void GameState::RequestReset() {
        m_needs_reset = true;
    }

    void GameState::Input() {
    }

    void GameState::PreUpdate() {
    }

    void GameState::Update() {
    }

    void GameState::PostUpdate() {
        if (m_needs_reset) {
            m_needs_reset = false;
            Reset();
        }
    }

    void GameState::Draw() {
    }

    bool GameState::Done() {
        return 1;
    }

    bool GameState::CanUpdateTime() {
        return 1;
    }

    void GameState::Reset() {
    }

    void GameState::method_0x60() {
        
    }

    void GameState::method_0x80(GameEngine* eng) {
    }
};