#include <libsr2/managers/AudioManager.h>
#include <libsr2/managers/audManager.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/audio/audSound.h>
#include <libsr2/audio/Cls_003310e0.h>
#include <libsr2/audio/Cls_00331090.h>

#include <stdio.h>

namespace sr2 {
    AudioManager* AudioManager::instance = nullptr;

    AudioManager::AudioManager(i32 someCount, i32 someOtherCount, i32 p3, Cls_003310e0* p4, i32 p5) {
        instance = this;

        m_somethingBig = p4 ? p4 : new Cls_003310e0(0x20, 0x100);

        audManager::create(someCount, p3, p5);

        m_someCount = someOtherCount;
        m_someArr = new Cls_00331090[m_someCount];
    }

    AudioManager::~AudioManager() {
        instance = nullptr;
        audManager::destroy();

        if (m_somethingBig) delete m_somethingBig;
        m_somethingBig = nullptr;
    }

    void AudioManager::method_0x10() {
    }

    void AudioManager::method_0x18() {
    }

    void AudioManager::method_0x20() {
    }

    void AudioManager::method_0x28() {
    }

    void AudioManager::method_0x30() {
    }

    void AudioManager::method_0x38() {
    }

    void AudioManager::method_0x40() {
    }

    void AudioManager::method_0x48() {
    }

    void AudioManager::method_0x50() {
    }

    void AudioManager::method_0x58() {
    }

    void AudioManager::method_0x60() {
    }

    void AudioManager::method_0x68() {
    }

    void AudioManager::method_0x70() {
    }

    void AudioManager::method_0x78() {
    }

    u32 AudioManager::loadSoundBanks(undefined4 p1) {
        m_somethingBig->field_0x0 = 0;
        m_somethingBig->field_0xc[0] = 0;
        m_somethingBig->m_unk0Count = 0;
        m_somethingBig->m_unk1Count = 0;
        
        u32 count = m_somethingBig->parseBanks(p1, "BNKINDEX.CSV");
        i32 ret = 0;
        for (u32 i = 0;i < count;i++) {
            ret += m_somethingBig->loadBank(m_somethingBig->m_bankFilenames[i]);
        }

        return ret;
    }

    u32 AudioManager::loadSoundBanksAtPath(const char* p1, undefined4 p2) {
        snprintf(audManager::get()->m_somePath, 128, "%s%s/", datAssetManager::getPath(), p1);
        return loadSoundBanks(p2);
    }

    void AudioManager::method_0x90(const char* p1, undefined4 p2) {
    }

    void AudioManager::method_0x98() {
    }

    void AudioManager::method_0xa0(undefined4 p1, undefined4 p2, i32 p3, undefined4 p4, i32 p5) {
    }

    void AudioManager::method_0xa8() {
    }

    void AudioManager::method_0xb0() {
    }

    void AudioManager::method_0xb8() {
    }

    void AudioManager::method_0xc0() {
    }

    void AudioManager::method_0xc8() {
    }

    void AudioManager::method_0xd0() {
    }

    void AudioManager::method_0xd8() {
    }

    void AudioManager::method_0xe0() {
    }

    void AudioManager::method_0xe8() {
    }

    void AudioManager::method_0xf0() {
    }

    


    i32 AudioManager::findSoundIndex(const char* filename) {
        audManager* g = audManager::get();
        audSound* unk = g->findSFX(filename, g->m_reservedSfxCount, -1);
        if (!unk) return -1;

        return unk->getIndex();
    }

    void AudioManager::FUN_0032f9e8(i32 p1) {
        audManager::get()->FUN_0032a0c8(p1);
    }

    AudioManager* AudioManager::get() {
        return instance;
    }

    bool AudioManager::isAlive() {
        return instance != nullptr;
    }
};