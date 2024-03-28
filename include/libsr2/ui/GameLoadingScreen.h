#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class GameLoadingScreen {
        public:
            static void create();
            static void destroy();
            static GameLoadingScreen* get();

            void FUN_001c54e8(i32 unk0);
            void FUN_001c54f0(f32 unk0);
            bool FUN_001c5540();
            void FUN_001c57c0();
        
        protected:
            GameLoadingScreen();
            ~GameLoadingScreen();

            static GameLoadingScreen* instance;
    };
};