#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/types.h>

namespace sr2 {
    class audSoundInfo;
    class audPlayer;
    
    class audSound {
        public:
            audSound();
            virtual ~audSound();

            void init();

            i32 load(const char* file);
            u32 getIndex() const;
            AudioTypeIndex getTypeIndex() const;

            void reset();
            void setFlags(u32 flags);
            bool isUsed();

            void FUN_00328220();
            void FUN_003287d0();
            bool FUN_00328968();
            void FUN_00328da8(f32 p1);
            void setVolume(f32 volume);
            void setVolumeOnly(f32 volume);
            void setRadius(f32 radius);
            void FUN_00329708(undefined4 p1);
            bool play();
            
            bool hasFlag(u32 bit) const;
        
        protected:
            friend class audManager;
            audPlayer* m_player;
            audSoundInfo* m_info;
            u32 m_index;
            u32 m_sampleRate;
            SampleRateCode m_sampleRateCode;
            i32 m_leftChannelFilenameIdx;
            i32 m_rightChannelFilenameIdx;
            u32 m_flags;
            u32 m_refCount;
            f32 m_volume;
            f32 m_balance;
            u32 m_pitch;
            f32 m_radiusSquared;
            AudioTypeIndex m_audioType;
            u32 m_unk1Index;

            vec3f field_0x0;
            u8 field_0x18;
            u8 field_0x19;
            u8 field_0x1a;
            u8 field_0x1b;
            u32 field_0x1c;
            u32 field_0x28;
            f32 field_0x3c;
            f32 field_0x40;
            f32 field_0x50;
    };
};