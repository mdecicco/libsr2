#pragma once
#include <libsr2/types.h>
#include <utils/Input.h>

namespace sr2 {
    class ioKeyboard : public utils::IInputHandler {
        public:
            ioKeyboard();
            ~ioKeyboard();

            virtual void onKeyDown(utils::KeyboardKey key);
            virtual void onKeyUp(utils::KeyboardKey key);
        
            bool isActive;
            
            struct {
                bool inactive[256];
                bool active[256];
            } keys;

            static ioKeyboard gKeyboard;
            static void Update();
    };
};