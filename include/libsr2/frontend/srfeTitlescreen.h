#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/srui2SRScreen.h>
#include <libsr2/ui/srui2Bitmap.h>
#include <libsr2/ui/ui2Image.h>
#include <libsr2/ui/ui2Timer.h>
#include <libsr2/ui/ui2Sound.h>

namespace sr2 {
    class srfeTitlescreen : public srui2SRScreen {
        public:
            srfeTitlescreen(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeTitlescreen();

            virtual void initScreen();
            virtual void deinitScreen();
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;
            virtual void setActive(bool isActive);
            void interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);

            void FUN_00117ad8();

            static void FUN_00117560();
            static void FUN_001175c8();
            static void FUN_00117718(const char* p1, undefined4 p2);
            static void FUN_00117800(undefined4 p1);
            static void Activate();
        
        protected:
            static undefined4 DAT_0035ef70;

            WidgetRef<srui2Bitmap> m_backgroundImg;
            WidgetRef<ui2Timer> m_inactivityTimer;
            WidgetRef<ui2Image> m_cursorImg;
            WidgetRef<ui2Timer> m_cursorTimer;
            WidgetRef<ui2Sound> m_cursorLeftSound;
            WidgetRef<ui2Sound> m_cursorRightSound;

            undefined4 field_0xa0;
            undefined4 field_0xa4;
            undefined4 field_0xa8;
            undefined4 field_0xac;
    };
};