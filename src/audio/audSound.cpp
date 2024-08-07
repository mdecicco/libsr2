#include <libsr2/audio/audSound.h>
#include <libsr2/audio/audSoundInfo.h>
#include <libsr2/audio/audPlayer.h>
#include <libsr2/managers/audManager.h>

#include <string.h>

namespace sr2 {
    audSound::audSound() {
        m_info = nullptr;
        m_player = nullptr;
        init();
    }

    audSound::~audSound() {
        // todo

        if (m_player) delete m_player;
        m_player = nullptr;
    }

    void audSound::init() {
        m_index = 0;
        m_refCount = 0;
        field_0x0 = { 0.0f, 0.0f, 0.0f };
        m_audioType = AudioTypeIndex::None;
        m_balance = 0.0f;
        m_unk1Index = 0;
        field_0x1c = 0;
        field_0x3c = 1.0f;
        m_volume = 1.0f;
        field_0x40 = 99.0f;
        m_radiusSquared = 10000.0f;
        field_0x28 = audManager::get()->field_0x2d4;
        field_0x50 = m_volume;
        m_leftChannelFilenameIdx = 0x3c;
        m_rightChannelFilenameIdx = 0;
        field_0x18 = 0x7f;
        field_0x19 = 0x40;
        field_0x1a = 0;
        field_0x1b = 0;
        m_flags = 0;
        m_pitch = 0;
        m_sampleRateCode = SampleRateCode::srZero;

        if (m_info) {
            delete m_info;
            m_info = nullptr;
        }
    }

    i32 audSound::load(const char* filename) {
        audManager* gat = audManager::get();
        
        if (!gat) {
            reset();
            return 0xFFFFFFFF;
        }

        if (gat->m_sfxCount >= gat->m_sfxCapacity - gat->m_reservedSfxCount) {
            reset();
            return 0xFFFFFFFF;
        }

        char path[512] = { 0 };
        strcpy(path, filename);
        
        if (gat->FUN_0032c9d0() == 0) {
            if (gat->m_nextSampleRateCode > SampleRateCode::srZero) {
                strcpy(path, gat->getPathPrefix());
                strcat(path, "\\");
                strcat(path, gat->getPathBase());
                strcat(path, filename);
            } else {
                strcpy(path, gat->getPathBase());
                strcat(path, filename);
            }
            
        } else if (gat->m_nextSampleRateCode < SampleRateCode::sr22050) {
            const char* tmp = strstr(gat->getPathBase(), "AUD");
            if (!tmp) tmp = strstr(gat->getPathBase(), "aud");

            if (!tmp) strcpy(path, gat->getPathBase());
            else strcpy(path, tmp);
            strcat(path, filename);
        } else {
            strcpy(path, gat->getPathPrefix());
            strcat(path, gat->getPathBase());
            strcat(path, filename);
        }

        const char* tmp = strstr(filename, ".VAG");
        if (!tmp) tmp = strstr(filename, ".vag");
        if (!tmp) strcat(path, ".VAG");
        
        if (gat->m_nextIndex == 0) m_index = gat->getFreeIndex();
        else m_index = gat->m_nextIndex;
        
        gat->m_lastLoaded = nullptr;
        if (!m_info) m_info = new audSoundInfo();
        
        if (gat->m_nextSampleRateCode > SampleRateCode::srZero) {
            m_sampleRateCode = gat->m_nextSampleRateCode;
            if (m_sampleRateCode == SampleRateCode::srDual44100) {
                m_sampleRateCode = SampleRateCode::sr44100;

                memset(m_info->m_leftChannelPath, 0, 128);
                strncpy(m_info->m_leftChannelPath, path, 128);
                strncat(m_info->m_leftChannelPath, "L.VAG", 128);

                memset(m_info->m_rightChannelPath, 0, 128);
                strncpy(m_info->m_rightChannelPath, path, 128);
                strncat(m_info->m_rightChannelPath, "R.VAG", 128);
                
                i32 filenameIdx = gat->addFile(u32(m_sampleRateCode) - u32(SampleRateCode::srZero), m_info->m_leftChannelPath);
                if (filenameIdx == -1) {
                    reset();
                    return 0xFFFFFFFF;
                }

                m_leftChannelFilenameIdx = filenameIdx;
                
                gat->m_filenameCount++;
                
                filenameIdx = gat->addFile(u32(m_sampleRateCode) - u32(SampleRateCode::srZero), m_info->m_rightChannelPath);
                if (filenameIdx == -1) {
                    reset();
                    return 0xFFFFFFFF;
                }

                m_rightChannelFilenameIdx = filenameIdx;
            }
            else {
                strcpy(m_info->m_leftChannelPath, path);
                m_info->m_rightChannelPath[0] = '\0';
                
                i32 filenameIdx = gat->addFile(u32(m_sampleRateCode) - u32(SampleRateCode::srZero), m_info->m_leftChannelPath);
                if (filenameIdx == -1) {
                    reset();
                    return 0xFFFFFFFF;
                }

                m_rightChannelFilenameIdx = 0;
                m_leftChannelFilenameIdx = filenameIdx;
            }

            gat->m_filenameCount++;
            setFlags(4);
            field_0x18 = 0;
            field_0x19 = 0;
            m_info->m_owner = this;
            m_info->m_flags = m_flags & 1;
            m_info->m_memOffset = -1;
            m_info->m_sampleRate = 1;
            m_info->m_channelSize = 0xffffffff;
        } else if (gat->field_0x294) {
            audSound* existing = gat->findSFX(filename, gat->m_reservedSfxCount, m_index);
            if (existing) {
                m_index = existing->m_index;
                m_flags = existing->m_flags;
                m_rightChannelFilenameIdx = existing->m_rightChannelFilenameIdx;
                field_0x18 = existing->field_0x18;
                field_0x19 = existing->field_0x19;

                m_leftChannelFilenameIdx = 0;
                m_sampleRateCode = SampleRateCode::srUnk1;
                existing->m_refCount++;
                if (existing->field_0x28 < existing->m_refCount) {
                    existing->field_0x28 = existing->m_refCount;
                }
                
                gat->setReference(existing, this);
                if (m_info->m_flags != 0) setFlags(1);
                
                gat->m_lastLoaded = this;
                gat->m_sfxCount++;
                return m_index;
            }
        }

        if (m_sampleRateCode < SampleRateCode::sr22050) {
            if (!gat->loadVAG(this, path)) {
                reset();
                return 0xffffffff;
            }
        }

        if (m_info->m_flags != 0) setFlags(1);
        
        gat->m_lastLoaded = this;
        gat->m_sfxCount++;
        gat->m_debugFilenames.push(filename);
        return m_index;
    }

