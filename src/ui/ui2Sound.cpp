#include <libsr2/ui/ui2Sound.h>
#include <libsr2/managers/Aud3DObjectManager.h>
#include <libsr2/audio/SomeAudioThing6.h>
#include <libsr2/audio/GlobalAudioThing.h>

#include <string.h>

namespace sr2 {
    // todo: default values?
    i32 ui2Sound::DAT_00362ea0 = 0;
    i32 ui2Sound::DAT_00362eac = 0;
    i32 ui2Sound::DAT_00362eb4 = 0;
    i32 ui2Sound::DAT_00362eb8[4] = { 0 };
    i32 ui2Sound::DAT_00362ec8[4] = { 0 };
    i32 ui2Sound::DAT_00362ed8[4] = { 0 };

    ui2Sound::ui2Sound(char* name, i32 p2, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        field_0x78 = nullptr;
        field_0x7c = -1;
        field_0x80 = -1;
        field_0x84 = 1.0f;
        field_0x88 = 0;

        FUN_00207da0(p2);
    }

    ui2Sound::~ui2Sound() {
    }

    void ui2Sound::method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3) {
        if (!field_0x1c) return;

        switch (p2) {
            case SOME_WIDGET_ENUM::UNK48: {
                if (Aud3DObjectManager::isAlive()) {
                    FUN_00207f78();
                }
                break;
            }
            case SOME_WIDGET_ENUM::UNK33: {
                if (!Aud3DObjectManager::isAlive()) return;
                if (field_0x88) return;
                if (field_0x80 > 0) {
                    SomeAudioThing6* unk = GlobalAudioThing::getSomeAudioThing6WithOffset(field_0x80);
                    if (unk && unk->hasSomeFlag0() && unk->FUN_00328968()) {
                        return;
                    }
                }

                if (!field_0x78 || !field_0x78->hasSomeFlag0() || !field_0x78->FUN_00328968()) {
                    FUN_00207eb8();
                }
                break;
            }
            case SOME_WIDGET_ENUM::UNK49: {
                if (Aud3DObjectManager::isAlive()) field_0x88 = 1;
                break;
            }
            case SOME_WIDGET_ENUM::UNK50: {
                if (Aud3DObjectManager::isAlive()) field_0x88 = 0;
                break;
            }
            case SOME_WIDGET_ENUM::UNK51: break;
            default: ui2Widget::method_0x30(p1, p2, p3);
        }
    }
    
    void ui2Sound::draw() {
    }

    const char* ui2Sound::getType() const {
        return "ui2Sound";
    }

    bool ui2Sound::isA(const char* type) const {
        if (strcmp("ui2Sound", type) == 0) return true;
        return strcmp(getType(), type) == 0;
    }

    void ui2Sound::FUN_00207da0(i32 p1) {
        if (!Aud3DObjectManager::isAlive() || !DAT_00362ea0) return;
        if (p1 < 0 || DAT_00362eac < 0) return;
        
        field_0x80 = field_0x7c;
        field_0x7c = p1;
        field_0x78 = GlobalAudioThing::getSomeAudioThing6WithOffset(p1);

        if (field_0x78) {
            FUN_00207fe0(field_0x84);
        }
    }

    void ui2Sound::FUN_00207e20(const char* search) {
        if (!Aud3DObjectManager::isAlive() || !DAT_00362ea0) return;
        if (!search || DAT_00362eac < 0) return;

        field_0x80 = field_0x7c;
        field_0x7c = GlobalAudioThing::FUN_0032fcb0(search);
        field_0x78 = GlobalAudioThing::getSomeAudioThing6WithOffset(field_0x7c);

        if (field_0x78) {
            FUN_00207fe0(field_0x84);
        }
    }

    void ui2Sound::FUN_00207eb8() {
        if (!Aud3DObjectManager::isAlive() || !field_0x78 || field_0x88) return;
        FUN_00207fe8(field_0x84);

        if (!field_0x78->FUN_00328968() || !field_0x78->hasSomeFlag0() || DAT_00362eb4 >= 4) return;

        DAT_00362ec8[DAT_00362eb4] = 0;
        DAT_00362eb8[DAT_00362eb4] = field_0x7c;
        DAT_00362eb4++;
    }

    void ui2Sound::FUN_00207f78() {
        if (!Aud3DObjectManager::isAlive() || !field_0x78 || field_0x88) return;
        field_0x78->FUN_003287d0();
    }

    void ui2Sound::FUN_00207fc8() {
        field_0x88 = 1;
    }

    void ui2Sound::FUN_00207fd8() {
        field_0x88 = 0;
    }

    void ui2Sound::FUN_00207fe0(f32 p1) {
        field_0x84 = p1;
    }

    void ui2Sound::FUN_00207fe8(f32 p1) {
        if (!Aud3DObjectManager::isAlive() || !field_0x78 || !DAT_00362ea0) return;

        GlobalAudioThing::Unk0& unk = GlobalAudioThing::get()->m_unk0[field_0x78->field_0x2c - 1];
        field_0x78->FUN_00329078(p1 * unk.field_0xc);
    }

    void ui2Sound::FUN_00208068(f32 p1) {
        if (!Aud3DObjectManager::isAlive() || !field_0x78) return;

        field_0x78->FUN_00328da8(p1);
    }
    
    void ui2Sound::FUN_002080b8(undefined4 p1) {
        if (!Aud3DObjectManager::isAlive() || !field_0x78) return;

        field_0x78->FUN_00329708(p1);
    }

    bool ui2Sound::FUN_00208108() {
        if (!Aud3DObjectManager::isAlive() || !field_0x78 || DAT_00362eb4 <= 0) return false;

        for (u32 i = 0;i < DAT_00362eb4;i++) {
            if (DAT_00362eb8[i] != field_0x7c) continue;
            if (DAT_00362ed8[i] != 1) return false;
            if (!field_0x78->FUN_00328968()) return false;
            return true;
        }

        return false;
    }
};