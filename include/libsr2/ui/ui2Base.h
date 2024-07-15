#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/WidgetBinTree.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Master.h>
#include <libsr2/ui/ui2Unknown0.h>
#include <libsr2/i18n/txtStringTable.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace sr2 {
    class ui2Base : public ui2Widget {
        public:
            ui2Base(const char* name);
            virtual ~ui2Base();

            virtual void releaseRef();
            virtual void method_0x48();
            virtual bool loadWidget();
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void draw();

            virtual void loadStrings(const char* p1, LANGUAGE lang, u32 stringFlags);
            virtual void loadStringsInner(const WidgetRef<ui2Unknown0>& p1);
            virtual void pushMaster(const WidgetRef<ui2Master>& p1);
            virtual void popMaster();
            virtual WidgetRef<ui2Master> currentMaster();
            undefined4 FUN_00203bb0(const ui::NamedRef& p1);
            u32 FUN_00203d38(const ui::NamedRef& p1);
            ui::BaseRef findWidget(const char* name, const char* type = nullptr);

            
            void getLangInfo(LANGUAGE* outLang, u32* outFlags);
            void setDirectory(const char* dir);
            const char* getDirectory();
            ui::BaseRef* FUN_00203b88();

            static u32 getNextWidgetId();
            static WidgetRef<ui2Base> getGlobalMaster();
            static WidgetRef<ui2Master> getMaster();
            static txtStringTable& getGameplayText();
            static ui2String* getSomeStrings();

            static void FUN_001f2ae0();
            static void FUN_002f7e88();
        
        protected:
            static u32 nextWidgetId;
            static ui2Base* globalMaster;

            WidgetBinTree m_widgets;
            std::unordered_map<std::string, ui2Widget*> m_widgetsTestMap;


            WidgetRef<ui2Master> m_master;
            ui2String m_languageDirs[10];
            ui2String m_someStrings[3];
            txtStringTable m_gameplayText;
            LANGUAGE m_currentLang;
            u32 m_stringFlags;
            std::vector<WidgetRef<ui2Master>> m_masters;

            char field_0x98[32];
            WidgetRef<ui2Unknown0> field_0xc0;
            undefined4 field_0x168;
            undefined4 field_0x16c;
            undefined4 field_0x170;
            undefined4 field_0x174;
    };
};