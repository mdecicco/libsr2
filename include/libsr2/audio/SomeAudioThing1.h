#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/audBase.h>

namespace sr2 {
    class SomeAudioThing1 : public audBase {
        public:
            SomeAudioThing1();
            virtual ~SomeAudioThing1();
    };
};