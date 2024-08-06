#pragma once
#include <libsr2/types.h>
#include <libsr2/managers/AudioManager.h>

#include <utils/Array.h>

namespace sr2 {
    class audSound;
    class aud3DObject;
    class audSource;
    
    class Aud3DObjectManager : public AudioManager {
        public:
            Aud3DObjectManager(u32 someCount, u32 p2, undefined4 p3);
            virtual ~Aud3DObjectManager();

            void FUN_001c1370();
            void FUN_001c1428();
            void FUN_001c14b0();
            void FUN_00321ac0();
            void loadSources();

            virtual void method_0xf8();
            virtual void method_0x108();
            virtual void method_0x110();
            virtual void method_0x118();
            virtual void method_0x120();
            virtual void method_0x128();
            virtual void method_0x130();

            static Aud3DObjectManager* get();
            static bool isAlive();
        
        protected:
            static Aud3DObjectManager* instance;

            mat3x4f** m_listenerMatrices;
            mat3x4f** m_listenerTransforms;
            i32 m_someCount;
            i32 m_count;
            f32 m_volumeArr[4];
            utils::Array<audSource*> m_sources;
            bool m_loadedSources;
            aud3DObject* m_streamList;
            aud3DObject* m_sfxList;
            f32 m_invSomething;
            f32 m_something;

            undefined4** field_0x10;
            undefined4 field_0x20;
            undefined4 field_0x24;
            undefined4 field_0x44;
            undefined4 field_0x48;
            undefined4 field_0x4c;
            undefined4 field_0x50;
            undefined4 field_0x54;
            undefined4 field_0x58;
            undefined4 field_0x5c;
    };
};