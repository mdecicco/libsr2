#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/io/datParser.h>

#include <stdio.h>

namespace sr2 {
    ui2Table::ui2Table(
        const char* name,
        u32 colCount,
        u32 rowCount,
        i32 x,
        i32 y,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        m_pos = { x, y };
        m_bounds = { 9999, 9999 };
        m_colCount = colCount;
        m_rowCount = rowCount;
        m_cells = nullptr;
        m_colInfo = nullptr;
        m_rowInfo = nullptr;

        field_0x8c = false;
        field_0xa0.x = 0;
        field_0xa0.y = 0;
        field_0xa8 = 0;
        field_0xac = { 5000, 5000 };
        field_0xb4 = { 5000, 5000 };
        m_someRowIdx = 0;
        m_someColIdx = 0;

        init(colCount, rowCount);
    }

    ui2Table::~ui2Table() {
        if (m_cells) delete [] m_cells;
        if (m_colInfo) delete [] m_colInfo;
        if (m_rowInfo) delete [] m_rowInfo;
    }

    void ui2Table::init(u32 colCount, u32 rowCount) {
        u32 cellCount = colCount * rowCount;
        m_cells = new Cell[cellCount];
        m_colInfo = new RowColInfo[colCount];
        m_rowInfo = new RowColInfo[rowCount];

        for (u32 i = 0;i < cellCount;i++) {
            m_cells[i].offset = { 0, 0 };
            m_cells[i].field_0x18 = 0;
        }

        for (u32 i = 0;i < colCount;i++) {
            m_colInfo[i].size = 20;
            m_colInfo[i].offset = 0;
        }

        for (u32 i = 0;i < rowCount;i++) {
            m_rowInfo[i].size = 20;
            m_rowInfo[i].offset = 0;
        }

        field_0xc4 = new ui2Position();
        field_0xc4->pos = { 0, 0 };

        field_0xcc = new ui2Position();
        field_0xcc->pos = { 0, 0 };

        addToMasterUnk0(10000, 0x80090000);
        field_0x48 = 1;
        field_0x8c = true;
    }
    
    void ui2Table::draw() {
        ui2Widget::draw();

        if (field_0x1c) FUN_001fd138();
    }
    
    void ui2Table::onEvent(const ui::BaseRef& p1, WidgetEventType p2, const ui::BaseRef& p3) {
        if (!field_0x1c || !p3) {
            ui2Widget::onEvent(p1, p2, p3);
            return;
        }

        if (p2 == WidgetEventType::SetPosition) {
            FUN_001fc660(p3.cast<ui2Position>());
            return;
        }
        
        if (p2 == WidgetEventType::UNK29) {
            // FUN_001fd0d8(?, ?)
            return;
        }
        
        ui2Widget::onEvent(p1, p2, p3);
    }

    void ui2Table::method_0x58() {
        FUN_001fdd88();
    }
    
    void ui2Table::prepParserAgain(datParser* parser) {
        char buf[160];

        snprintf(buf, 160, "%s.PosX", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.x, 1, nullptr);
        snprintf(buf, 160, "%s.PosY", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.y, 1, nullptr);
        snprintf(buf, 160, "%s.BBoxWidth", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_bounds.x, 1, nullptr);
        snprintf(buf, 160, "%s.BBoxHeight", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_bounds.y, 1, nullptr);

        for (u32 i = 0;i < m_colCount;i++) {
            snprintf(buf, 160, "%s.Col:%x.Width", m_widgetName->get(), i);
            parser->add(PARSE_TYPE::INT32, buf, &m_colInfo[i].size, 1, nullptr);
        }

        for (u32 i = 0;i < m_rowCount;i++) {
            snprintf(buf, 160, "%s.Row:%x.Height", m_widgetName->get(), i);
            parser->add(PARSE_TYPE::INT32, buf, &m_rowInfo[i].size, 1, nullptr);
        }

        for (u32 c = 0;c < m_colCount;c++) {
            for (u32 r = 0;r < m_rowCount;r++) {
                Cell& cell = m_cells[m_rowCount * c + r];
                snprintf(buf, 160, "%s.Row:%x.Col:%x.OffsetX", m_widgetName->get(), r, c);
                parser->add(PARSE_TYPE::INT32, buf, &cell.offset.x, 1, nullptr);
                snprintf(buf, 160, "%s.Row:%x.Col:%x.OffsetY", m_widgetName->get(), r, c);
                parser->add(PARSE_TYPE::INT32, buf, &cell.offset.y, 1, nullptr);
            }
        }

        FUN_001fdd88();
    }



