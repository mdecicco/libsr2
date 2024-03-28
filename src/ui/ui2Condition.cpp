#include <libsr2/ui/ui2Condition.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    void ui2Condition::Data::FUN_002018f0() {
    }


    ui2Condition::ui2Condition(
        const char* name,
        const char* p2,
        undefined4 p3,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        field_0x78.set(name);
        field_0x88 = p3;

        m_someObject = new Data();
        m_someObject->next = m_someObject;
        m_someObject->last = m_someObject;
    }

    ui2Condition::~ui2Condition() {
        m_someObject->FUN_002018f0();
        delete m_someObject;
    }

    void ui2Condition::method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3) {
        if (!field_0x1c || p2 != SOME_WIDGET_ENUM::UNK42) {
            ui2Widget::method_0x30(p1, p2, p3);
            return;
        }

        FUN_00201378(nullptr);
    }
    
    void ui2Condition::FUN_00200c40(const char* p1) {
        field_0x78.set(p1);
    }
    
    void ui2Condition::FUN_00200c70(const ui::NamedRef& p1) {
        field_0x78.set(p1->getName());
    }

    void ui2Condition::FUN_00200cb8(const ui::BaseRef& p1, const ui::NamedRef& p2, undefined4 p3, ui::BaseRef& p4, u64 p5) {
        FUN_00200d38(p1, p2->getName(), p3, p4, p5);
    }

    void ui2Condition::FUN_00200d38(const ui::BaseRef& p1, const char* p2, undefined4 p3, ui::BaseRef& p4, u64 p5) {
        Data* obj = new Data();

        // actually assigned to *(sp + 0x48), which is never actually assigned
        obj->field_0x10 = 0;

        // actually assigned to *(sp + 0x54), which is never actually assigned
        obj->field_0x1c = 0;

        obj->next = m_someObject->next;
        obj->last = m_someObject->next->last;
        
        Data* next = m_someObject->next;
        next->last->next = obj;
        next->last = obj;

        next->field_0x8 = p1;
        next->field_0x10 = p3;
        next->field_0x1c = p5;
    }
    
    void ui2Condition::FUN_00200fe8(u64 p1, const ui::BaseRef& p2) {

    }
    
    void ui2Condition::FUN_00201378(const ui::BaseRef& p1) {
        if (p1) {
            FUN_00200fe8(field_0x88, p1);
            return;
        }

        FUN_002013b0();
    }

    void ui2Condition::FUN_002013b0() {
        ui::BaseRef w = ui2Base::getGlobalMaster()->findWidget(field_0x78.get());

        if (!w) {
            FUN_00200fe8(field_0x88, nullptr);
            return;
        }

        ui::BaseRef u;
        // FUN_00202628(w, u);

        FUN_00200fe8(field_0x88, u);
    }
};