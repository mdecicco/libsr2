#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2String.h>
#include <libsr2/ui/ui2Base.h>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    ui2Widget::ui2Widget(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster) {
        field_0x18 = false;
        field_0x1c = true;
        m_addedToMasterUnk0 = false;
        field_0x24 = 0;
        field_0x28 = 0;
        field_0x38 = 0;
        m_widgetName = nullptr;
        field_0x40 = 0;
        field_0x44 = 0;
        field_0x4c = 0;
        field_0x5c = nullptr;

        // memset(&field_0x68, 0, 8);
        field_0x68 = 0;
        m_addedToMasterUnk1 = false;
        field_0x74 = 0;

        m_master = master;

        if (!master && doAssignMaster) getMaster();

        m_refCount++;
        if (!name) m_widgetName = generateName();
        else m_widgetName = new ui2String(name, false);

        field_0x5c = this;

        if (doAssignMaster) addToMasterUnk1();
        m_refCount--;
    }

    ui2Widget::~ui2Widget() {
        if (m_widgetName) delete m_widgetName;
        m_widgetName = nullptr;
    }

    void ui2Widget::releaseRef() {
        if (!field_0x74) {
            if (m_refCount == field_0x24 + 1) {
                field_0x74 = 1;
                removeFromMasterUnk0();
                removeFromMasterUnk1();
            }
        }

        m_refCount--;

        // uhhhh... It looks like the code is doing this:
        // if (m_refCount == 0 && this) delete this;
        //
        // But I don't think I should do that
    }
    
    void ui2Widget::reset() {
        addRef();

        m_someBinTree0.reset();
        removeFromMasterUnk0();
        removeFromMasterUnk1();

        field_0x38 = 1;
        m_master = nullptr;
        field_0x1c = 0;

        baseReleaseRef();
    }
    
    void ui2Widget::onEvent(const ui::BaseRef& p1, WidgetEventType p2, const ui::BaseRef& p3) {
        if (p2 == 0xa0000069) {
            method_0xa0(0);
            return;
        }
        if (p2 > 0xa0000069) return;
        if (p2 != 0xa0000064) {
            if (p2 != 0xa0000065) return;
            method_0xa8(1);
            return;
        }

        method_0xa0(1);
    }

    void ui2Widget::method_0x38(const ui::BaseRef& p1, WidgetEventType p2, const ui::BaseRef& p3) {
        auto root = m_someBinTree1.getRoot();

        if (!field_0x38) {
            UnkWidgetBinTree1::UnkData d;
            d.a = p2;
            auto found = m_someBinTree1.FUN_0020b7b0(d);
            bool someCond = false;

            while (true) {
                if (found == root || found->someSortingValue != p2) break;

                onEvent(p1, found->field_0x14, p3);

                auto n = found->right;
                if (!n) {
                    n = found->parent;
                    while (found == n->right) {
                        found = n;
                        n = n->parent;
                    }

                    if (found->right != n) found = n;
                } else {
                    auto n1 = n->left;
                    while (found = n, n1) {
                        n = n->left;
                        n1 = n->left;
                    }
                }

                someCond = true;
            }

            if (someCond) onEvent(p1, p2, p3);
        }
    }

    void ui2Widget::method_0x48() {
    }

    void ui2Widget::method_0x58() {
    }

    void ui2Widget::method_0x68(const ui::NamedRef& ref, WidgetEventType p2, u64 p3) {
        method_0x70(ref->getName(), p2, p3);
    }

    void ui2Widget::method_0x70(const char* p1, WidgetEventType p2, u64 p3) {
        if (p2 + 0x7ffffff6 < 2) {
            method_0x70(p1, WidgetEventType::UNK9, p3);
            method_0x70(p1, WidgetEventType::UNK10, p3);
        } else if (p2 != WidgetEventType::UNK35) {
            // todo
            // nonsense 
            // Looks like it's removing from m_someBinTree0
            return;
        }

        if (p2 != WidgetEventType::UNK36 && p2 != WidgetEventType::UNK35) return;
        
        method_0x70(p1, WidgetEventType::UNK0, p3);
        method_0x70(p1, WidgetEventType::UNK2, p3);
    }

    void ui2Widget::method_0x78(const ui::NamedRef& ref, i32 p2, u64 p3) {
        method_0x80(ref->getName(), p2, p3);
    }

    void ui2Widget::method_0x80(const char* p1, i32 p2, i32 p3) {
        // ... nonsense
        // Looks like it's inserting to m_someBinTree0
        // todo
        // 0x00208b60
    }

    void ui2Widget::method_0x88(const ui::NamedRef& ref) {
        method_0x90(ref->getName());
    }

    void ui2Widget::method_0x90(const char* p1) {
        // ... nonsense
        // Looks like it's inserting to m_someBinTree0
        // todo
        // 0x00209278
    }
    
    void ui2Widget::method_0x98(WidgetEventType p1, const ui::BaseRef& p2, ui::BaseRef& p3) {
        WidgetRef<ui2Base> master = ui2Base::getGlobalMaster();
        UnkWidgetBinTree0::Node* someNode = nullptr;
        auto baseRoot = m_someBinTree0.getRoot();

        // Interesting note:
        // Both the top level while loops in this function are nearly identical

        if (field_0x1c && !field_0x38) {
            if (!p3) p3 = this;

            someNode = m_someBinTree0.FUN_0020b748(p1);
            while (true) {
                bool doBreak = false;

                while (true) {
                    if (someNode == baseRoot || someNode->someSortingValue != p1) {
                        someNode = m_someBinTree0.FUN_0020b748(WidgetEventType::UNK12);
                        doBreak = true;
                        break;
                    }

                    ui::BaseRef found = master->findWidget(someNode->text.get());
                    if (found) {
                        // wtf?
                        // Not only is the decompilation incorrect (or I'm dumb),
                        // but the apparent behavior is just bizarre...
                        // See the if statement under 0x00209a24
                    }
                    
                    auto n = someNode->right;
                    if (!n) break;

                    while (n->left) n = n->left;
                    someNode = n;
                }

                if (doBreak) break;

                auto n = someNode->parent;
                while (someNode == n->right) {
                    someNode = n;
                    n = n->parent;
                }
                if (someNode->right != n) someNode = n;
            }
        }

        while (true) {
            bool doBreak = false;
            while (true) {
                if (someNode == baseRoot || someNode->someSortingValue != WidgetEventType::UNK12) {
                    doBreak = true;
                    break;
                }

                auto found = master->findWidget(someNode->text.get());
                if (found) {
                    // wtf?
                    // Not only is the decompilation incorrect (or I'm dumb),
                    // but the apparent behavior is just bizarre...
                    // See the if statement under 0x00209b84
                }

                auto n = someNode->right;
                if (!n) break;

                while (n->left) n = n->left;
                someNode = n;
            }

            if (doBreak) break;

            auto n = someNode->parent;
            while (someNode == n->right) {
                someNode = n;
                n = n->parent;
            }
            if (someNode->right != n) someNode = n;
        }
    }

    void ui2Widget::method_0xa0(bool p1) {
        if (field_0x1c != p1) {
            field_0x1c = p1;
            ui::BaseRef w;
            if (p1 == 1) method_0x98(WidgetEventType::UNK9, nullptr, w);
            else method_0x98(WidgetEventType::UNK10, nullptr, w);
        }

        if (!field_0x1c) field_0x40 = 0;
    }

    void ui2Widget::method_0xa8(i32 p1) {
        field_0x40 = p1;
    }

    void ui2Widget::method_0xb0(WidgetEventType p1, WidgetEventType p2, const ui::BaseRef& p3) {
        ui2WidgetBase* w = *p3;
        UnkWidgetBinTree1::UnkData d;
        d.a = p1;
        d.b = p2;
        d.widget = w;

        if (w) {
            w->addRef();
            w->addRef();
            w->addRef();
        }

        m_someBinTree1.FUN_0020bc00(d);
        if (d.widget) w->baseReleaseRef();

        if (w) {
            w->baseReleaseRef();
            w->baseReleaseRef();
        }
    }

    void ui2Widget::method_0xb8(WidgetEventType p1, WidgetEventType p2) {
        UnkWidgetBinTree1::UnkData search;
        search.a = p1;

        // Code that was actually in this function moved into `maybeRemove` function below
        // Definitely wasn't inlined... I think... But it modifies protected properties and
        // doesn't touch `this` so it probably belongs there
        m_someBinTree1.maybeRemove(p2, search);
    }

    void ui2Widget::method_0xc0(bool p1) {
        field_0x4c = p1;
        if (p1) setName(m_widgetName->get());
    }

    void ui2Widget::method_0xc8() {
    }

    void ui2Widget::prepParserAgain(datParser* parser) {
    }

    void ui2Widget::draw() {
        if (field_0x40 > 0) field_0x40--;
        if (field_0x40 == 0) method_0xa0(1);
    }

    const char* ui2Widget::getName() const {
        return m_widgetName->get();
    }
    
    const char* ui2Widget::getType() const {
        return "ui2Widget";
    }

    bool ui2Widget::isA(const char* type) const {
        return strcmp(getType(), type) == 0;
    }
    
    const char* ui2Widget::getFileType() const {
        return "";
    }

    const char* ui2Widget::getDirectory() const {
        return ui2Base::getGlobalMaster()->getDirectory();
    }

    datParserNode* ui2Widget::prepParser(datParser* parser) {
        // return field_0xc->prepParser(parser);
        return nullptr;
    }

    void ui2Widget::afterLoad() {
        // field_0xc->afterLoad();
    }

    ui2String* ui2Widget::generateName() {
        u32 id = ui2Base::getNextWidgetId();

        char idStr[12] = { 0 };
        snprintf(idStr, 12, "%x", id);

        const char* masterName = nullptr;
        if (m_master) masterName = m_master->getName();

        u32 nameLen = strlen(idStr) + strlen(masterName) + 3;
        char* finalName = new char[nameLen];
        snprintf(finalName, nameLen, "~%s.%s", masterName, idStr);

        return new ui2String(finalName, true);
    }

    WidgetRef<ui2Master> ui2Widget::getMaster() {
        if (m_master) return m_master;

        m_master = ui2Base::getGlobalMaster()->currentMaster();

        return m_master;
    }

    bool ui2Widget::FUN_0020a070() {
        return field_0x1c;
    }

    void ui2Widget::addToMasterUnk0(i32 p1, u64 p2) {
        if (m_addedToMasterUnk0) field_0x24--;

        getMaster()->insertToUnk0(this, p1, p2);

        m_addedToMasterUnk0 = true;
        field_0x68 = p2;
        field_0x24++;
    }

    void ui2Widget::removeFromMasterUnk0() {
        if (!m_addedToMasterUnk0) return;
        m_addedToMasterUnk0 = false;
        field_0x24--;

        getMaster()->deleteFromUnk0(this);

        field_0x68 = 0;
        // memset(&field_0x68, 0, 8);
    }
    
    void ui2Widget::FUN_0020aa80(i32 p1, u64 p2) {
        if (!m_addedToMasterUnk0) return;
        addToMasterUnk0(p1, field_0x68);
    }

    void ui2Widget::addToMasterUnk1() {
        field_0x28 = getMaster()->insertToUnk1(this);
        m_addedToMasterUnk1 = true;
        field_0x24 += field_0x28;
    }
    
    void ui2Widget::removeFromMasterUnk1() {
        if (!m_addedToMasterUnk1) return;

        m_addedToMasterUnk1 = false;
        field_0x24 -= field_0x28;
        getMaster()->deleteFromUnk1(this);

        field_0x28 = 0;
    }
    
    void ui2Widget::FUN_0020ac08(i32* p1, u64 p2) {
        if (p2 & 1) {
            // m_someBinTree0.FUN_0020be20(p1 + 0x10);
        }

        if (p2 & 2) {
            // m_someBinTree1.FUN_0020c0d8(p1 + 0x2c);
        }
    }

    bool ui2Widget::loadWidget() {
        if (!field_0x4c) return false;
        return load();
    }
};