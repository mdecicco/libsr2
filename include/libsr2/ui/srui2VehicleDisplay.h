#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>
#include <libsr2/gfx/ui.h>

namespace sr2 {
    class srui2VehicleDisplay : public ui2Visual {
        public:
            srui2VehicleDisplay(
                const char* name,
                i32 x,
                i32 y,
                i32 p4,
                const WidgetRef<ui2Master>& master
            );
            virtual ~srui2VehicleDisplay();

            void FUN_001e0f68(i32 p1, i32 p2);
        
        protected:
            uiRenderer::element* m_elem;
    };
};