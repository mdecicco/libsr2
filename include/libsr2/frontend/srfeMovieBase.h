#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/srui2SRScreen.h>
#include <libsr2/ui/ui2Movie.h>
#include <libsr2/ui/ui2DynamicText.h>
#include <libsr2/ui/ui2Timer.h>

namespace sr2 {
    class srfeMovieBase : public srui2SRScreen {
        public:
            srfeMovieBase(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeMovieBase();

            virtual void draw();
            virtual void setActive(bool isActive);
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;
            virtual void initScreen();
            virtual void deinitScreen();

            void setNextScreenName(const char* p1);
            void set0xb8(f32 p1);
            void set0xbc(undefined4 p1);
            void set0x40c();
            void FUN_0011db18();
            void FUN_0011db70();
            void FUN_0011dbc0();
            void FUN_0011dd08();
            void FUN_0011ddb8();
            void FUN_0011e130();
            void interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            void loadSubtitles(const char* path, i32 p2, u32 color);

            static bool isWidgetAMovie(const char* someWidgetName);
            static bool ShowSubtitles;
        
        protected:
            struct subtitle {
                f32 timeOffset;
                char* langTexts[4];
            };

            struct subtitle_data {
                bool isLoaded;
                undefined4 currentIdx;
                undefined4 field_0x8;
                undefined4 color;
                subtitle subtitles[40];
            };

            subtitle_data m_subtitleData;
            WidgetRef<ui2Movie> m_movie;
            WidgetRef<ui2Timer> m_timer;
            WidgetRef<ui2Timer> m_subTimer;
            WidgetRef<ui2DynamicText> m_subtitleText;
            ui2String m_nextScreenName;

            undefined4 field_0xb4;
            f32 field_0xb8;
            undefined4 field_0xbc;
            undefined4 field_0x408;
            undefined4 field_0x40c;
    };
};