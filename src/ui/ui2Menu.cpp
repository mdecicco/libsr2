#include <libsr2/ui/ui2Menu.h>
#include <libsr2/ui/ui2Base.h>

#include <string.h>
#include <assert.h>

namespace sr2 {
    ui2Menu::ui2Menu(
        const char* name,
        u32 rowCount,
        u32 colCount,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        m_rowCount = rowCount;
        m_colCount = colCount;
        m_someRowIdx = 0;
        m_someColIdx = 0;
        m_grid = nullptr;
        field_0x8c = 1;

        init();
    }

    ui2Menu::~ui2Menu() {
        if (m_grid) delete [] m_grid;
    }

    void ui2Menu::init() {
        u32 cellCount = m_rowCount * m_colCount;
        m_grid = new GridCell[cellCount];
        for (u32 i = 0;i < cellCount;i++) {
            m_grid[i].field_0x10 = 0;
            m_grid[i].field_0x14 = 0;
            m_grid[i].field_0x20 = 0;
            m_grid[i].field_0x30 = 0;
        }

        field_0x90 = new uiUnknown25();
        field_0x98 = new uiUnknown25();

        addToMasterUnk0(5000, 0x800090000);
    }

    void ui2Menu::reset() {
        if (m_grid) {
            delete [] m_grid;
            m_grid = nullptr;
        }

        field_0x90 = nullptr;
        field_0x98 = nullptr;

        ui2Widget::reset();
    }

