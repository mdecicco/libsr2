#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/types.h>

#include <utils/Array.h>
#include <utils/String.h>

namespace sr2 {
    class audSound;
    class Aud3DObjectManager;
    class Cls_0032e0b0;

    /*
        Part of this class is a client that communicates with the AGE audio driver (system/audio.irx)
        This is a description of each of their custom audio commands that run on the IOP
        
        0x00: SPU_SetQueueMsg(packet) -> maybe calls "PlayResidentSFX" (see SPU_ProcessQueue at 0x00000c14)
        0x01: Start stream
            payload[0]:
            payload[1]: voice index if < 64, otherwise stream index ???
            payload[2]:
            payload[3]:
            payload[4]:
            payload[5]:
            payload[6]:
        0x02: Set voice volume
            payload[0]: voice index
            payload[1]: left channel volume
            payload[2]: right channel volume
        0x03: Set voice pitch
            payload[0]: voice index
            payload[1]: pitch
        0x04: SPU_SetQueueMsg(packet) (see SPU_ProcessQueue at 0x00000c14)
        0x05: Stop stream
            payload[0]: voice index if < 64, otherwise stream index ???
        0x06: Not implemented?
        0x07: Not implemented?
        0x08: Not implemented?
        0x09: Calls "GetChannelStatus" and "AdpcmGetStatus(0)"
            payload[0] -> "LoadFromEE", if equal to 0xFFFF set "StreamBusy = 2"
            payload[1] -> "LastLoadCheck"
        0x0A: Pause all (payload unused)
        0x0B: Resume all (payload unused)
        0x0C: Initialize stream data (payload unused)
        0x0D: Allocate stream buffer
            stream index = payload[0]
            offset probably = *(u32*)&payload[1]
            buffer size = *(u32*)&payload[3]
        0x0E: StreamMemoryEnd = payload[0] * 0x10000 + payload[1]
        0x0F: Free stream buffer, payload[0] = index
        0x10: Set maximum user streams, whatever that means
            if payload[0] < 48: UserMaxStreams = payload[0]
        0x11: Init Check Stereo Status
            payload[0]: voice index
            payload[1]: ?
            payload[2]: ?
        0x12: Clear Check Stereo Status (payload unused)
        0x13: Chain streams?
            payload[0]: voice index
            payload[1]: number of 16-bit integers to follow in the payload
        0x14: Enable reverb
            payload[0]: type
            payload[1]: "clearwa"
        0x15: Set reverb channel mix
            payload[0]: channel
            payload[1]: "fx_lev"
        0x16: Set reverb FX
            payload[0]: type
            payload[1]: "fx_lev"
        0x17: Set reverb mix
            payload[0]: ?
            payload[1]: ?
        0x18: Set "master core output"
            payload[0]: ?
            payload[1]: ?
        0x19: Set "master channel volume"
            payload[0]: channel
            payload[1]: pan
        0x1a: Set "master core input"
            payload[0]: ?
            payload[1]: ?
        0x1b: All channel effects on
            payload[0]: ?
        0x1c: All channel effects off
            payload[0]: ?
        0x1d: Not implemented?
        0x1e: "PatchSFX"
            payload[0]: filename index
            payload[1]: size / 64
            payload[2]:
            payload[3]:
            payload[4, 5]: u32 offset of some kind
        0x1f: "block_cd = 1"
        0x20: "block_cd = 0"
    
    */

    class audManager {
        public:
            struct AudioTypeCtrl {
                u32 count;
                f32 volume;
                undefined4 field_0x8;
                f32 field_0xc;
                f32 field_0x10;
            };
            struct Unk0 {
                u32 count;
                f32 balance;
                f32 field_0x8;
                u32 index;
                undefined4 field_0x10;
            };

            audManager(i32 someCount, i32 p2, i32 p3);
            ~audManager();

            void init();
            void shutdown();
            u32 getFreeIndex();
            audSound* findSFX(const char* filename, u32 startIndex, u32 notThisIndex);
            AudioTypeCtrl* getAudioTypeCtrl(AudioTypeIndex index);
            void allocateAudioHeap();
            undefined4 initClient();
            undefined4 initStreams();
            void setMasterVolume(f32 p1);

