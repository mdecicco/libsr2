#include <libsr2/audio/SomeAudioThing5.h>

namespace sr2 {
    SomeAudioThing5::SomeAudioThing5() {
    }

    SomeAudioThing5::~SomeAudioThing5() {
    }

    bool SomeAudioThing5::hasSomeFlag0() const {
        return ((field_0x34 >> 2) & 1) != 0;
    }
};