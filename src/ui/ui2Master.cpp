#include <libsr2/ui/ui2Master.h>
#include <libsr2/ui/ui2Base.h>

#include <utils/Array.hpp>

namespace sr2 {
    ui2Master::ui2Master(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster) : ui2Widget(name, master, doAssignMaster) {
        if (doAssignMaster) makeRenderable(5000);
        setShouldLoad(true);
        field_0x48 = 1;
    }

    ui2Master::~ui2Master() {
    }

    void ui2Master::reset() {
        for (auto n : m_testUnk1) n->reset();

        ui2Widget::reset();
    }

    void ui2Master::draw() {
        if (!m_isActive) return;

        for (auto n : m_renderables) n.element->draw();
    }

    void ui2Master::method_0x48() {
        for (u32 i = 0;i < m_testUnk1.size();i++) {
            m_testUnk1[i]->method_0x48();
        }
    }

    bool ui2Master::loadWidget() {
        ui2Widget::loadWidget();
        
        for (auto n : m_testUnk1) {
            if (n->shouldBeLoaded()) n->loadWidget();
        }

        return true;
    }

    void ui2Master::method_0x58() {
        for (auto n : m_testUnk1) {
            if (!n->shouldBeLoaded()) n->method_0x58();
        }
    }

    void ui2Master::method_0xc8(u32 p1) {
        for (auto n : m_testUnk1) n->method_0xc8(p1);
    }

    void ui2Master::configureParser(datParser* parser) {
        for (auto n : m_testUnk1) {
            if (n->field_0x48 == 1 && !n->shouldBeLoaded()) {
                n->configureParser(parser);
            }
        }
    }

    const char* ui2Master::getType() const {
        return "ui2Master";
    }

    bool ui2Master::isA(const char* type) const {
        if (strcmp("ui2Master", type) == 0) return true;
        return ui2Widget::isA(type);
    }

    void ui2Master::method_0xe8() {
        WidgetRef<ui2Base> gm = ui2Base::getGlobalMaster();
        gm->pushMaster(this);
    }

    void ui2Master::method_0xf0() {
        WidgetRef<ui2Base> gm = ui2Base::getGlobalMaster();
        gm->popMaster();
    }

    void ui2Master::addRenderable(const ui::NamedRef& element, i32 priority) {
        removeRenderable(element);

        for (u32 i = 0;i < m_renderables.size();i++) {
            if (priority < m_renderables[i].priority) {
                m_renderables.insert(i, { priority, element });
                return;
            }
        }
        
        m_renderables.push({ priority, element });
    }
    
    void ui2Master::removeRenderable(const ui::NamedRef& element) {
        const char* name = element->getName();
        for (i32 i = m_renderables.size() - 1;i >= 0;i--) {
            if (strcmp(m_renderables[i].element->getName(), name) == 0) {
                m_renderables.remove(i);
            }
        }
    }
    
    undefined4 ui2Master::insertToUnk1(const ui::NamedRef& element) {
        m_testUnk1.push(element);
        ui2Base::getGlobalMaster()->FUN_00203bb0(element);
        return 1;
    }
    
    i32 ui2Master::deleteFromUnk1(const ui::NamedRef& element) {
        const char* name = element->getName();
        for (i32 i = m_testUnk1.size() - 1;i >= 0;i--) {
            if (strcmp(m_testUnk1[i]->getName(), name) == 0) {
                m_testUnk1.remove(i);
            }
        }

        ui2Base::getGlobalMaster()->FUN_00203d38(element);

        return 0;
    }
};