#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2String.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2EventData.h>

#include <utils/Array.hpp>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    ui2Widget::ui2Widget(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster) {
        field_0x18 = false;
        m_isActive = true;
        m_isRenderable = false;
        field_0x24 = 0;
        field_0x28 = 0;
        field_0x38 = 0;
        m_widgetName = nullptr;
        field_0x40 = 0;
        field_0x44 = 0;
        m_shouldBeLoaded = false;
        field_0x5c = nullptr;

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
                stopRendering();
                removeFromMasterUnk1();
            }
        }

        m_refCount--;

        // uhhhh... Rockstar? Why have you done this
        if (m_refCount == 0 && this) delete this;
    }
    
    void ui2Widget::reset() {
        addRef();

        stopRendering();
        removeFromMasterUnk1();

        field_0x38 = 1;
        m_master = nullptr;
        m_isActive = 0;

        baseReleaseRef();
    }
    
    void ui2Widget::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        switch (event) {
            case WidgetEventType::Activate: {
                setActive(true);
                break;
            }
            case WidgetEventType::UNK1: {
                method_0xa8(true);
                break;
            }
            case WidgetEventType::Deactivate: {
                setActive(false);
                break;
            }
            default: break;
        }
    }

    void ui2Widget::acceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (field_0x38) return;

        bool didDispatch = false;

        for (u32 i = 0;i < m_mappers.size();i++) {
            if (m_mappers[i].incoming == event) {
                onEvent(source, m_mappers[i].outgoing, m_mappers[i].event);
                didDispatch = true;
            }
        }

        if (!didDispatch) onEvent(source, event, data);
    }

    void ui2Widget::method_0x48() {
    }

    void ui2Widget::method_0x58() {
    }

    void ui2Widget::addListener(const ui::NamedRef& listener, WidgetEventType event, SomeWidgetCallback callback) {
        m_listeners.push({ event, listener->getName(), callback });
    }
    
    void ui2Widget::addListener(const char* listenerName, WidgetEventType event, SomeWidgetCallback callback) {
        if (event == WidgetEventType::UNK36 || event == WidgetEventType::UNK58) {
            // above condition used to be `p2 + 0x7ffffff6 < 2`
            // this is the same thing (I think...) but it makes more sense
            addListener(listenerName, WidgetEventType::Activated, callback);
            addListener(listenerName, WidgetEventType::Deactivated, callback);
        } else if (event != WidgetEventType::UNK35) {
            m_listeners.push({ event, listenerName, callback });
            return;
        }

        if (event != WidgetEventType::UNK36 && event != WidgetEventType::UNK35) return;
        
        addListener(listenerName, WidgetEventType::Activate, callback);
        addListener(listenerName, WidgetEventType::Deactivate, callback);
    }

    void ui2Widget::removeListener(const ui::NamedRef& listener, WidgetEventType event) {
        removeListener(listener->getName(), event);
    }

    void ui2Widget::removeListener(const char* listenerName, WidgetEventType event) {
        for (i32 i = m_listeners.size() - 1;i >= 0;i--) {
            if (m_listeners[i].type == event && m_listeners[i].widgetName == listenerName) {
                m_listeners.remove(i);
            }
        }
    }

    void ui2Widget::removeAllListeners(const ui::NamedRef& listener) {
        removeAllListeners(listener->getName());
    }

    void ui2Widget::removeAllListeners(const char* listenerName) {
        for (i32 i = m_listeners.size() - 1;i >= 0;i--) {
            if (m_listeners[i].widgetName == listenerName) {
                m_listeners.remove(i);
            }
        }
    }

    void ui2Widget::addEventMapper(WidgetEventType incoming, WidgetEventType outgoing, const WidgetRef<ui2EventData>& event) {
        m_mappers.push({
            incoming,
            outgoing,
            event
        });
    }

    void ui2Widget::removeEventMapper(WidgetEventType incoming, WidgetEventType outgoing) {
        for (i32 i = m_mappers.size() - 1;i >= 0;i--) {
            if (m_mappers[i].incoming == incoming && m_mappers[i].outgoing == outgoing) {
                m_mappers.remove(i);
            }
        }
    }

    void ui2Widget::dispatchEvent(WidgetEventType event, const WidgetRef<ui2EventData>& data, const ui::NamedRef& source) {
        if (!m_isActive || field_0x38) return;

        WidgetRef<ui2Base> master = ui2Base::getGlobalMaster();

        for (u32 i = 0;i < m_listeners.size();i++) {
            if (m_listeners[i].type == event || m_listeners[i].type == WidgetEventType::MaybeAll) {
                ui::NamedRef found = master->findWidget(m_listeners[i].widgetName.c_str()).cast<ui2Widget>();
                if (found) {
                    ((*found)->*m_listeners[i].callback)(!source ? this : source, event, data);
                }
            }
        }
    }

    void ui2Widget::setActive(bool p1) {
        if (m_isActive != p1) {
            m_isActive = p1;
            if (p1 == 1) dispatchEvent(WidgetEventType::Activated, nullptr);
            else dispatchEvent(WidgetEventType::Deactivated, nullptr);
        }

        if (!m_isActive) field_0x40 = 0;
    }

    void ui2Widget::method_0xa8(i32 p1) {
        field_0x40 = p1;
    }

    void ui2Widget::setShouldLoad(bool p1) {
        m_shouldBeLoaded = p1;
        if (p1) setName(m_widgetName->get());
    }

    void ui2Widget::method_0xc8(u32 p1) {
    }

    void ui2Widget::configureParser(datParser* parser) {
    }

    void ui2Widget::draw() {
        if (field_0x40 > 0) {
            field_0x40--;
            if (field_0x40 == 0) setActive(true);
        }
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
    
    const char* ui2Widget::getFileType() {
        return "";
    }

    const char* ui2Widget::getDirectory() {
        return ui2Base::getGlobalMaster()->getDirectory();
    }

    void ui2Widget::prepParser(datParser* parser) {
        return configureParser(parser);
    }

    void ui2Widget::afterLoad() {
        method_0x58();
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

    void ui2Widget::makeRenderable(i32 priority) {
        if (m_isRenderable) field_0x24--;

        getMaster()->addRenderable(this, priority);

        m_isRenderable = true;
        field_0x24++;
    }

    void ui2Widget::stopRendering() {
        if (!m_isRenderable) return;
        m_isRenderable = false;
        field_0x24--;

        getMaster()->removeRenderable(this);
    }
    
    void ui2Widget::setRenderPriority(i32 priority) {
        if (!m_isRenderable) return;
        makeRenderable(priority);
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
        // I assume?
        if (p2 & 1) {
            for (u32 i = 0;i < p1->m_listeners.size();i++) {
                bool found = false;
                for (u32 j = 0;j < m_listeners.size();j++) {
                    if (m_listeners[j].type != p1->m_listeners[i].type) continue;
                    if (m_listeners[j].callback != p1->m_listeners[i].callback) continue;
                    if (m_listeners[j].widgetName != p1->m_listeners[i].widgetName) continue;
                    found = true;
                    break;
                }

                if (!found) m_listeners.push(p1->m_listeners[i]);
            }
        }

        if (p2 & 2) {
            for (u32 i = 0;i < p1->m_mappers.size();i++) {
                bool found = false;
                for (u32 j = 0;j < m_mappers.size();j++) {
                    if (m_mappers[j].event != p1->m_mappers[i].event) continue;
                    if (m_mappers[j].incoming != p1->m_mappers[i].incoming) continue;
                    if (m_mappers[j].outgoing != p1->m_mappers[i].outgoing) continue;
                    found = true;
                    break;
                }

                if (!found) m_mappers.push(p1->m_mappers[i]);
            }
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