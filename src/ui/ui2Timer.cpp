#include <libsr2/ui/ui2Timer.h>
#include <libsr2/managers/datTimeManager.h>

namespace sr2 {
    ui2Timer::ui2Timer(const char* name, f32 duration, bool doLoop, bool doStart, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        m_state = State::Stopped;
        m_startTime = 0.0f;
        field_0x88 = 0;

        init(duration, doLoop, doStart);
    }

    ui2Timer::~ui2Timer() {
    }
    
    void ui2Timer::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) {
            ui2Widget::onEvent(source, event, data);
            return;
        }

        switch (event) {
            case WidgetEventType::StartTimer: {
                startTimer();
                break;
            }
            case WidgetEventType::SetTimerDuration: {
                if (data) setDuration(data.cast<FloatEventData>());

                if (field_0x88 == 1 && (u32(event) & 0x40000000)) startTimer();

                ui2Widget::onEvent(source, event, data);
                break;
            }
        }
    }

    void ui2Timer::draw() {
        ui2Widget::draw();
        if (!m_isActive) return;

        f32 elapsed = datTimeManager::ElapsedTime;
        switch (m_state) {
            case State::Starting: {
                m_state = State::Running;
                m_startTime = elapsed;

                dispatchEvent(WidgetEventType::TimerStarted, nullptr);

                break;
            }
            case State::Running: {
                if (m_startTime + m_duration >= elapsed) return;

                dispatchEvent(WidgetEventType::TimerFinished, nullptr);

                if (!m_doLoop) {
                    m_state = State::Stopped;
                    return;
                }

                m_startTime = elapsed;
                m_state = State::Running;

                dispatchEvent(WidgetEventType::TimerStarted, nullptr);

                break;
            }
            case State::Stopped: {
                break;
            }
        }
    }

    void ui2Timer::init(f32 duration, bool doLoop, bool doStart) {
        m_doLoop = doLoop;
        m_duration = duration;

        m_event = new FloatEventData();

        if (doStart) startTimer();

        addToMasterUnk0(50000, 0x800090000);
    }

    void ui2Timer::startTimer() {
        m_state = State::Starting;
    }

    void ui2Timer::stopTimer() {
        m_state = State::Stopped;
    }
    
    void ui2Timer::setDuration(const WidgetRef<FloatEventData>& event) {
        if (m_duration != event->data) {
            m_duration = event->data;

            dispatchEvent(WidgetEventType::TimerDurationChanged, event);
        }
    }
    
    void ui2Timer::setDuration(f32 duration) {
        m_event->data = duration;
        setDuration(m_event);
    }
    
    void ui2Timer::FUN_002005f0(undefined4 p1) {
        field_0x88 = p1;
    }
};