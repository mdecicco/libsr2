#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Position.h>

namespace sr2 {
    class ui2Menu;

    class ui2Table : public ui2Widget {
        public:
            struct Cell {
                ui2String value;
                vec2i offset;
                WidgetEventCallback callback;
            };

            struct RowColInfo {
                i32 size;
                i32 offset;
            };

            ui2Table(const char* name, u32 colCount, u32 rowCount, i32 x, i32 y, const WidgetRef<ui2Master>& master);
            virtual ~ui2Table();

            void init(u32 rowCount, u32 colCount);

            virtual void draw();
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void method_0x58();
            virtual void configureParser(datParser* parser);

            void FUN_001fc540(i32 x, i32 y);
            void FUN_001fc660(const WidgetRef<ui2Position>& p1);
            void FUN_001fc6f8(const ui::NamedRef& p1, u32 col, u32 row, WidgetEventCallback p4);
            void FUN_001fc818(const char* p1, u32 col, u32 row, WidgetEventCallback p4);
            void FUN_001fc958(u32 col, u32 row);
            void FUN_001fc9f0(u32 col, u32 row, const std::initializer_list<ui2Widget*>& args);
            void FUN_001fcae0(u32 col, u32 row, const std::initializer_list<ui2Widget*>& args);
            void FUN_001fcbd0(u32 col, u32 row, const std::initializer_list<const char*>& args);
            void FUN_001fcc88(u32 col, u32 row, const std::initializer_list<const char*>& args);
            void setColSizes(i32 size, i32 colBegin, i32 colEnd);
            void setColSize(i32 size, i32 col);
            i32 getColSize(u32 col);
            void setRowSizes(i32 size, i32 rowBegin, i32 rowEnd);
            void setRowSize(i32 size, i32 row);
            i32 getRowSize(u32 row);
            vec2i getTableSize();
            void setCellOffsets(i32 offsetX, i32 offsetY, i32 beginCol, i32 endCol, i32 beginRow, i32 endRow);
            void setCellOffset(i32 offsetX, i32 offsetY, i32 row, i32 col);
            void setBounds(i32 width, i32 height);
            void setBoundsFromPos(i32 x, i32 y);
            void FUN_001fd0a8(i32 p1, i32 p2, i32 p3, i32 p4);
            void FUN_001fd0c8(u32 p1);
            void FUN_001fd0d8(u32 col, u32 row);
            void FUN_001fd0f0(u32* outCol, u32* outRow);
            void FUN_001fd108(i32 p1, i32 p2);
            void FUN_001fd120(i32* p1, i32* p2);
            bool FUN_001fd138();
            void FUN_001fd178(u32 col, u32 row);
            void FUN_001fd1d0(u32 col, u32 row);
            void recalculateOffsets();
            void FUN_001fd690();
            bool FUN_001fdd88();
        
        protected:
            u32 m_rowCount;
            u32 m_colCount;
            vec2i m_pos;
            vec2i m_bounds;
            Cell* m_cells;
            RowColInfo* m_rowInfo;
            RowColInfo* m_colInfo;
            u32 m_someRowIdx;
            u32 m_someColIdx;

            bool m_needsRecalculation;
            vec2i field_0xa0;
            undefined4 field_0xa8;
            vec2i field_0xac;
            vec2i field_0xb4;
            WidgetRef<ui2Position> field_0xc4;
            WidgetRef<ui2Position> field_0xcc;
    };
};