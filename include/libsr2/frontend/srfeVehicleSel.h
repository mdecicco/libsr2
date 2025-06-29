#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>
#include <libsr2/ui/ui2Master.h>
#include <libsr2/ui/ui2Input.h>
#include <libsr2/ui/srui2CycleMenu.h>
#include <libsr2/ui/srui2ButtonMenu.h>
#include <libsr2/ui/ui2Menu.h>
#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/srui2VehicleDisplay.h>
#include <libsr2/ui/ui2Sound.h>
#include <libsr2/ui/ui2Image.h>
#include <libsr2/ui/ui2TranslatedText.h>
#include <libsr2/ui/ui2DynamicText.h>

namespace sr2 {
    class srfeVehicleSel : public srfeBasicLayout2 {
        public:
            srfeVehicleSel(
                const char* name,
                const char* prevScreenName,
                const char* nextScreenName,
                const WidgetRef<ui2Master>& master
            );
            ~srfeVehicleSel();

            virtual void setActive(bool active);
            virtual void initScreen();
            virtual void deinitScreen();

            void interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
        
        protected:
            struct Widgets {
                WidgetRef<ui2Master> master;
                WidgetRef<ui2Input> vehicleInput1;
                WidgetRef<srui2CycleMenu> p1TeamMenu;
                WidgetRef<srui2CycleMenu> p1VehicleMenu;
                WidgetRef<srui2CycleMenu> p1MateMenu1;
                WidgetRef<srui2CycleMenu> p1MateMenu2;
                WidgetRef<srui2CycleMenu> p1MateMenu3;
                WidgetRef<ui2Menu> menu1;
                WidgetRef<ui2Table> menu1Tbl;
                WidgetRef<ui2Table> menu1CareerTbl;
                WidgetRef<srui2VehicleDisplay> vehicleDisplay1;
                WidgetRef<ui2Sound> vehicleLockedL;
                WidgetRef<ui2Sound> vehicleLockedR;
                WidgetRef<ui2Sound> vehicleTeamDisp1L;
                WidgetRef<ui2Sound> vehicleTeamDisp1R;
                WidgetRef<ui2Image> valueTextBackdrop;
                WidgetRef<ui2Image> valueBackdrop;
                WidgetRef<ui2Image> menuLeftBackdrop;
                WidgetRef<ui2Image> menuMiddleBackdrop;
                WidgetRef<ui2Image> menuRightBackdrop;
                WidgetRef<ui2Image> carBackdrop;
                WidgetRef<ui2Table> vehicleValue1Tbl;
                WidgetRef<ui2TranslatedText> vehicleHeading1;
                WidgetRef<ui2TranslatedText> vehicleDesc1;
                WidgetRef<ui2Input> transInput1;
                WidgetRef<srui2ButtonMenu> trans1SelectionMenu;
                WidgetRef<ui2TranslatedText> transReadyText;
                WidgetRef<ui2Image> cursor1Img;
                WidgetRef<ui2Timer> cursor1Timer;
                WidgetRef<ui2Table> trans1Tbl;
            };

            const char* m_prevScreenName;
            const char* m_nextScreenName;

            undefined4 field_0xb8;
            undefined4 field_0xc4;
            undefined4 field_0xc8;

            static Widgets* WidgetsInstance;
            static u32 ActiveCount;
            static u32 InitializedCount;
            void initWidgets();
            static void initWidgetsSub1();
            static void initWidgetsSub2();
    };
};