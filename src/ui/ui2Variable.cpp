#include <libsr2/ui/ui2Variable.h>

#include <string.h>

namespace sr2 {
    ui2Variable::ui2Variable(
        const char* name,
        u32 sz,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true), field_0x7c(sz) {
        field_0x78 = 0;
        field_0x8c = 0;

        init();
    }

    ui2Variable::~ui2Variable() {
    }
    
    void ui2Variable::init() {
        m_intEventData = new IntegerEventData();
        m_strEventData = new ASCIIStringEventData();
    }

    void ui2Variable::method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3) {
        if (!field_0x1c) {
            ui2Widget::method_0x30(p1, p2, p3);
            return;
        }

        if (p2 == SOME_WIDGET_ENUM::UNK26) {
            if (!p3) return;
            FUN_002024c0(p3.cast<ASCIIStringEventData>());
        } else {
            if (p2 < SOME_WIDGET_ENUM::UNK43) {
                if (p2 == SOME_WIDGET_ENUM::UNK37) {
                    if (!p3) return;
                    FUN_00202310(p3.cast<IntegerEventData>());
                    return;
                } else if (p2 == SOME_WIDGET_ENUM::UNK44) {
                    FUN_00202580();
                    return;
                }
            }

            ui2Widget::method_0x30(p1, p2, p3);
        }
    }

    void ui2Variable::FUN_002021f0(undefined4 p1) {
        m_intEventData->data = p1;
        FUN_00202310(m_intEventData);
    }
    
    void ui2Variable::FUN_00202310(const WidgetRef<IntegerEventData>& p1) {
        if (field_0x8c == 1 && field_0x78 == p1->data) return;

        field_0x8c = 1;
        field_0x78 = p1->data;

        ui::BaseRef w;
        method_0x98(SOME_WIDGET_ENUM::UNK39, p1, w);
    }
    
    void ui2Variable::FUN_002023a0(const char* p1) {
        m_strEventData->data = p1;
        FUN_002024c0(m_strEventData);
    }
    
    void ui2Variable::FUN_002024c0(const WidgetRef<ASCIIStringEventData>& p1) {
        const char* v = p1->data;
        if (field_0x8c == 2 && strcmp(field_0x7c.get(), v) == 0) return;

        field_0x7c.set(v);
        field_0x8c = 2;

        ui::BaseRef w;
        method_0x98(SOME_WIDGET_ENUM::UNK40, p1, w);
    }

    void ui2Variable::FUN_00202580() {
        if (field_0x8c == 0) return;
        field_0x8c = 0;

        ui::BaseRef w;
        method_0x98(SOME_WIDGET_ENUM::UNK45, nullptr, w);
    }

    undefined4 ui2Variable::FUN_00202610() {
        return field_0x8c;
    }

    undefined4 ui2Variable::FUN_00202618() {
        return field_0x78;
    }

    const char* ui2Variable::FUN_00202620() {
        return field_0x7c.get();
    }
    
    void ui2Variable::FUN_00202628(ui2Variable* p1) {
        // todo
    }
};