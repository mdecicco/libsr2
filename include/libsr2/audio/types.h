#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum class PathTokenType : u32 {
        ModelObjectBank = 1,
        SampleSamplesVariants = 2,
        MusicSpeechEngine = 3
    };

    enum class PathTokenMOB : u32 {
        None,
        Model,
        Object,
        Bank,
        PathWin,
        PathPsx,
        LoadWin,
        LoadPsx,
        LoadAll
    };

    enum class PathTokenSSV : u32 {
        None,
        Sample,
        Samples,
        Variants
    };

    enum class PathTokenMSE : u32 {
        None,
        Music,
        Speech,
        Engine
    };
    
    enum class AudioType : u32 {
        SoundEffects = 0,
        Music,
        Voice
    };

    enum class AudioTypeIndex : u32 {
        None = 0,
        SoundEffects,
        Music,
        Voice
    };

    enum class SampleRateCode : u32 {
        srZero      = 0x77,
        sr22050     = 0x78,
        sr44100     = 0x79,
        srDual44100 = 0x7a,
        srUnk0      = 0x7d,
        srUnk1      = 0x7e
    };
};