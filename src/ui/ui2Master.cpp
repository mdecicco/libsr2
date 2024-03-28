#include <libsr2/ui/ui2Master.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    void ui2Master::Unk0::destroy(ui2Master::Unk0** listPtr) {
        Unk0* n = (*listPtr)->next;
        while (n != *listPtr) {
            Unk0* tmp = n;
            n = n->next;
            delete tmp;
        }

        (*listPtr)->next = (*listPtr);
        (*listPtr)->last = (*listPtr);
    }

    void ui2Master::Unk1::destroy(ui2Master::Unk1** listPtr) {
        Unk1* n = (*listPtr)->next;
        while (n != *listPtr) {
            Unk1* tmp = n;
            n = n->next;
            delete tmp;
        }

        (*listPtr)->next = (*listPtr);
        (*listPtr)->last = (*listPtr);
    }

    void ui2Master::Unk1::removeAll(Unk1** listPtr, const ui::NamedRef& widget) {
        Unk1* n = (*listPtr)->next;
        while (n != *listPtr) {
            if (n->field_0x8 != widget) {
                n = n->next;
                continue;
            }

            Unk1* last = n->last;
            Unk1* next = n->next;
            last->next = next;
            next->last = last;
            delete n;
        }
    }


    ui2Master::ui2Master(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster) : ui2Widget(name, master, doAssignMaster) {
        m_unk0 = new Unk0;
        m_unk0->next = m_unk0;
        m_unk0->last = m_unk0;

        m_unk1 = new Unk1;
        m_unk1->next = m_unk1;
        m_unk1->last = m_unk1;

        if (doAssignMaster) addToMasterUnk0(5000, 0x800090000);
        method_0xc0(true);
        field_0x48 = 1;
    }

    ui2Master::~ui2Master() {
        Unk1::destroy(&m_unk1);
        delete m_unk1;
        m_unk1 = nullptr;

        Unk0::destroy(&m_unk0);
        delete m_unk0;
        m_unk0 = nullptr;
    }

    void ui2Master::reset() {
        Unk1* n = m_unk1->next;
        while (n != m_unk1) {
            n->field_0x8->reset();
            n = n->next;
        }

        ui2Widget::reset();
    }

    void ui2Master::draw() {
        if (!field_0x1c) return;

        Unk0* n = m_unk0->next;
        while (n != m_unk0) {
            // local_4c is calculated from n->field_0xc in some bizarre way...

            // local_4c(*n->field_0x14)
            n = n->next;
        }
    }

    void ui2Master::method_0x48() {
        Unk1* n = m_unk1->next;
        while (n != m_unk1) {
            n->field_0x8->method_0x48();
            n = n->next;
        }
    }

    bool ui2Master::loadWidget() {
        ui2Widget::loadWidget();
        
        Unk1* n = m_unk1->next;
        while (n != m_unk1) {
            if (n->field_0x8->field_0x4c == 1) n->field_0x8->loadWidget();
            n = n->next;
        }

        return true;
    }

    void ui2Master::method_0x58() {
        Unk1* n = m_unk1->next;
        while (n != m_unk1) {
            if (n->field_0x8->field_0x4c == 0) n->field_0x8->method_0x58();
            n = n->next;
        }
    }

    void ui2Master::method_0xc8() {
        Unk1* n = m_unk1->next;
        while (n != m_unk1) {
            n->field_0x8->method_0xc8();
            n = n->next;
        }
    }

    void ui2Master::prepParserAgain(datParser* parser) {
        Unk1* n = m_unk1->next;
        while (n != m_unk1) {
            if (n->field_0x8->field_0x48 == 1 && n->field_0x8->field_0x4c == 0) {
                n->field_0x8->prepParserAgain(parser);
            }
            n = n->next;
        }
    }

    const char* ui2Master::getType() const {
        return "ui2Master";
    }

    bool ui2Master::isA(const char* type) const {
        if (strcmp("ui2Master", type) == 0) return true;
        return strcmp(getType(), type) == 0;
    }

    void ui2Master::method_0xe8() {
        WidgetRef<ui2Base> gm = ui2Base::getGlobalMaster();
        gm->pushMaster(this);
    }

    void ui2Master::method_0xf0() {
        WidgetRef<ui2Base> gm = ui2Base::getGlobalMaster();
        gm->popMaster();
    }

    void ui2Master::insertToUnk0(const ui::NamedRef& p1, i32 p2, u64 p3) {
        deleteFromUnk0(p1);

        Unk0* n = m_unk0->next;
        while (n != m_unk0) {
            if (n->field_0x8 < p2) break;
            n = n->next;
        }

        Unk0* newNode = new Unk0;
        newNode->next = n;
        newNode->last = n->last;
        newNode->field_0x8 = p2;
        newNode->field_0xc = p3;
        newNode->field_0x14 = p1;

        n->last->next = newNode;
        n->last = newNode;
    }
    
    void ui2Master::deleteFromUnk0(const ui::NamedRef& p1) {
        Unk0* n = m_unk0->next;
        while (n != m_unk0) {
            if (n->field_0x14 == p1) break;
            n = n->next;
        }

        if (n == m_unk0) return;
        Unk0* next = n->next;
        Unk0* last = n->last;
        last->next = next;
        next->last = last;
        delete n;
    }
    
    undefined4 ui2Master::insertToUnk1(const ui::NamedRef& p1) {
        Unk1* n = new Unk1;
        n->field_0x8 = p1;
        n->next = m_unk1;
        n->last = m_unk1->last;
        m_unk1->last->next = n;
        m_unk1->last = n;

        undefined4 unk = ui2Base::getGlobalMaster()->FUN_00203bb0(p1);
        return unk + 1;
    }
    
    i32 ui2Master::deleteFromUnk1(const ui::NamedRef& p1) {
        Unk1* n;
        
        u32 countBefore = 0;
        n = m_unk1->next;
        while (n != m_unk1) {
            countBefore++;
            n = n->next;
        }

        Unk1::removeAll(&m_unk1, p1);

        u32 countAfter = 0;
        n = m_unk1->next;
        while (n != m_unk1) {
            countAfter++;
            n = n->next;
        }

        u32 unk = ui2Base::getGlobalMaster()->FUN_00203d38(p1);

        u32 deletedCount = countBefore - countAfter;

        return deletedCount + unk;
    }
};