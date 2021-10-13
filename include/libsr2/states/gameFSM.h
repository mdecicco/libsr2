#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum GAME_STATE {
        GAME_LOAD,
        IN_GAME,
        IN_GAME_PAUSED,
        IN_MENU,
        MENU_LOAD,
        PRE_GAME = 0xffffffff
    };

    class GameState;
    class gameFSM {
        public:
            gameFSM();
            ~gameFSM();
            
            GameState* current() const;

            // 0x00100c70
            void change(GAME_STATE state);

            void deferred_change(GAME_STATE state);

            void update();

        protected:
            GAME_STATE m_target;
            GAME_STATE m_current;
            GameState* m_states[5];
    };
};