            void FUN_00329f60(i32 p1, i32 p2, i32 p3, i32 maxStreams);
            void FUN_0032a0c8(i32 p1);
            void FUN_0032a328();
            audSound* getSoundEffect(u32 idx);
            const char* getPathPrefix();
            const char* getPathBase();
            const char* getSomePath();
            void setPathPrefix(const char* pathPrefix);
            void setPathBase(const char* pathBase);
            void FUN_0032a720();
            void FUN_0032a850();
            void FUN_0032a880(i32 p1, i32 p2);
            undefined4* FUN_0032a958(i32 p1, i32 p2);
            void FUN_0032aa18(i32 p1);
            audSound* loadSoundAtIndex(const char* filename, i32 someIdx);
            void FUN_0032aaa0(f32 p1, f32 p2, f32 p3, i32 p4);
            void FUN_0032abf8(f32 p1, i32 p2);
            void FUN_0032ace0();
            u32 FUN_0032adb8(f32 p1, f32 p2, f32 p3, i32 p4);
            void FUN_0032af10(f32 p1, i32 p2);
            void FUN_0032aff8();
            void FUN_0032b0b8(mat3x4f* p1);
            mat3x4f* FUN_0032b0d8();
            undefined4 FUN_0032b0e0(i32 p1);
            void setReference(audSound* original, audSound* referencedBy);
            i32 FUN_0032b410(i32 p1, f32* p2);
            i32 FUN_0032b4a0(i32 p1, f32* p2);
            i32 FUN_0032b530(i32 p1, f32* p2, i32 p3, i32 p4);
            void FUN_0032b5d8(mat3x4f* p1);
            i32 FUN_0032b620();
            void FUN_0032b760();
            undefined4 FUN_0032b8e8();
            void FUN_0032b958(f32 p1, f32* p2, f32* p3);
            i32 FUN_0032ba10(i32 p1);
            void setNextSampleRateCode(SampleRateCode p1);
            audSound* FUN_0032bb60();
            f32 FUN_0032bc10(f32* p1);
            f32 FUN_0032c138(vec3f* p1);
            void FUN_0032c180(u8 p1);
            void FUN_0032c1f0(f32 p1, i32 p2);
            void FUN_0032c2c8(i32 p1, undefined2 p2);
            void FUN_0032c340(f32 p1, i32 p2);
            void FUN_0032c458(i32 p1);
            i32 FUN_0032c580(f32 p1, f32 p2, f32 p3, i32 p4, i32 p5, undefined4 p6, undefined4 p7, undefined4 p8, i32 p9);
            i32 addFile(i32 p1, const char* p2);
            undefined4 FUN_0032c958(const char* p2);
            undefined4 FUN_0032c9a0(undefined4 p1, undefined4 p2);
            undefined4 FUN_0032c9d0();
            undefined4 FUN_0032c9d8(const char* p1);
            bool loadVAG(audSound* sfx, const char* path);
            i32 FUN_0032cd08(i32 p1);
            void FUN_0032ce58();
            void FUN_0032ceb0(i32 p1);
            undefined4 FUN_0032cec0(i32 p1);
            i32 FUN_0032ced0(i32 p1);
            bool FUN_0032cf28(i32 p1);
            u32 FUN_0032cf58(i32 p1);
            i32 FUN_0032cf80();
            void FUN_0032d010(undefined p1);
            i32 FUN_0032d070(i32 p1, i32 p2, i32 p3);
            undefined4 FUN_0032d108(u32 p1, i32 p2);
            undefined4 FUN_0032d198(u32 p1, i32 p2, i32 p3);
            void FUN_0032d1f0(f32 p1, f32 p2, f32 p3, f32 p4, i32 p5);
            void FUN_0032d220(f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6, i32 p7);
            void FUN_0032d2e0(f32 p1);
            void FUN_0032d328(f32 p1);
            void FUN_0032d370(f32 p1, f32 p2);
            void FUN_0032d3f0(f32 p1, f32 p2);
            void FUN_0032d440(i32 p1, undefined2 p2);
            void FUN_0032d478(undefined2 p2);
            void FUN_0032d4a0(u16 p1, u16 p2);
            void FUN_0032d4d0(u16 p1, i32 p2, i32 p3, u16 p4, undefined2 p6, undefined2 p7, i16 p8, i32 p9);
            void FUN_0032d538();
            void FUN_0032d598(u16 p1, i32 p2, u16 p3, u16 p4, u16 p5, i16 p6, i32 p7);
            void FUN_0032d5f0(u16 p1, u16 p2, u16 p3);
            void FUN_0032d620(u16 p1, u16 p2);
            void FUN_0032d650(u16 p1);
            void FUN_0032d678(u16 p1);
            void FUN_0032d6a0(u16 p1, u16 p2);
            void FUN_0032d6e0();
            void FUN_0032d700();
            void FUN_0032d720(u16 p1);
            void FUN_0032d748(u16 p1);
            void FUN_0032d770(u16 p1);
            void FUN_0032d798(u16 p1, u16 p2, u16 p3);
            void FUN_0032d7d8(u16 p1);
            void FUN_0032d808();
            void FUN_0032d830();
            void FUN_0032d858(i32 p1);
            void FUN_0032d8a8(u16 p1, u16 p2);
            void FUN_0032d8d8(u16 p1, u16 p2);
            void FUN_0032d908(u16 p1, u16 p2);
            void FUN_0032d938(u16 p1, u8 p2);
            void FUN_0032d9c0(u16 p1, u16 p2, u16* p3);
            void FUN_0032da38(u16 p1, u16 p2, u16 p3, u16 p4, undefined4 p5);
            void FUN_0032da78(undefined2 p1, i16 p2);
            void FUN_0032db50();
            void FUN_0032db78(i32 p1);
            void FUN_0032dc00();
            u32 FUN_0032de88(i32 p1);
            i32 transferToIOP(void* src, void* dst, i32 size);