    void ui2Table::FUN_001fc540(i32 x, i32 y) {
        field_0xc4->pos = { x, y };
        FUN_001fc660(field_0xc4);
    }
    
    void ui2Table::FUN_001fc660(const WidgetRef<ui2Position>& p1) {
        vec2i newPos = p1->pos;

        if (m_pos.x == newPos.x && m_pos.y == newPos.y) return;

        m_pos = newPos;

        ui::BaseRef w;
        method_0x98(WidgetEventType::UNK11, p1, w);

        field_0x8c = true;
        FUN_001fd138();
    }
    
    void ui2Table::FUN_001fc6f8(const ui::NamedRef& p1, u32 col, u32 row, u64 p4) {
        Cell& cell = m_cells[m_rowCount * col + row];
        cell.value.set(p1->getName());
        cell.field_0x18 = p4;

        FUN_001fd178(col, row);
    }

    void ui2Table::FUN_001fc818(const char* p1, u32 col, u32 row, u64 p4) {
        ui::BaseRef w = ui2Base::getGlobalMaster()->findWidget(p1);
        if (!w) return;

        FUN_001fc6f8(w.cast<ui2Widget>(), col, row, p4);
    }
    
    void ui2Table::FUN_001fc958(u32 col, u32 row) {
        Cell& cell = m_cells[m_rowCount * col + row];
        cell.value.set("");
        cell.field_0x18 = 0;
    }
    
    void ui2Table::FUN_001fc9f0(u32 col, u32 row, const std::initializer_list<ui2Widget*>& args) {
        for (ui2Widget* w : args) {
            if (!w) return;
            
            FUN_001fc6f8(w, col, row, 0x800080000);
            
            col++;
            if (col >= m_colCount) {
                col = 0;
                row++;
            }
        }
    }
    
    void ui2Table::FUN_001fcae0(u32 col, u32 row, const std::initializer_list<ui2Widget*>& args) {
        for (ui2Widget* w : args) {
            if (!w) return;
            
            FUN_001fc6f8(w, col, row, 0x800080000);
            
            row++;
            if (row >= m_rowCount) {
                row = 0;
                col++;
            }
        }
    }
    
    void ui2Table::FUN_001fcbd0(u32 col, u32 row, const std::initializer_list<const char*>& args) {
        for (const char* w : args) {
            if (!w) return;
            
            FUN_001fc818(w, col, row, 0x800080000);
            
            col++;
            if (col >= m_colCount) {
                col = 0;
                row++;
            }
        }
    }
    
    void ui2Table::FUN_001fcc88(u32 col, u32 row, const std::initializer_list<const char*>& args) {
        for (const char* w : args) {
            if (!w) return;
            
            FUN_001fc818(w, col, row, 0x800080000);
            
            row++;
            if (row >= m_rowCount) {
                row = 0;
                col++;
            }
        }
    }
    
    void ui2Table::setColSizes(i32 size, i32 colBegin, i32 colEnd) {
        u32 begin = 0;
        if (colBegin != -1) begin = colBegin;
        
        u32 end = m_colCount - 1;
        if (colEnd != -2) end = colEnd;

        for (u32 i = begin;i <= end;i++) {
            m_colInfo[i].size = size;
        }

        field_0x8c = true;
    }
    
    void ui2Table::setColSize(i32 size, i32 col) {
        setColSizes(size, col, col);
    }
    
    i32 ui2Table::getColSize(u32 col) {
        return m_colInfo[col].size;
    }

