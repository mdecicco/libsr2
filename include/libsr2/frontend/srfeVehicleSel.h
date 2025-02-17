#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>
#include <libsr2/ui/ui2Master.h>

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
        
        protected:
            struct Widgets {
                WidgetRef<ui2Master> master;
            };

            const char* m_prevScreenName;
            const char* m_nextScreenName;

            undefined4 field_0xb8;
            undefined4 field_0xc4;
            undefined4 field_0xc8;

            static Widgets* WidgetsInstance;
            static u32 ActiveCount;
            static u32 InitializedCount;
    };
};