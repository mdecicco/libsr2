#pragma once
#include <libsr2/types.h>
#include <libsr2/states/GameState.h>

namespace sr2 {
    class InGameState : public GameState {
        public:
            InGameState();
            ~InGameState();

            virtual bool Done();
    };
};