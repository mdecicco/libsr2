#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class lvlInstance {
        public:
            lvlInstance();
            virtual ~lvlInstance();

            virtual mat3x4f getTransform() const = 0;
            virtual void setTransform(const mat3x4f& transform) = 0;

            u16 geomIdxPlus1;
    };
    
    class UnkInstance0 : public lvlInstance {
        public:
            struct Flags {
                bool unk0  : 1;
                bool unk1  : 1;
                bool unk2  : 1;
                bool unk3  : 1;
                bool unk4  : 1;
                bool unk5  : 1;
                bool unk6  : 1;
                bool unk7  : 1;
                bool unk8  : 1;
                bool unk9  : 1;
                bool unk10 : 1;
                bool unk11 : 1;
                bool unk12 : 1;
                bool unk13 : 1;
                bool unk14 : 1;
                bool unk15 : 1;
            };

            UnkInstance0();
            ~UnkInstance0();
            
            u16 room_id;
            Flags flags;
            UnkInstance0* prev;
            UnkInstance0* next;
    };
};