    void ui2Table::setRowSizes(i32 size, i32 rowBegin, i32 rowEnd) {
        u32 begin = 0;
        if (rowBegin != -1) begin = rowBegin;
        
        u32 end = m_rowCount - 1;
        if (rowEnd != -2) end = rowEnd;

        for (u32 i = begin;i <= end;i++) {
            m_rowInfo[i].size = size;
        }

        field_0x8c = true;
    }
    
    void ui2Table::setRowSize(i32 size, i32 row) {
        setRowSizes(size, row, row);
    }
    
    i32 ui2Table::getRowSize(u32 row) {
        return m_rowInfo[row].size;
    }

    vec2i ui2Table::getTableSize() {
        vec2i sz = { 0, 0 };

        for (u32 i = 0;i < m_colCount;i++) sz.x += m_colInfo->size;
        for (u32 i = 0;i < m_rowCount;i++) sz.y += m_rowInfo->size;

        return sz;
    }

    void ui2Table::setCellOffsets(i32 offsetX, i32 offsetY, i32 beginCol, i32 endCol, i32 beginRow, i32 endRow) {
        if (beginCol == -1) beginCol = 0;
        if (endCol == -2) endCol = m_colCount - 1;
        if (beginRow == -1) beginRow = 0;
        if (endRow == -2) endRow = m_rowCount - 1;

        for (u32 c = beginCol;c <= endCol;c++) {
            for (u32 r = beginRow;r <= endRow;r++) {
                m_cells[m_rowCount * c + r].offset = { offsetX, offsetY };
                FUN_001fd178(c, r);
            }
        }
    }
    
    void ui2Table::setCellOffset(i32 offsetX, i32 offsetY, i32 col, i32 row) {
        setCellOffsets(offsetX, offsetY, col, col, row, row);
    }

    void ui2Table::setBounds(i32 width, i32 height) {
        m_bounds = { width, height };
        field_0x8c = true;
    }
    
    void ui2Table::setBoundsFromPos(i32 x, i32 y) {
        setBounds(x - m_pos.x, y - m_pos.y);
    }
    
    void ui2Table::FUN_001fd0a8(i32 p1, i32 p2, i32 p3, i32 p4) {
        field_0xac = { p1, p2 };
        field_0xb4 = { p3, p4 };
        field_0x8c = true;
    }
    
    void ui2Table::FUN_001fd0c8(u32 p1) {
        field_0xa8 = p1;
        field_0x8c = true;
    }

    void ui2Table::FUN_001fd0d8(u32 col, u32 row) {
        m_someRowIdx = row;
        m_someColIdx = col;
        field_0x8c = true;
    }
    
    void ui2Table::FUN_001fd0f0(u32* outCol, u32* outRow) {
        *outRow = m_someRowIdx;
        *outCol = m_someColIdx;
    }
    
    void ui2Table::FUN_001fd108(undefined4 p1, undefined4 p2) {
        field_0xa0.x = p1;
        field_0xa0.y = p2;
        field_0x8c = true;
    }
    
    void ui2Table::FUN_001fd120(undefined4* p1, undefined4* p2) {
        *p1 = field_0xa0.x;
        *p2 = field_0xa0.y;
    }

    bool ui2Table::FUN_001fd138() {
        if (field_0x8c) {
            recalculateOffsets();
            FUN_001fd690();
            return true;
        }

        return false;
    }

    void ui2Table::FUN_001fd178(u32 col, u32 row) {
        if (FUN_001fd138()) {
            FUN_001fd1d0(col, row);
        }
    }
    
    void ui2Table::FUN_001fd1d0(u32 col, u32 row) {
        Cell& cell = m_cells[m_rowCount * col + row];
        field_0xc4->pos = {
            m_pos.x + field_0xa0.x + m_colInfo[col].offset + cell.offset.x,
            m_pos.y + field_0xa0.y + m_rowInfo[row].offset + cell.offset.y
        };
        
        ui::BaseRef w = ui2Base::getGlobalMaster()->findWidget(cell.value.get());
        if (!w) return;

        // It proceeds to get pcVar12 in some bizarre way involving cell.field_0x18
        // Looks a lot like the code in some of the ui2Menu functions
        
        // (*pcVar12)(w + iVar10, this, 0xa00003e8, field_0xc4);
    }

