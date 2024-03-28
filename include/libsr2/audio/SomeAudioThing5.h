#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/SomeAudioThing4.h>

namespace sr2 {
    class SomeAudioThing7;

    class SomeAudioThing5 : public SomeAudioThing4 {
        public:
            SomeAudioThing5();
            virtual ~SomeAudioThing5();
            
            bool hasSomeFlag0() const;
            
            i32 field_0x20;
            undefined4 field_0x2c;
            u32 field_0x34;
            SomeAudioThing7* field_0x54;
    };
};