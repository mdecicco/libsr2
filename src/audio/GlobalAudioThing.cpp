#include <libsr2/audio/GlobalAudioThing.h>
#include <libsr2/audio/SomeAudioThing6.h>
#include <libsr2/audio/SomeAudioThing7.h>

#include <string.h>

namespace sr2 {
    GlobalAudioThing* GlobalAudioThing::instance = nullptr;

    GlobalAudioThing::GlobalAudioThing() {
    }

    GlobalAudioThing::~GlobalAudioThing() {
    }

    SomeAudioThing6* GlobalAudioThing::getSomeAudioThing6(u32 idx) {
        return &m_sat6Arr[idx];
    }
    
    SomeAudioThing6* GlobalAudioThing::FUN_0032b7b8(const char* search, i32 idx, i32 p3) {
        u32 len = strlen(search);
        const char* filename = search + len - 1;

        while (filename != search) {
            if (*filename == '\\' || *filename == '/') {
                filename++;
                break;
            }

            filename--;
        }

        for (u32 i = idx;idx < m_someSat6Idx;idx++) {
            SomeAudioThing6& unk = m_sat6Arr[idx];
            if (!unk.field_0x54 || unk.field_0x20 == p3) continue;
            if (!strstr(unk.field_0x54->m_leftChannelPath, filename)) continue;

            return &unk;
        }

        return nullptr;
    }

    SomeAudioThing6* GlobalAudioThing::getSomeAudioThing6WithOffset(u32 idx) {
        return instance->getSomeAudioThing6(idx + instance->field_0x2c8);
    }

    i32 GlobalAudioThing::FUN_0032fcb0(const char* search) {
        SomeAudioThing6* unk = instance->FUN_0032b7b8(search, instance->field_0x2c8, -1);
        if (!unk) return -1;

        return unk->field_0x20;
    }

    GlobalAudioThing* GlobalAudioThing::get() {
        return instance;
    }
};