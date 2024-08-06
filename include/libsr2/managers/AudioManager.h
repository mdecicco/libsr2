#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Cls_003310e0;
    class Cls_00331090;

    class AudioManager {
        public:
            AudioManager(i32 someCount, i32 someOtherCount, i32 p3, Cls_003310e0* p4, i32 p5);
            virtual ~AudioManager();

            virtual void method_0x10();
            virtual void method_0x18();
            virtual void method_0x20();
            virtual void method_0x28();
            virtual void method_0x30();
            virtual void method_0x38();
            virtual void method_0x40();
            virtual void method_0x48();
            virtual void method_0x50();
            virtual void method_0x58();
            virtual void method_0x60();
            virtual void method_0x68();
            virtual void method_0x70();
            virtual void method_0x78();
            virtual u32 loadSoundBanks(undefined4 p1);
            virtual u32 loadSoundBanksAtPath(const char* p1, undefined4 p2);
            virtual void method_0x90(const char* p1, undefined4 p2);
            virtual void method_0x98();
            virtual void method_0xa0(undefined4 p1, undefined4 p2, i32 p3, undefined4 p4, i32 p5);
            virtual void method_0xa8();
            virtual void method_0xb0();
            virtual void method_0xb8();
            virtual void method_0xc0();
            virtual void method_0xc8();
            virtual void method_0xd0();
            virtual void method_0xd8();
            virtual void method_0xe0();
            virtual void method_0xe8();
            virtual void method_0xf0();

            void FUN_0032f9e8(i32 p1);
        
            static i32 findSoundIndex(const char* filename);
            static AudioManager* get();
            static bool isAlive();

        protected:
            static AudioManager* instance;

            Cls_003310e0* m_somethingBig;
            u32 m_someCount;
            Cls_00331090* m_someArr;
    };
};