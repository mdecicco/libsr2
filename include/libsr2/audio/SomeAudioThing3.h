#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/SomeAudioThing2.h>

namespace sr2 {
    class SomeAudioThing3 : public SomeAudioThing2 {
        public:
            SomeAudioThing3();
            virtual ~SomeAudioThing3();
    };
};