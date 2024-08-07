#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/srui2SRScreen.h>
#include <libsr2/ui/ui2TranslatedText.h>
#include <libsr2/ui/ui2Sound.h>
#include <libsr2/ui/ui2Movie.h>

namespace sr2 {
    class srfeBackground : public srui2SRScreen {
        public:
            struct Widgets {
                WidgetRef<ui2Master> master;
                WidgetRef<ui2Movie> bg_movie;
                WidgetRef<ui2Sound> movie_score_snd;
                WidgetRef<ui2Sound> bluebar_l_snd;
                WidgetRef<ui2Sound> bluebar_r_snd;
                WidgetRef<ui2Sound> navok_l_snd;
                WidgetRef<ui2Sound> navok_r_snd;
                WidgetRef<ui2Sound> showbuttons_l_snd;
                WidgetRef<ui2Sound> showbuttons_r_snd;
                WidgetRef<ui2Sound> showtexts_l_snd;
                WidgetRef<ui2Sound> showtexts_r_snd;
            };

            srfeBackground(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeBackground();

            virtual void setActive(bool p1);
            virtual void initScreen();
            virtual void deinitScreen();
            virtual void method_0x108();
            virtual void method_0x110();
            virtual bool method_0x118();
            virtual bool method_0x120();
        
        protected:
            static u32 ActiveCount;
            static u32 InitializedCount;
            static Widgets* WidgetsInstance;
            static void initWidgets();

            WidgetRef<ui2TranslatedText> m_headingText;
            bool m_wasUninitialized;
    };
};