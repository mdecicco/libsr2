#include <libsr2/managers/Aud3DObjectManager.h>
#include <libsr2/managers/audManager.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/audio/audSource.h>
#include <libsr2/io/stream.h>

#include <utils/Array.hpp>

namespace sr2 {
    Aud3DObjectManager* Aud3DObjectManager::instance = nullptr;

    Aud3DObjectManager::Aud3DObjectManager(u32 someCount, u32 p2, undefined4 p3) : AudioManager(400, 0, 0x3c, nullptr, 1) {
        instance = this;
        m_loadedSources = false;
        m_count = 0;
        m_someCount = someCount;
        m_listenerMatrices = nullptr;
        m_listenerTransforms = nullptr;
        m_something = 9.0f;
        m_invSomething = 1.0f / m_something;
        m_streamList = nullptr;
        m_sfxList = nullptr;

        m_volumeArr[0] = 1.0f;
        m_volumeArr[1] = 1.0f;
        m_volumeArr[2] = 1.0f;
        m_volumeArr[3] = 1.0f;

        field_0x10 = new undefined4*[m_someCount];
        field_0x20 = 0;
        field_0x24 = 0;
        field_0x44 = p2;
        field_0x48 = 1;
        field_0x4c = 0;
        field_0x50 = p3;
        field_0x54 = 0;
        field_0x58 = 0;
        field_0x5c = 0;

        memset(field_0x10, 0, sizeof(undefined4*) * m_someCount);

        audManager::get()->setNextSampleRateCode((SampleRateCode)1);
        audManager::get()->field_0x294 = 1;
        audManager::get()->field_0x298 = 0;
        audManager::get()->setMasterVolume(1.0f);
        audManager::get()->setPathBase("aud/audvag");
        loadSources();
    }

    Aud3DObjectManager::~Aud3DObjectManager() {
        instance = nullptr;

        if (field_0x10) delete [] field_0x10;
        field_0x10 = nullptr;

        for (u32 i = 0;i < m_sources.size();i++) delete m_sources[i];
        m_sources.clear();
    }

    void Aud3DObjectManager::FUN_001c1370() {
    }

    void Aud3DObjectManager::FUN_001c1428() {
    }

    void Aud3DObjectManager::FUN_001c14b0() {
    }
    
    void Aud3DObjectManager::FUN_00321ac0() {
        audManager::get()->FUN_00329f60(0x30, 0, field_0x44 ? 0 : 0x10, 3);
    }

    void Aud3DObjectManager::loadSources() {
        Stream* fp = datAssetManager::open("aud/sndmgr", "soundsources", "csv", 0, true);
        if (!fp) return;

        char line[1000] = { 0 };
        fgets(line, 1000, fp->getRaw());
        
        u32 index = 0;
        while (fgets(line, 1000, fp->getRaw()) != 0) {
            strtok(line, "\r\n");
            char* name = strtok(line, ",");
            i32 maxAudible = atoi(strtok(line, ","));
            i32 priority = atoi(strtok(line, ","));
            m_sources.push(new audSource(name, maxAudible, priority, index++));
        }

        delete fp;
    }

    void Aud3DObjectManager::method_0xf8() {
    }

    void Aud3DObjectManager::method_0x108() {
    }

    void Aud3DObjectManager::method_0x110() {
    }

    void Aud3DObjectManager::method_0x118() {
    }

    void Aud3DObjectManager::method_0x120() {
    }

    void Aud3DObjectManager::method_0x128() {
    }

    void Aud3DObjectManager::method_0x130() {
        m_something = 9.5f;
        m_invSomething = 1.0f / m_something;
        field_0x48 = 1;
        field_0x4c = 0;
        field_0x54 = 0;
        m_count = 0;
        field_0x24 = 0;

        if (!m_loadedSources) loadSources();

        if (!field_0x10) {
            field_0x10 = new undefined4*[m_someCount];
            memset(field_0x10, 0, sizeof(undefined4*) * m_someCount);
        }

        audManager* mgr = audManager::get();
        mgr->setPathBase("aud/audvag");
        mgr->field_0x294 = 1;
        mgr->field_0x298 = 0;
    }

    Aud3DObjectManager* Aud3DObjectManager::get() {
        return instance;
    }

    bool Aud3DObjectManager::isAlive() {
        return instance != nullptr;
    }
};