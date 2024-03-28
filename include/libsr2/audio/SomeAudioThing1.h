#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/SomeAudioThingBase.h>

namespace sr2 {
    class SomeAudioThing1 : public SomeAudioThingBase {
        public:
            SomeAudioThing1();
            virtual ~SomeAudioThing1();
    };
};