    void ui2Table::recalculateOffsets() {
        i32 maybeOffsetX = 0;
        for (u32 i = 0;i < m_colCount;i++) {
            m_colInfo[i].offset = maybeOffsetX;
            maybeOffsetX += m_colInfo[i].size;
        }

        i32 maybeOffsetY = 0;
        for (u32 i = 0;i < m_rowCount;i++) {
            m_rowInfo[i].offset = maybeOffsetY;
            maybeOffsetY += m_rowInfo[i].size;
        }

        if (field_0xa8) {
            field_0x8c = false;
            return;
        }

        i32 someColOffsetX = m_colInfo[m_someRowIdx].offset;
        i32 someColSizeX = m_colInfo[m_someRowIdx].size;
        if (someColOffsetX + field_0xa0.x < 0) field_0xa0.x = -someColOffsetX;
        else if (someColSizeX + field_0xa0.x > m_bounds.x) field_0xa0.x = m_bounds.x - someColSizeX;

        i32 someRowOffsetY = m_rowInfo[m_someColIdx].offset;
        i32 someRowSizeY = m_rowInfo[m_someColIdx].size;
        if (someRowOffsetY + field_0xa0.y < 0) field_0xa0.y = -someRowOffsetY;
        else if (someRowSizeY + field_0xa0.y > m_bounds.y) field_0xa0.y = m_bounds.y - someRowSizeY;
        
        field_0x8c = false;
    }

    void ui2Table::FUN_001fd690() {
        if (m_colCount < 1) {
            ui::BaseRef w;
            method_0x98(WidgetEventType::UNK30, nullptr, w);
            return;
        }

        for (u32 c = 0;c < m_colCount;c++) {
            for (u32 r = 0;r < m_rowCount;r++) {
                vec2i offset = {
                    m_pos.x + field_0xa0.x + m_colInfo[c].offset,
                    m_pos.y + field_0xa0.y + m_rowInfo[r].offset
                };

                Cell& cell = m_cells[m_rowCount * c + r];
                field_0xcc->pos = {
                    offset.x + cell.offset.x,
                    offset.y + cell.offset.y
                };

                ui::BaseRef w = ui2Base::getGlobalMaster()->findWidget(cell.value.get());
                if (!w) continue;

                // Calculates pcVar10 in some bizarre way involving someCell.field_0x18
                // (*pcVar10)(w + iVar8, this, SOME_WIDGET_ENUM::SetPosition, field_0xcc);

                if (offset.x < m_pos.x - field_0xac.x) {
                    // Calculates pcVar10 in some bizarre way involving someCell.field_0x18
                    // (*pcVar10)(w + iVar8, this, SOME_WIDGET_ENUM::UNK31, nullptr);
                    continue;
                }

                if (offset.y < m_pos.y - field_0xac.y) {
                    // Calculates pcVar10 in some bizarre way involving someCell.field_0x18
                    // (*pcVar10)(w + iVar8, this, SOME_WIDGET_ENUM::UNK31, nullptr);
                    continue;
                }
                
                if (m_pos.x + m_bounds.x + field_0xb4.x < offset.x + m_colInfo[r].size) {
                    // Calculates pcVar10 in some bizarre way involving someCell.field_0x18
                    // (*pcVar10)(w + iVar8, this, SOME_WIDGET_ENUM::UNK31, nullptr);
                    continue;
                }

                if (offset.y + m_rowInfo[c].size <= m_pos.y + m_bounds.y + field_0xb4.y) {
                    // Calculates pcVar10 in some bizarre way involving someCell.field_0x18
                    // (*pcVar10)(w + iVar8, this, SOME_WIDGET_ENUM::UNK32, nullptr);
                    continue;
                }
                
                // Calculates pcVar10 in some bizarre way involving someCell.field_0x18
                // (*pcVar10)(w + iVar8, this, SOME_WIDGET_ENUM::UNK31, nullptr);
            }
        }

        ui::BaseRef w;
        method_0x98(WidgetEventType::UNK30, nullptr, w);
    }

    bool ui2Table::FUN_001fdd88() {
        field_0x8c = true;
        return FUN_001fd138();
    }
};