    void ui2Menu::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) {
            ui2Widget::onEvent(source, event, data);
            return;
        }

        GridCell& someCell = m_grid[m_rowCount * m_someColIdx + m_someRowIdx];

        switch (event) {
            case WidgetEventType::AcceptPressed: {
                field_0x98->col = m_someColIdx;
                field_0x98->row = m_someRowIdx;

                if (someCell.field_0x14) return;

                ui::NamedRef found = ui2Base::getGlobalMaster()->findWidget(someCell.str.get()).cast<ui2Widget>();
                if (found) {
                    ((*found)->*someCell.field_0x18)(this, WidgetEventType::UNK22, field_0x98);
                }

                dispatchEvent(WidgetEventType::UNK22, field_0x98);
                break;
            }
            case WidgetEventType::BackPressed: {
                field_0x98->col = m_someColIdx;
                field_0x98->row = m_someRowIdx;

                if (someCell.field_0x14) return;

                ui::NamedRef found = ui2Base::getGlobalMaster()->findWidget(someCell.str.get()).cast<ui2Widget>();
                if (found) {
                    ((*found)->*someCell.field_0x18)(this, WidgetEventType::UNK23, field_0x98);
                }

                dispatchEvent(WidgetEventType::UNK23, field_0x98);
                break;
            }
            case WidgetEventType::UpPressed: {
                FUN_001f9990(m_someColIdx, FUN_001fad48());
                return;
            }
            case WidgetEventType::DownPressed: {
                FUN_001f9990(m_someColIdx, FUN_001fae08());
                return;
            }
            case WidgetEventType::LeftPressed: {
                FUN_001f9990(FUN_001faed0(), m_someRowIdx);
                return;
            }
            case WidgetEventType::RightPressed: {
                FUN_001f9990(FUN_001faf90(), m_someRowIdx);
                return;
            }
            default: {
                ui2Widget::onEvent(source, event, data);
                return;
            }
        }
    }

    void ui2Menu::FUN_001f8218(const ui::NamedRef& p1, i32 row, i32 col, undefined4 p4, SomeWidgetCallback p5) {
        GridCell& someCell = m_grid[m_rowCount * col + row];
        
        someCell.str.set(p1->getName());
        someCell.field_0x10 = p4;
        someCell.field_0x18 = p5;
        someCell.field_0x20 = 0;

        FUN_001f9990(m_someRowIdx, m_someColIdx);
    }

    void ui2Menu::FUN_001f8388(const char* p1, i32 row, i32 col, undefined4 p4, SomeWidgetCallback p5) {
        ui::BaseRef w = ui2Base::getGlobalMaster()->findWidget(p1);
        if (!w) return;
        
        FUN_001f8218(w.cast<ui2Widget>(), row, col, p4, p5);
    }

    void ui2Menu::FUN_001f84d8(i32 row, i32 col) {
        GridCell& someCell = m_grid[m_rowCount * col + row];
        someCell.str.set("");
        someCell.field_0x10 = 0;
        someCell.field_0x18 = 0;
        someCell.field_0x20 = 0;

        FUN_001f9990(m_someRowIdx, m_someColIdx);
    }

    void ui2Menu::FUN_001f85c0(i32 row, i32 col, WidgetEventType p3, const ui::BaseRef& p4) {
        GridCell& someCell = m_grid[m_rowCount * col + row];
        someCell.field_0x20 = 1;
        someCell.field_0x24 = p3;
        someCell.field_0x28 = p4;
        someCell.field_0x30 = 0;
    }
    
    void ui2Menu::FUN_001f86c8(i32 row, i32 col, WidgetEventType p3, const ui::BaseRef& p4) {
        GridCell& someCell = m_grid[m_rowCount * col + row];
        someCell.field_0x20 = 1;
        someCell.field_0x24 = p3;
        someCell.field_0x28 = p4;
        someCell.field_0x30 = 1;
    }
    
    void ui2Menu::FUN_001f87d8(i32 row, i32 col, ui2Widget* p3) {
        // In its current state this function will loop infinitely if p3 is not null...
        // But this function is apparently unused so I'll just ignore it
        while (p3) {
            ui::NamedRef w = p3;
            FUN_001f8218(w, row, col, 1, &ui2Widget::method_0x38);
            
            row++;
            // p3 = ppuVar2->u ??? (ppuVar2 is a pointer to a variable that is _never_ assigned)

            if (m_rowCount <= row) {
                row = 0;
                col++;
            }
        }
    }
    
    void ui2Menu::FUN_001f88c8(i32 row, i32 col, ui2Widget* p3) {
        // In its current state this function will loop infinitely if p3 is not null...
        // But this function is apparently unused so I'll just ignore it
        while (p3) {
            ui::NamedRef w = p3;
            FUN_001f8218(w, row, col, 1, &ui2Widget::method_0x38);
            
            col++;
            // p3 = ppuVar2->u ??? (ppuVar2 is a pointer to a variable that is _never_ assigned)

            if (m_colCount <= col) {
                col = 0;
                row++;
            }
        }
    }
    
    void ui2Menu::FUN_001f89b8(i32 row, i32 col, const char* p3) {
        // In its current state this function will loop infinitely if p3 is not null...
        // But this function is apparently unused so I'll just ignore it
        while (p3) {
            FUN_001f8388(p3, row, col, 1, &ui2Widget::method_0x38);
            
            row++;
            // p3 = *ppcVar1 ??? (ppcVar1 is a pointer to a variable that is _never_ assigned)

            if (m_rowCount <= row) {
                row = 0;
                col++;
            }
        }
    }

    void ui2Menu::FUN_001f88c8(i32 row, i32 col, const char* p3) {
        // In its current state this function will loop infinitely if p3 is not null...
        // But this function is apparently unused so I'll just ignore it
        while (p3) {
            FUN_001f8388(p3, row, col, 1, &ui2Widget::method_0x38);
            
            col++;
            // p3 = *ppcVar1 ??? (ppcVar1 is a pointer to a variable that is _never_ assigned)

            if (m_colCount <= col) {
                col = 0;
                row++;
            }
        }
    }

    void ui2Menu::FUN_001f8b38(i32 row, i32 col, undefined4 p3) {
        GridCell& someCell = m_grid[m_rowCount * col + row];
        if (someCell.field_0x10 == p3) return;

        someCell.field_0x10 = p3;

        ui::NamedRef w = ui2Base::getGlobalMaster()->findWidget(someCell.str.get()).cast<ui2Widget>();
        if (!w) {
            FUN_001f9990(m_someRowIdx, m_someColIdx);
            return;
        }

        WidgetEventType someVal;
        if (p3) someVal = WidgetEventType::UNK24;
        else someVal = WidgetEventType::UNK25;

        ((*w)->*someCell.field_0x18)(this, WidgetEventType::UNK24, nullptr);
        
        FUN_001f9990(m_someRowIdx, m_someColIdx);
    }

    undefined4 ui2Menu::FUN_001f8ee0(i32 row, i32 col) {
        return m_grid[m_rowCount * col + row].field_0x10;
    }
    
    void ui2Menu::FUN_001f8f08(i32 row, i32 col, i32 p3) {
        // todo
    }

    undefined4 ui2Menu::FUN_001f9968(i32 row, i32 col) {
        return m_grid[m_rowCount * col + row].field_0x14;
    }

    void ui2Menu::FUN_001f9990(i32 row, i32 col) {
        field_0x90->col = m_someColIdx;
        field_0x90->row = m_someRowIdx;

        FUN_001f9a28(field_0x90);
    }

    void ui2Menu::FUN_001f9a28(const ui::BaseRef& p1) {
        // todo
    }

    void ui2Menu::FUN_001fa298(const ui::BaseRef& p1) {
        // todo
    }
    
    bool ui2Menu::FUN_001fa3d8(i32* outRow, i32* outCol, ui::BaseRef& outWidget) {
        *outRow = m_someRowIdx;
        *outCol = m_someColIdx;

        GridCell& someCell = m_grid[m_rowCount * m_someColIdx + m_someRowIdx];

        outWidget = ui2Base::getGlobalMaster()->findWidget(someCell.str.get());

        if (outWidget && someCell.field_0x10 != 1) return true;

        return false;
    }
    
    void ui2Menu::FUN_001fa510(undefined4 p1) {
        field_0x8c = p1;
    }
    
    void ui2Menu::FUN_001fa518() {
        // todo
    }

    i32 ui2Menu::FUN_001fad48() {
        i32 c = m_someColIdx;

        while (true) {
            c -= 1;

            if (c < 0) {
                if (field_0x8c == 1) c = m_colCount - 1;
                else break;
            }

            if (c == m_someColIdx) break;

            GridCell& cell = m_grid[m_rowCount * c + m_someRowIdx];
            if (cell.field_0x10 != 0) {
                const char* str0 = cell.str.get();
                if (*str0 == 0) continue;

                const char* str1 = m_grid[m_rowCount * m_someColIdx + m_someRowIdx].str.get();

                if (strcmp(str0, str1) != 0) break;
            }
        }

        return c;
    }

    i32 ui2Menu::FUN_001fae08() {
        i32 c = m_someColIdx;

        while (true) {
            c += 1;
            if (m_colCount <= c) {
                c = 0;
                if (field_0x8c != 1) break;
            }

            if (c == m_someColIdx) break;

            GridCell& cell = m_grid[m_rowCount * c + m_someRowIdx];
            if (cell.field_0x10 != 0) {
                const char* str0 = cell.str.get();
                if (*str0 == 0) continue;

                const char* str1 = m_grid[m_rowCount * m_someColIdx + m_someRowIdx].str.get();
                if (strcmp(str0, str1) != 0) break;
            }
        }

        return c;
    }

    i32 ui2Menu::FUN_001faed0() {
        i32 r = m_someRowIdx;

        while (true) {
            r -= 1;

            if (r < 0) {
                if (field_0x8c == 1) r = m_rowCount - 1;
                else break;
            }

            if (r == m_someRowIdx) break;

            GridCell& cell = m_grid[m_rowCount * m_someColIdx + r];
            if (cell.field_0x10 != 0) {
                const char* str0 = cell.str.get();
                if (*str0 == 0) continue;

                const char* str1 = m_grid[m_rowCount * m_someColIdx + m_someRowIdx].str.get();

                if (strcmp(str0, str1) != 0) break;
            }
        }

        return r;
    }

    i32 ui2Menu::FUN_001faf90() {
        i32 r = m_someRowIdx;

        while (true) {
            r += 1;
            if (m_rowCount <= r) {
                r = 0;
                if (field_0x8c != 1) break;
            }

            if (r == m_someRowIdx) break;

            GridCell& cell = m_grid[m_rowCount * m_someColIdx + r];
            if (cell.field_0x10 != 0) {
                const char* str0 = cell.str.get();
                if (*str0 == 0) continue;

                const char* str1 = m_grid[m_rowCount * m_someColIdx + m_someRowIdx].str.get();
                if (strcmp(str0, str1) != 0) break;
            }
        }

        return r;
    }

    bool ui2Menu::FUN_001fb050(i32* outRow, i32* outCol) { 
        i32 someIdx = m_rowCount * *outCol + *outRow;
        GridCell& someCell = m_grid[someIdx];

        if (someCell.field_0x10 != 0 && *someCell.str.get() != '\0') return true;

        i32 someOtherIdx = someIdx - 1;
        if (someOtherIdx >= 0) {
            someCell = m_grid[someOtherIdx];

            if (someCell.field_0x10 != 0 && *someCell.str.get() != '\0') {
                if (someOtherIdx >= 0) {
                    assert(m_rowCount != 0);

                    *outRow = someOtherIdx % m_rowCount;
                    *outCol = someOtherIdx / m_rowCount;
                    return true;
                }
            } else {
                i32 cellIdx = someIdx - 2;
                while (cellIdx >= 0) {
                    someCell = m_grid[cellIdx];

                    if (someCell.field_0x10 != 0 && *someCell.str.get() != '\0') {
                        if (cellIdx >= 0) {
                            assert(m_rowCount != 0);

                            *outRow = cellIdx % m_rowCount;
                            *outCol = cellIdx / m_rowCount;

                            return true;
                        }
                        
                        break;
                    }
                    
                    cellIdx += -1;
                }
            }
        }

        someIdx = m_rowCount * *outCol + *outRow + 1;
        if (someIdx < m_rowCount * m_colCount) {
            do {
                someCell = m_grid[someIdx];
                if (someCell.field_0x10 != 0 && *someCell.str.get() != '\0') break;
                someIdx += 1;
            } while (someIdx < m_rowCount * m_colCount);
        }

        if (m_rowCount * m_colCount <= someIdx) return false;
        assert(m_rowCount != 0);

        *outRow = someIdx % m_rowCount;
        *outCol = someIdx / m_rowCount;
        
        return true;
    }
};