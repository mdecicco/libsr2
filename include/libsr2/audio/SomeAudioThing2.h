#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/SomeAudioThing1.h>

namespace sr2 {
    class SomeAudioThing2 : public SomeAudioThing1 {
        public:
            SomeAudioThing2();
            virtual ~SomeAudioThing2();
    };
};