            static void initStatics();
            static void loadSoundBanks(const char* path, undefined4 p2);
            static f32 FUN_0032b9f8(f32 p1);
            static audSound* getOffsetSFX(u32 idx);
            static void playSound(i32 soundIdx);
            static audManager* get();
            static void create(i32 someCount, i32 p2, i32 p3);
            static void destroy();

            static i32 DAT_00362eb0;
            static Aud3DObjectManager* SomeManager;
            static i32 Count;
            static i32 DAT_00362ea8;
            static f32 FLOAT_00362ee8;
            static i32 DAT_00362eec;
            static u32 DAT_00362eac;
            static i32 DAT_00362eb4;
            static i32 DAT_00362eb8[4];
            static i32 DAT_00362ec8[4];
            static i32 DAT_00362ed8[4];
        
        protected:
            friend class AudioManager;
            friend class Aud3DObjectManager;
            friend class audSound;
            friend class Cls_003310e0;
            static audManager* instance;

            u32 m_reservedSfxCount;
            u32 m_sfxCapacity;
            u32 m_sfxCount;
            u32 m_totalFileCount;
            u32 m_filenameCount;
            u32 m_totalAudioMem;
            u32 m_totalAlignedAudioMem;
            audSound* m_sounds;
            audSound* m_lastLoaded;
            AudioTypeCtrl m_audioTypeCtrls[16];
            SampleRateCode m_nextSampleRateCode;
            u32 m_nextIndex;
            u32 m_streamMemoryEnd;
            Cls_0032e0b0* m_someArrayContainer;
            u32 m_streamsRemaining;
            u32 m_maxStreams;
            u32 m_someCount;
            u32 m_someSampleRate;
            bool m_hasAudioMem;
            mat3x4f* m_currentListenerTransform;
            u32 m_totalCtrlCount;
            u32 m_totalUnk1Count;
            f32 m_masterVolume;
            u32 m_someOffset;
            bool m_hasSomeCount;
            bool m_useHostFS;

            Unk0 m_unk[16];
            char m_somePath[128];
            char m_pathPrefix[128];
            char m_pathBase[32];
            
            undefined4 field_0x8;
            undefined4 field_0x294;
            undefined4 field_0x298;
            undefined4 field_0x2ac;
            undefined4 field_0x2b8;
            undefined4 field_0x2bc;
            undefined4 field_0x2d0;
            undefined4 field_0x2d4;
            undefined4 field_0x2e0;
            undefined4 field_0x2ec;
            undefined4 field_0x370;
            undefined4 field_0xf40;
            undefined4 field_0xf50;
            undefined4 field_0xf44;
            undefined4 field_0xf48;
            undefined4 field_0xf4c;
            undefined4 field_0x1370;
            undefined4 field_0x13b5;
            undefined4 field_0x1314;
            undefined4 field_0x1318;
            undefined4 field_0x131c;
            undefined4 field_0x1350;
            undefined4 field_0x1354;
            undefined4 field_0x1358;
            undefined4 field_0x135c;
            undefined4 field_0x1360;
            undefined4 field_0x13c0;
            undefined4 field_0x13cc;
            undefined4 field_0x13c8;
            undefined4 field_0x136c;
            undefined4 field_0x1364;
            undefined field_0x13d8[48];
            undefined field_0x1408[48];
            undefined4 field_0x14bc;
            undefined4 field_0x14c8;
            undefined4 field_0x14dc;

            utils::Array<utils::String> m_filenames;
            utils::Array<utils::String> m_debugFilenames;
    };
};