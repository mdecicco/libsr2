#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class SomeAudioThing6;

    class SomeAudioThing7 {
        public:
            SomeAudioThing7();
            ~SomeAudioThing7();
        
        protected:
            friend class GlobalAudioThing;

            char m_leftChannelPath[128];
            char m_rightChannelPath[128];
            i32 m_sampleRate;
            i32 m_channelSize;
            u32 m_someValFromChannelData;
            
            SomeAudioThing6* field_0x0;
            i32 field_0x108;
            i32 field_0x10c;
            i32 field_0x114;
            i32 field_0x11c;
    };
};