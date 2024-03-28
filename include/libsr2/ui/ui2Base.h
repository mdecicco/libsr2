#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/WidgetBinTree.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Master.h>
#include <libsr2/ui/ui2Unknown0.h>
#include <libsr2/i18n/txtStringTable.h>

#include <vector>

namespace sr2 {
    class ui2Base : public ui2Widget {
        public:
            ui2Base(const char* name);
            virtual ~ui2Base();

            virtual void releaseRef();
            virtual void method_0x48();
            virtual bool loadWidget();
            virtual void method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);
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
            static void FUN_001fe638();
            static void FUN_002f7e88();

            static undefined4 DAT_00362d7c;
            static f32 FLOAT_00362d84;
            static f32 FLOAT_00362d88;
            static f32 FLOAT_00362d90;
            static undefined4 DAT_008d53e0;
            static undefined4 DAT_008d53fc;
            static undefined4 DAT_008d541c;
            static f32 FLOAT_00362d8c;
            static undefined4 DAT_008d5380;
            static undefined4 DAT_00362d80;
            static undefined4 DAT_008d53e4;
            static undefined4 DAT_008d53e8;
            static undefined4 DAT_008d53ec;
            static undefined4 DAT_008d53f0;
            static undefined4 DAT_008d53f4;
            static undefined4 DAT_008d53f8;
            static undefined4 DAT_008d5400;
            static undefined4 DAT_008d5418;
            static undefined4 DAT_008d5388;
            static undefined4 DAT_008d5390;
            static undefined4 DAT_008d539c;
            static undefined4 DAT_008d5398;
            static undefined4 DAT_008d53a0;
            static undefined4 DAT_008d53ac;
            static undefined4 DAT_008d5404;
            static undefined4 DAT_008d5408;
            static undefined4 DAT_008d540c;
            static undefined4 DAT_008d5410;
            static undefined4 DAT_008d5414;
            static undefined4 DAT_008d538c;
            static undefined4 DAT_008d5394;
            static undefined4 DAT_008d53a4;
            static undefined4 DAT_008d5384;
            static undefined4 DAT_008d53a8;
            static undefined4 DAT_008d53d8;
            static undefined4 DAT_00362d78;
            static undefined4 DAT_008d53b0;
            static undefined4 DAT_008d53b4;
            static undefined4 DAT_008d53bc;
            static undefined4 DAT_008d53b8;
            static undefined4 DAT_008d53c0;
            static undefined4 DAT_008d53cc;
            static undefined4 DAT_008d53c8;
            static undefined4 DAT_008d53d4;
            static undefined4 DAT_008d53dc;
            static undefined4 DAT_008d53c4;
            static undefined4 DAT_008d53d0;
            static undefined4 DAT_00362d14;
        
        protected:
            static u32 nextWidgetId;
            static ui2Base* globalMaster;

            WidgetBinTree m_widgets;
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