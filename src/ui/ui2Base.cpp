#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2Input.h>
#include <libsr2/globals.h>

#include <stdio.h>
#include <assert.h>

namespace sr2 {
    u32 ui2Base::nextWidgetId = 0;
    ui2Base* ui2Base::globalMaster = nullptr;

    ui2Base::ui2Base(const char* name) : ui2Widget(name ? name : "~Base", nullptr, false) {
        globalMaster = this;
        nextWidgetId = 1;

        m_currentLang = LANGUAGE::English;
        m_stringFlags = 0;
        field_0x168 = 0;
        field_0x16c = 0;
        field_0x170 = 0;
        field_0x174 = 0;
        m_someStrings[0] = nullptr;
        m_someStrings[1] = nullptr;
        m_someStrings[2] = nullptr;

        m_widgets.init();

        const char* prefix = "~";
        if (m_widgetName->get()[0] == '~') prefix = "";
        char masterName[100];
        snprintf(masterName, 100, "%s%s-GlobalMaster", prefix, m_widgetName->get());
        m_master = new ui2Master(masterName, nullptr, false);

        field_0x98[0] = 0;
        field_0xc0 = new ui2Unknown0();
        field_0xc0->language = LANGUAGE::English;
        field_0xc0->stringFlags = 0xffffffff;
        strcpy(field_0xc0->field_0x10, "");

        if (!m_gameplayText.isInitialized()) {
            m_gameplayText.init();
            field_0x170 = 1;
        }

        setDirectory("tune/ui");

        m_masters.push_back(m_master);

        ui2Input::FUN_001fe638();
    }

    ui2Base::~ui2Base() {
        FUN_001f2ae0();
        m_widgets.deinit();
    }


    void ui2Base::releaseRef() {
        if (!field_0x174 && m_refCount == 1) {
            field_0x174 = 1;
            m_masters.pop_back();

            if (m_master) {
                m_master->reset();
                m_master = nullptr;
            }

            if (field_0x170) m_gameplayText.kill();
            globalMaster = nullptr;
        }

        ui2Widget::releaseRef();
    }

    void ui2Base::method_0x48() {
        field_0x168 = 1;
        m_master->method_0x48();
    }

    bool ui2Base::loadWidget() {
        field_0x16c = 1;
        return m_master->loadWidget();
    }
    
    void ui2Base::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive || event != WidgetEventType::UNK46) {
            ui2Widget::onEvent(source, event, data);
            return;
        }

        if (!data) return;

        loadStringsInner(data.cast<ui2Unknown0>());
    }

    void ui2Base::draw() {
        if (!field_0x168) method_0x48();

        m_master->draw();
    }

    void ui2Base::loadStrings(const char* p1, LANGUAGE lang, u32 stringFlags) {
        strcpy(field_0xc0->field_0x10, p1);
        field_0xc0->stringFlags = stringFlags;
        field_0xc0->language = lang;

        loadStringsInner(field_0xc0);
    }

    void ui2Base::loadStringsInner(const WidgetRef<ui2Unknown0>& lang) {
        if (m_currentLang == lang->language && m_stringFlags == lang->stringFlags && strncmp(field_0x98, lang->field_0x10, 32) == 0) return;

        m_currentLang = lang->language;
        m_stringFlags = lang->stringFlags;
        strncpy(field_0x98, lang->field_0x10, 32);

        m_gameplayText.kill();
        m_gameplayText.load(field_0x98, m_currentLang, m_stringFlags, false);

        m_master->method_0xc8(1);

        m_master->dispatchEvent(WidgetEventType::UNK47, lang);

        if (field_0x16c == 1 && field_0x168) loadWidget();
    }

    void ui2Base::pushMaster(const WidgetRef<ui2Master>& p1) {
        m_masters.push_back(p1);
    }

    void ui2Base::popMaster() {
        m_masters.pop_back();
    }
    
    WidgetRef<ui2Master> ui2Base::currentMaster() {
        if (m_masters.size() == 0) return nullptr;
        return m_masters[m_masters.size() - 1];
    }

    undefined4 ui2Base::FUN_00203bb0(const ui::NamedRef& p1) {
        // probably insert
        assert(m_widgetsTestMap.count(p1->getName()) == 0);
        m_widgetsTestMap.insert(std::pair(p1->getName(), *p1));
        p1->addRef();

        return 1;

        // WidgetBinTree found = m_widgets.findByName(p1->getName());
        // assert(found.getRoot() == m_widgets.getRoot());

        // m_widgets.FUN_00204b68(p1);
        // return 1;
    }

    u32 ui2Base::FUN_00203d38(const ui::NamedRef& p1) {
        // probably remove
        auto it = m_widgetsTestMap.find(p1->getName());

        if (it == m_widgetsTestMap.end()) return 0;
        m_widgetsTestMap.erase(it);
        return 1;

        // return m_widgets.FUN_00205458(p1->getName());
    }

    ui::BaseRef ui2Base::findWidget(const char* name, const char* type) {
        auto it = m_widgetsTestMap.find(name);

        if (it == m_widgetsTestMap.end()) return ui::BaseRef();
        ui2Widget* w = it->second;

        if (type && !w->isA(type)) return ui::BaseRef();

        return w;

        // WidgetBinTree::Node* n = m_widgets.findWidgetByName(name);

        // if (n == m_widgets.getRoot()) {
        //     return ui::BaseRef();
        // }

        // if (type && !n->widget.cast<ui2Widget>()->isA(type)) {
        //     return ui::BaseRef();
        // }

        // return n->widget;
    }



    void ui2Base::getLangInfo(LANGUAGE* outLang, u32* outFlags) {
        *outLang = m_currentLang;
        *outFlags = m_stringFlags;
    }

    void ui2Base::setDirectory(const char* dir) {
        size_t dirLen = strlen(dir);

        const char* sep = "/";
        if (dirLen == 0 || dir[dirLen - 1] == '\\' || dir[dirLen - 1] == '/') sep = "";

        static const char* langAbbrev[] = { "en", "es", "fr", "de", "it", "pt", "jp", "ch", "ko", "no" };
        char buf[100];

        for (u32 i = 0;i < 10;i++) {
            snprintf(buf, 100, "%s%s%s", dir, sep, langAbbrev[i]);
            m_languageDirs[i].set(buf);
        }        
    }

    const char* ui2Base::getDirectory() {
        return m_languageDirs[m_currentLang].get();
    }

    ui::BaseRef* ui2Base::FUN_00203b88() {
        // todo
        // return &field2_0x7c[-1];
        return nullptr;
    }
    



    u32 ui2Base::getNextWidgetId() {
        return nextWidgetId++;
    }

    WidgetRef<ui2Base> ui2Base::getGlobalMaster() {
        return globalMaster;
    }
    
    WidgetRef<ui2Master> ui2Base::getMaster() {
        return globalMaster->m_master;
    }

    txtStringTable& ui2Base::getGameplayText() {
        return globalMaster->m_gameplayText;
    }
    
    ui2String** ui2Base::getSomeStrings() {
        return globalMaster->m_someStrings;
    }

    void ui2Base::FUN_001f2ae0() {
        if (DAT_00362d14) {
            FUN_002f7e88();
            DAT_00362d14 = 0;
        }
    }

    void ui2Base::FUN_002f7e88() {
        // todo
    }
};