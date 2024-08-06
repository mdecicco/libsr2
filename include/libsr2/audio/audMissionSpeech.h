#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/audBase.h>

namespace sr2 {
    class audMissionSpeech : public audBase {
        public:
            audMissionSpeech();
            virtual ~audMissionSpeech();
    };
};