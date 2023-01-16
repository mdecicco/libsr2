#pragma once
#include <libsr2/types.h>
#include <libsr2/states/GameState.h>

namespace sr2 {
    class GameLoadState : public GameState {
        public:
            GameLoadState();
            ~GameLoadState();

            virtual void Reset();
            virtual void Enter(GameEngine* eng);
            virtual void Update();
            virtual void Draw();
            virtual bool Done();
            virtual void Exit();

            undefined4 next_load_switch_mode;
            undefined4 load_switch_mode;

            // ui2Base* loading_base;
    };
};