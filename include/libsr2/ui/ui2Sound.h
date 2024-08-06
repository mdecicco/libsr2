#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    class audSound;

    class ui2Sound : public ui2Widget {
        public:
            ui2Sound(const char* name, i32 p2, const WidgetRef<ui2Master>& master);
            virtual ~ui2Sound();

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void draw();
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;

            void init(i32 p1);
            void FUN_00207e20(const char* search);
            void FUN_00207eb8();
            void FUN_00207f78();
            void FUN_00207fc8();
            void FUN_00207fd8();
            void FUN_00207fe0(f32 p1);
            void setVolume(f32 p1);
            void FUN_00208068(f32 p1);
            void FUN_002080b8(undefined4 p1);
            bool FUN_00208108();
        
        protected:
            audSound* m_sound;
            i32 m_sfxIndex;
            i32 m_lastSfxIndex;
            f32 m_volume;
            undefined4 field_0x88;
    };
};