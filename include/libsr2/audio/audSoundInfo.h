#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class audSound;

    class audSoundInfo {
        public:
            audSoundInfo();
            ~audSoundInfo();
        
        protected:
            friend class audManager;
            friend class audSound;

            char m_leftChannelPath[128];
            char m_rightChannelPath[128];
            i32 m_sampleRate;
            i32 m_channelSize;
            u32 m_someValFromChannelData;
            u32 m_flags;
            i32 m_memOffset;
            audSound* m_owner;
            
            i32 field_0x108;
            i32 field_0x10c;
            i32 field_0x114;
            i32 field_0x11c;
    };
};