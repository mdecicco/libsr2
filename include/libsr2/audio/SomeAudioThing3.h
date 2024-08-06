#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/audMissionSpeech.h>

namespace sr2 {
    class SomeAudioThing3 : public audMissionSpeech {
        public:
            SomeAudioThing3();
            virtual ~SomeAudioThing3();
    };
};