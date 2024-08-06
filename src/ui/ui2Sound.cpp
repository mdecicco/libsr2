#include <libsr2/ui/ui2Sound.h>
#include <libsr2/managers/Aud3DObjectManager.h>
#include <libsr2/audio/audSound.h>
#include <libsr2/managers/audManager.h>

#include <string.h>

namespace sr2 {
    ui2Sound::ui2Sound(const char* name, i32 p2, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        m_sound = nullptr;
        m_sfxIndex = -1;
        m_lastSfxIndex = -1;
        m_volume = 1.0f;
        field_0x88 = 0;

        init(p2);
    }

    ui2Sound::~ui2Sound() {
    }

    void ui2Sound::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) return;

        switch (event) {
            case WidgetEventType::UNK48: {
                if (Aud3DObjectManager::isAlive()) {
                    FUN_00207f78();
                }
                break;
            }
            case WidgetEventType::UNK33: {
                if (!Aud3DObjectManager::isAlive()) return;
                if (field_0x88) return;
                if (m_lastSfxIndex > 0) {
                    audSound* unk = audManager::getOffsetSFX(m_lastSfxIndex);
                    if (unk && unk->hasFlag(2) && unk->FUN_00328968()) {
                        return;
                    }
                }

                if (!m_sound || !m_sound->hasFlag(2) || !m_sound->FUN_00328968()) {
                    FUN_00207eb8();
                }
                break;
            }
            case WidgetEventType::UNK49: {
                if (Aud3DObjectManager::isAlive()) field_0x88 = 1;
                break;
            }
            case WidgetEventType::UNK50: {
                if (Aud3DObjectManager::isAlive()) field_0x88 = 0;
                break;
            }
            case WidgetEventType::UNK51: break;
            default: ui2Widget::onEvent(source, event, data);
        }
    }
    
    void ui2Sound::draw() {
    }

    const char* ui2Sound::getType() const {
        return "ui2Sound";
    }

    bool ui2Sound::isA(const char* type) const {
        if (strcmp("ui2Sound", type) == 0) return true;
        return ui2Widget::isA(type);
    }

    void ui2Sound::init(i32 p1) {
        if (!Aud3DObjectManager::isAlive() || !audManager::SomeManager) return;
        if (p1 < 0 || audManager::DAT_00362eac < 0) return;
        
        m_lastSfxIndex = m_sfxIndex;
        m_sfxIndex = p1;
        m_sound = audManager::getOffsetSFX(p1);

        if (m_sound) {
            FUN_00207fe0(m_volume);
        }
    }

    void ui2Sound::FUN_00207e20(const char* search) {
        if (!Aud3DObjectManager::isAlive() || !audManager::SomeManager) return;
        if (!search || audManager::DAT_00362eac < 0) return;

        m_lastSfxIndex = m_sfxIndex;
        m_sfxIndex = AudioManager::findSoundIndex(search);
        m_sound = audManager::getOffsetSFX(m_sfxIndex);

        if (m_sound) {
            FUN_00207fe0(m_volume);
        }
    }

    void ui2Sound::FUN_00207eb8() {
        if (!Aud3DObjectManager::isAlive() || !m_sound || field_0x88) return;
        setVolume(m_volume);

        if (!m_sound->FUN_00328968() || !m_sound->play() || !m_sound->hasFlag(2) || audManager::DAT_00362eb4 >= 4) return;

        audManager::DAT_00362ec8[audManager::DAT_00362eb4] = 0;
        audManager::DAT_00362eb8[audManager::DAT_00362eb4] = m_sfxIndex;
        audManager::DAT_00362eb4++;
    }

    void ui2Sound::FUN_00207f78() {
        if (!Aud3DObjectManager::isAlive() || !m_sound || field_0x88) return;
        m_sound->FUN_003287d0();
    }

    void ui2Sound::FUN_00207fc8() {
        field_0x88 = 1;
    }

    void ui2Sound::FUN_00207fd8() {
        field_0x88 = 0;
    }

    void ui2Sound::FUN_00207fe0(f32 p1) {
        m_volume = p1;
    }

    void ui2Sound::setVolume(f32 p1) {
        if (!Aud3DObjectManager::isAlive() || !m_sound || !audManager::SomeManager) return;

        audManager::AudioTypeCtrl* unk = audManager::get()->getAudioTypeCtrl(m_sound->getTypeIndex());
        m_sound->setVolume(p1 * unk->volume);
    }

    void ui2Sound::FUN_00208068(f32 p1) {
        if (!Aud3DObjectManager::isAlive() || !m_sound) return;

        m_sound->FUN_00328da8(p1);
    }
    
    void ui2Sound::FUN_002080b8(undefined4 p1) {
        if (!Aud3DObjectManager::isAlive() || !m_sound) return;

        m_sound->FUN_00329708(p1);
    }

    bool ui2Sound::FUN_00208108() {
        if (!Aud3DObjectManager::isAlive() || !m_sound || audManager::DAT_00362eb4 <= 0) return false;

        for (u32 i = 0;i < audManager::DAT_00362eb4;i++) {
            if (audManager::DAT_00362eb8[i] != m_sfxIndex) continue;
            if (audManager::DAT_00362ed8[i] != 1) return false;
            if (!m_sound->FUN_00328968()) return false;
            return true;
        }

        return false;
    }
};