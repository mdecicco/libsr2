#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class SomeAudioThing6;
    
    class Aud3DObjectManager {
        public:
            Aud3DObjectManager();
            ~Aud3DObjectManager();

            void FUN_001c1370();
            void FUN_001c1428();
            void FUN_001c14b0();

            static Aud3DObjectManager* get();
            static bool isAlive();
        
        protected:
            static Aud3DObjectManager* instance;
    };
};