    u32 audSound::getIndex() const {
        return m_index;
    }
    
    AudioTypeIndex audSound::getTypeIndex() const {
        return m_audioType;
    }

    void audSound::reset() {
        init();
    }

    void audSound::setFlags(u32 flags) {
        m_flags = flags;

        if (m_player) m_player->setDoesLoop(flags & 1);
    }

    bool audSound::isUsed() {
        if (!m_info) return false;
        if (intptr_t(m_info) == 0xFFFFFFFF || intptr_t(m_info) < 0xFF) abort();
        if (m_info->m_memOffset == 0) return false;

        return true;
    }

    void audSound::FUN_00328220() {
        // todo
    }

    void audSound::FUN_003287d0() {
        // todo
    }

    bool audSound::FUN_00328968() {
        // todo
        return true;
    }
    
    void audSound::FUN_00328da8(f32 p1) {
        // todo
    }

    void audSound::setVolume(f32 p1) {
        // todo
        if (m_player) {
            m_player->setGain(p1);
        }
    }
    
    void audSound::setVolumeOnly(f32 volume) {
        m_volume = volume;
        if (m_player) m_player->setGain(volume);
    }
    
    void audSound::setRadius(f32 radius) {
        m_radiusSquared = radius * radius;
        if (m_player) m_player->setMaxDistance(radius);
    }
    
    void audSound::FUN_00329708(undefined4 p1) {
        // todo
    }

    bool audSound::play() {
        if (m_player && !m_player->isPlaying()) m_player->play();
        return true;
    }

    bool audSound::hasFlag(u32 bit) const {
        return ((m_flags >> bit) & 1) != 0;
    }
};