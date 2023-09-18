#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class srActive;
    class srCreatureGroup;
    
    class srActivePool {
        public:
            srActivePool();
            ~srActivePool();

            void reset();
            void addGroup(srCreatureGroup* group);
            void removeGroup(srCreatureGroup* group);
            void update();
            void calculateGroupRepulsion();

            u32 count;
            srActive* pool;
    };
};