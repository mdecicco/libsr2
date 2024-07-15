#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    class ui2Input : public ui2Widget {
        public:
            ui2Input(const char* name, const WidgetRef<ui2Master>& master);
            virtual ~ui2Input();

            virtual void addListener(const ui::NamedRef& listener, WidgetEventType event, SomeWidgetCallback callback);
            virtual void removeListener(const ui::NamedRef& listener, WidgetEventType event);
            virtual void draw();

            u32 processController();
            void processKeyboard(u32* p1);
            void processWheel(u32* p1);
            void FUN_001ff4b8(u32 p1, u32* p2);
            void FUN_001ffba8(f32 ax, f32 ay);
            f32 FUN_001ffc18(i32 padIdx, i32 p2);
            void FUN_001fee50();
            f32 FUN_001fee98(u32 idx);

            static void FUN_001fe638();
        
        protected:
            f32 m_someDeadzone;
            f32 m_pressedAtTime[8];
            f32 m_pressedAmount[8];
            
            u32 field_0x78;
            undefined4 field_0x7c;
            f32 field_0x84;
            undefined4 field_0xa8[8];
            undefined4 field_0xe8[8];
    };
};