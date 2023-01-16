#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class GameEngine;
    class GameState {
        public:
            GameState();
            ~GameState();

            virtual void Enter(GameEngine* eng);
            virtual void Exit();
            virtual void RequestReset();
            virtual void Input();
            virtual void PreUpdate();
            virtual void Update();
            virtual void PostUpdate();
            virtual void Draw();
            virtual bool Done();
            virtual bool CanUpdateTime();
            virtual void Reset();

            virtual void method_0x60();
            virtual void Reset(GameEngine* eng);
            
        protected:
            bool m_needs_reset;
    };
};