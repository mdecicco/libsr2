#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/uiUnknown25.h>

namespace sr2 {
    class ui2Menu : public ui2Widget {
        public:
            struct GridCell {
                ui2String str;
                undefined4 field_0x10;
                undefined4 field_0x14;
                undefined8 field_0x18;
                undefined4 field_0x20;
                WidgetEventType field_0x24;
                ui::BaseRef field_0x28;
                undefined4 field_0x30;
            };

            ui2Menu(const char* name, u32 rowCount, u32 colCount, const WidgetRef<ui2Master>& master);
            virtual ~ui2Menu();

            void init();

            virtual void reset();
            virtual void onEvent(const ui::BaseRef& p1, WidgetEventType p2, const ui::BaseRef& p3);

            void FUN_001f8218(const ui::NamedRef& p1, i32 row, i32 col, undefined4 p4, u64 p5);
            void FUN_001f8388(const char* p1, i32 row, i32 col, undefined4 p4, u64 p5);
            void FUN_001f84d8(i32 row, i32 col);
            void FUN_001f85c0(i32 row, i32 col, WidgetEventType p3, const ui::BaseRef& p4);
            void FUN_001f86c8(i32 row, i32 col, WidgetEventType p3, const ui::BaseRef& p4);
            void FUN_001f87d8(i32 row, i32 col, ui2Widget* p3);
            void FUN_001f88c8(i32 row, i32 col, ui2Widget* p3);
            void FUN_001f89b8(i32 row, i32 col, const char* p3);
            void FUN_001f88c8(i32 row, i32 col, const char* p3);
            void FUN_001f8b38(i32 row, i32 col, undefined4 p3);
            undefined4 FUN_001f8ee0(i32 row, i32 col);
            void FUN_001f8f08(i32 row, i32 col, i32 p3); // todo
            undefined4 FUN_001f9968(i32 row, i32 col);
            void FUN_001f9990(i32 row, i32 col);
            void FUN_001f9a28(const ui::BaseRef& p1); // todo
            void FUN_001fa298(const ui::BaseRef& p1); // todo
            bool FUN_001fa3d8(i32* outRow, i32* outCol, ui::BaseRef& outWidget);
            void FUN_001fa510(undefined4 p1);
            void FUN_001fa518(); // todo
            i32 FUN_001fad48();
            i32 FUN_001fae08();
            i32 FUN_001faed0();
            i32 FUN_001faf90();
            bool FUN_001fb050(i32* outRow, i32* outCol);
        
        protected:
            u32 m_rowCount;
            u32 m_colCount;
            u32 m_someRowIdx;
            u32 m_someColIdx;
            GridCell* m_grid;

            undefined4 field_0x8c;
            WidgetRef<uiUnknown25> field_0x90;
            WidgetRef<uiUnknown25> field_0x98;
    };
};