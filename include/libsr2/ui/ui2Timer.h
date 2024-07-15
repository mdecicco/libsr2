#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/FloatEventData.h>

namespace sr2 {
    class ui2Timer : public ui2Widget {
        public:
            enum class State {
                Starting = 0,
                Running = 1,
                Stopped = 2
            };

            ui2Timer(const char* name, f32 duration, bool doLoop, bool doStart, const WidgetRef<ui2Master>& master);
            virtual ~ui2Timer();

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void draw();

            void init(f32 duration, bool doLoop, bool doStart);
            void startTimer();
            void stopTimer();
            void setDuration(const WidgetRef<FloatEventData>& event);
            void setDuration(f32 duration);
            void FUN_002005f0(undefined4 p1);
        
        protected:
            f32 m_startTime;
            f32 m_duration;
            State m_state;
            bool m_doLoop;
            WidgetRef<FloatEventData> m_event;

            undefined4 field_0x88;
    };
};