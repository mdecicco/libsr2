#include <libsr2/ui/ui2Base.h>

#include <stdio.h>

namespace sr2 {
    undefined4 ui2Base::DAT_00362d7c = 0;
    f32 ui2Base::FLOAT_00362d84 = 0;
    f32 ui2Base::FLOAT_00362d88 = 0;
    f32 ui2Base::FLOAT_00362d90 = 0;
    undefined4 ui2Base::DAT_008d53e0 = 0;
    undefined4 ui2Base::DAT_008d53fc = 0;
    undefined4 ui2Base::DAT_008d541c = 0;
    f32 ui2Base::FLOAT_00362d8c = 0;
    undefined4 ui2Base::DAT_008d5380 = 0;
    undefined4 ui2Base::DAT_00362d80 = 0;
    undefined4 ui2Base::DAT_008d53e4 = 0;
    undefined4 ui2Base::DAT_008d53e8 = 0;
    undefined4 ui2Base::DAT_008d53ec = 0;
    undefined4 ui2Base::DAT_008d53f0 = 0;
    undefined4 ui2Base::DAT_008d53f4 = 0;
    undefined4 ui2Base::DAT_008d53f8 = 0;
    undefined4 ui2Base::DAT_008d5400 = 0;
    undefined4 ui2Base::DAT_008d5418 = 0;
    undefined4 ui2Base::DAT_008d5388 = 0;
    undefined4 ui2Base::DAT_008d5390 = 0;
    undefined4 ui2Base::DAT_008d539c = 0;
    undefined4 ui2Base::DAT_008d5398 = 0;
    undefined4 ui2Base::DAT_008d53a0 = 0;
    undefined4 ui2Base::DAT_008d53ac = 0;
    undefined4 ui2Base::DAT_008d5404 = 0;
    undefined4 ui2Base::DAT_008d5408 = 0;
    undefined4 ui2Base::DAT_008d540c = 0;
    undefined4 ui2Base::DAT_008d5410 = 0;
    undefined4 ui2Base::DAT_008d5414 = 0;
    undefined4 ui2Base::DAT_008d538c = 0;
    undefined4 ui2Base::DAT_008d5394 = 0;
    undefined4 ui2Base::DAT_008d53a4 = 0;
    undefined4 ui2Base::DAT_008d5384 = 0;
    undefined4 ui2Base::DAT_008d53a8 = 0;
    undefined4 ui2Base::DAT_008d53d8 = 0;
    undefined4 ui2Base::DAT_00362d78 = 0;
    undefined4 ui2Base::DAT_008d53b0 = 0;
    undefined4 ui2Base::DAT_008d53b4 = 0;
    undefined4 ui2Base::DAT_008d53bc = 0;
    undefined4 ui2Base::DAT_008d53b8 = 0;
    undefined4 ui2Base::DAT_008d53c0 = 0;
    undefined4 ui2Base::DAT_008d53cc = 0;
    undefined4 ui2Base::DAT_008d53c8 = 0;
    undefined4 ui2Base::DAT_008d53d4 = 0;
    undefined4 ui2Base::DAT_008d53dc = 0;
    undefined4 ui2Base::DAT_008d53c4 = 0;
    undefined4 ui2Base::DAT_008d53d0 = 0;
    undefined4 ui2Base::DAT_00362d14 = 0;
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

        FUN_001fe638();
    }

    ui2Base::~ui2Base() {
        FUN_001f2ae0();
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
    
    void ui2Base::method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3) {
        if (!field_0x1c || p2 != SOME_WIDGET_ENUM::UNK46) {
            ui2Widget::method_0x30(p1, p2, p3);
            return;
        }

        if (!p3) return;

        loadStringsInner(p3.cast<ui2Unknown0>());
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

        m_master->method_0xc8();

        ui::BaseRef w;
        m_master->method_0x98(SOME_WIDGET_ENUM::UNK47, lang.cast<ui2WidgetBase>(), w);

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
        WidgetBinTree found = m_widgets.findByName(p1->getName());
        if (found.getRoot() != m_widgets.getRoot()) {
            // todo: assert
            exit(-1);
        }

        m_widgets.FUN_00204b68(p1->getName());
        return 1;
    }

    u32 ui2Base::FUN_00203d38(const ui::NamedRef& p1) {
        return m_widgets.FUN_00205458(p1->getName());
    }

    ui::BaseRef ui2Base::findWidget(const char* name, const char* type) {
        WidgetBinTree::Node* n = m_widgets.findWidgetByName(name);

        if (n == m_widgets.getRoot()) {
            return ui::BaseRef();
        }

        if (type && !n->widget.cast<ui2Widget>()->isA(type)) {
            return ui::BaseRef();
        }

        return n->widget;
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
    
    ui2String* ui2Base::getSomeStrings() {
        return globalMaster->m_someStrings;
    }

    void ui2Base::FUN_001f2ae0() {
        if (DAT_00362d14) {
            FUN_002f7e88();
            DAT_00362d14 = 0;
        }
    }

    void ui2Base::FUN_001fe638() {
        DAT_00362d7c = 0x331;
        FLOAT_00362d84 = 0.15;
        FLOAT_00362d88 = 0.5;
        FLOAT_00362d90 = 0.7;
        DAT_008d53e0 = 0x1c;
        DAT_008d53fc = 0xf;
        DAT_008d541c = 0x100;
        FLOAT_00362d8c = 0.7;
        DAT_008d5380 = 0x2000;
        DAT_00362d80 = 7;
        DAT_008d53e4 = 0xe;
        DAT_008d53e8 = 200;
        DAT_008d53ec = 0xd0;
        DAT_008d53f0 = 0xcb;
        DAT_008d53f4 = 0xcd;
        DAT_008d53f8 = 0x39;
        DAT_008d5400 = 0x40;
        DAT_008d5418 = 0x800;
        DAT_008d5388 = 0x8000;
        DAT_008d5390 = 0x1000;
        DAT_008d539c = 3;
        DAT_008d5398 = 0x4000;
        DAT_008d53a0 = 0x10;
        DAT_008d53ac = 5;
        DAT_008d5404 = 0x10;
        DAT_008d5408 = 0x1000;
        DAT_008d540c = 0x4000;
        DAT_008d5410 = 0x8000;
        DAT_008d5414 = 0x2000;
        DAT_008d538c = 1;
        DAT_008d5394 = 2;
        DAT_008d53a4 = 4;
        DAT_008d5384 = 0;
        DAT_008d53a8 = 0x20;
        DAT_008d53d8 = 2;
        DAT_00362d78 = 1;
        DAT_008d53b0 = 0x40;
        DAT_008d53b4 = 6;
        DAT_008d53bc = 7;
        DAT_008d53b8 = 0x80;
        DAT_008d53c0 = 4;
        DAT_008d53cc = 9;
        DAT_008d53c8 = 8;
        DAT_008d53d4 = 10;
        DAT_008d53dc = 0xb;
        DAT_008d53c4 = 8;
        DAT_008d53d0 = 1;
    }

    void ui2Base::FUN_002f7e88() {
        // todo
    }
};