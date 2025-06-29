#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class ambientManager {
        public:
            static ambientManager* get();
            static void create();
            static void destroy();

            bool antiGravityEnabled;

        private:
            ambientManager();
            ~ambientManager();

            static ambientManager* instance;
    };
};