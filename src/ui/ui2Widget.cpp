#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2String.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2EventData.h>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    ui2Widget::ui2Widget(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster) {
        field_0x18 = false;
        m_isActive = true;
        m_addedToMasterUnk0 = false;
        field_0x24 = 0;
        field_0x28 = 0;
        field_0x38 = 0;
        m_widgetName = nullptr;
        field_0x40 = 0;
        field_0x44 = 0;
        m_shouldBeLoaded = false;
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

        // uhhhh... Rockstar? Why have you done this
        if (m_refCount == 0 && this) delete this;
    }
    
    void ui2Widget::reset() {
        addRef();

        m_someBinTree0.reset();
        removeFromMasterUnk0();
        removeFromMasterUnk1();

        field_0x38 = 1;
        m_master = nullptr;
        m_isActive = 0;

        baseReleaseRef();
    }
    
    void ui2Widget::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (event == WidgetEventType::UNK2) {
            setActive(false);
            return;
        }
        if (event > WidgetEventType::UNK2) return;
        if (event != WidgetEventType::UNK0) {
            if (event != WidgetEventType::UNK1) return;
            method_0xa8(1);
            return;
        }

        setActive(1);
    }

    void ui2Widget::method_0x38(const ui::NamedRef& p1, WidgetEventType p2, const WidgetRef<ui2EventData>& p3) {
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

            if (!someCond) onEvent(p1, p2, p3);
        }
    }

    void ui2Widget::method_0x48() {
    }

    void ui2Widget::method_0x58() {
    }

    void ui2Widget::addListener(const ui::NamedRef& listener, WidgetEventType event, SomeWidgetCallback callback) {
        addListener(listener->getName(), event, callback);
    }
    
    void ui2Widget::addListener(const char* listenerName, WidgetEventType event, SomeWidgetCallback callback) {
        if (event == WidgetEventType::UNK36 || event == WidgetEventType::UNK58) {
            // above condition used to be `p2 + 0x7ffffff6 < 2`
            // this is the same thing (I think...) but it makes more sense
            addListener(listenerName, WidgetEventType::Activate, callback);
            addListener(listenerName, WidgetEventType::Deactivate, callback);
        } else if (event != WidgetEventType::UNK35) {
            UnkWidgetBinTree0::UnkData d;
            d.callback = nullptr;
            d.event = event;
            UnkWidgetBinTree0::Node* found = m_someBinTree0.FUN_0020b6e8(d);
            found->unk.str.set(listenerName);
            found->unk.callback = callback;
            return;
        }

        if (event != WidgetEventType::UNK36 && event != WidgetEventType::UNK35) return;
        
        addListener(listenerName, WidgetEventType::UNK0, callback);
        addListener(listenerName, WidgetEventType::UNK2, callback);
    }

    void ui2Widget::removeListener(const ui::NamedRef& listener, WidgetEventType event) {
        removeListener(listener->getName(), event);
    }

    void ui2Widget::removeListener(const char* listenerName, WidgetEventType event) {
        // todo
        // 0x00208b60
    }

    void ui2Widget::removeAllListeners(const ui::NamedRef& listener) {
        removeAllListeners(listener->getName());
    }

    void ui2Widget::removeAllListeners(const char* listenerName) {
        // todo
        // 0x00209278
    }

    void ui2Widget::dispatchEvent(WidgetEventType event, const WidgetRef<ui2EventData>& data, const ui::NamedRef& source) {
        if (!m_isActive || field_0x38) return;

        WidgetRef<ui2Base> master = ui2Base::getGlobalMaster();
        UnkWidgetBinTree0::Node* someNode = nullptr;
        auto baseRoot = m_someBinTree0.getRoot();

        someNode = m_someBinTree0.FUN_0020b748(event);

        // Interesting note:
        // Both the top level while loops in this function are nearly identical
        while (true) {
            bool doBreak = false;

            while (true) {
                if (someNode == baseRoot || someNode->unk.event != event) {
                    baseRoot = m_someBinTree0.FUN_0020b748(WidgetEventType::UNK12);
                    someNode = baseRoot;
                    doBreak = true;
                    break;
                }

                ui::NamedRef found = master->findWidget(someNode->unk.str.get()).cast<ui2Widget>();
                if (found) {
                    ((*found)->*someNode->unk.callback)(!source ? this : source, event, data);
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

        while (true) {
            bool doBreak = false;
            while (true) {
                if (someNode == baseRoot || someNode->unk.event != WidgetEventType::UNK12) {
                    return;
                }

                ui::NamedRef found = master->findWidget(someNode->unk.str.get()).cast<ui2Widget>();
                if (found) {
                    ((*found)->*someNode->unk.callback)(!source ? this : source, event, data);
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

    void ui2Widget::setActive(bool p1) {
        if (m_isActive != p1) {
            m_isActive = p1;
            if (p1 == 1) dispatchEvent(WidgetEventType::Activate, nullptr);
            else dispatchEvent(WidgetEventType::Deactivate, nullptr);
        }

        if (!m_isActive) field_0x40 = 0;
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
        m_shouldBeLoaded = p1;
        if (p1) setName(m_widgetName->get());
    }

    void ui2Widget::method_0xc8(u32 p1) {
    }

    void ui2Widget::configureParser(datParser* parser) {
    }

    void ui2Widget::draw() {
        if (field_0x40 > 0) field_0x40--;
        if (field_0x40 == 0) setActive(true);
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

    bool ui2Widget::isActive() {
        return m_isActive;
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
    
    void ui2Widget::addToMasterUnk0IfNecessary(i32 p1) {
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
    
    void ui2Widget::FUN_0020ac08(const ui::NamedRef& p1, u64 p2) {
        if (p2 & 1) {
            // m_someBinTree0.FUN_0020be20(p1 + 0x10);
        }

        if (p2 & 2) {
            // m_someBinTree1.FUN_0020c0d8(p1 + 0x2c);
        }
    }
    
    bool ui2Widget::shouldBeLoaded() {
        return m_shouldBeLoaded;
    }

    bool ui2Widget::loadWidget() {
        if (!m_shouldBeLoaded) return false;
        return load();
    }
};