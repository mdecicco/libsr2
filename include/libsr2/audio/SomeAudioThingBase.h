#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class SomeAudioThingBase {
        public:
            SomeAudioThingBase();
            virtual ~SomeAudioThingBase();
        
        protected:
            SomeAudioThingBase* m_last;
            SomeAudioThingBase* m_next;
    };
};