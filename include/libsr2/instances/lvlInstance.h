#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class lvlInstance {
        public:
            lvlInstance();
            ~lvlInstance();

            u16 geomIdxPlus1;
    };
    
    class UnkInstance0 : public lvlInstance {
        public:
            UnkInstance0();
            ~UnkInstance0();
            
            u16 room_id;
            u16 flags;
            UnkInstance0* prev;
            UnkInstance0* next;
    };
};