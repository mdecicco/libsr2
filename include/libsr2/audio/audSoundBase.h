#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/SomeAudioThing3.h>

namespace sr2 {
    class audSoundBase : public SomeAudioThing3 {
        public:
            audSoundBase();
            virtual ~audSoundBase();
    };
};