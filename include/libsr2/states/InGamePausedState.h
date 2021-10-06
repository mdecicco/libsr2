#pragma once
#include <libsr2/types.h>
#include <libsr2/states/GameState.h>

namespace sr2 {
    class InGamePausedState : public GameState {
        public:
            InGamePausedState() { }
            ~InGamePausedState() { }
    };
};