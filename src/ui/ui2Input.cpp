#include <libsr2/ui/ui2Input.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/io/ioPad.h>
#include <libsr2/io/ioKeyboard.h>
#include <libsr2/globals.h>
#include <libsr2/math/math.h>

namespace sr2 {
    constexpr u32 UNK_ACCEPT = 0;
    constexpr u32 UNK_BACK = 1;
    constexpr u32 UNK_UP = 2;
    constexpr u32 UNK_DOWN = 3;
    constexpr u32 UNK_LEFT = 4;
    constexpr u32 UNK_RIGHT = 5;
    constexpr u32 UNK_START = 6;
    constexpr u32 UNK_SELECT = 7;


    ui2Input::ui2Input(const char* name, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        field_0x78 = DAT_00362d7c;
        field_0x7c = DAT_00362d80;
        m_someDeadzone = FLOAT_00362d8c;
        field_0x84 = FLOAT_00362d90;

        for (u32 i = 0;i < 8;i++) {
            field_0xa8[i] = 0;
            field_0xe8[i] = 0;
            m_pressedAtTime[i] = 0.0f;
            m_pressedAmount[i] = 0.0f;
        }

        makeRenderable(100000);
    }

    ui2Input::~ui2Input() {
    }

    void ui2Input::addListener(const ui::NamedRef& listener, WidgetEventType event, WidgetEventCallback acceptOverride) {
        if (event == WidgetEventType::AnyInput) {
            ui2Widget::addListener(listener, WidgetEventType::AcceptPressed, acceptOverride);
            ui2Widget::addListener(listener, WidgetEventType::BackPressed, acceptOverride);
            ui2Widget::addListener(listener, WidgetEventType::UpPressed, acceptOverride);
            ui2Widget::addListener(listener, WidgetEventType::DownPressed, acceptOverride);
            ui2Widget::addListener(listener, WidgetEventType::LeftPressed, acceptOverride);
            ui2Widget::addListener(listener, WidgetEventType::RightPressed, acceptOverride);
        } else {
            ui2Widget::addListener(listener, event, acceptOverride);
        }
    }

    void ui2Input::removeListener(const ui::NamedRef& listener, WidgetEventType event) {
        if (event == WidgetEventType::AnyInput) {
            ui2Widget::removeListener(listener, WidgetEventType::AcceptPressed);
            ui2Widget::removeListener(listener, WidgetEventType::BackPressed);
            ui2Widget::removeListener(listener, WidgetEventType::UpPressed);
            ui2Widget::removeListener(listener, WidgetEventType::DownPressed);
            ui2Widget::removeListener(listener, WidgetEventType::LeftPressed);
            ui2Widget::removeListener(listener, WidgetEventType::RightPressed);
        } else {
            ui2Widget::removeListener(listener, event);
        }
    }

    void ui2Input::draw() {
        f32 elapsed = datTimeManager::ElapsedTime;
        ui2Widget::draw();

        if (!m_isActive) return;
        
        u32 v39[8];
        processController();
        processKeyboard(v39);
        processWheel(v39);

        for (u32 i = 0;i < 8;i++) {
            if (field_0xa8[i]) {
                f32 dt = m_pressedAtTime[i] - elapsed;
                f32 absDt = dt < 0.0 ? -dt : dt;

                if ((FLOAT_00362d88 >= absDt || field_0xe8[i] != 1) && (FLOAT_00362d84 >= absDt || field_0xe8[i])) {
                    if (!v39[i]) field_0xa8[i] = 0;
                    continue;
                }
            }
            
            if (v39[i] == 1) {
                switch (i) {
                    case UNK_ACCEPT: {
                        if ((field_0x7c & 4) && v39[UNK_BACK]) break;
                        if ((field_0x7c & 8) && v39[UNK_START]) break;

                        dispatchEvent(WidgetEventType::AcceptPressed);
                        break;
                    }
                    case UNK_BACK: {
                        if ((field_0x7c & 4) && v39[UNK_ACCEPT]) break;

                        dispatchEvent(WidgetEventType::BackPressed);
                        break;
                    }
                    case UNK_UP: {
                        if (((field_0x7c ^ 1) & 1) == 0 && v39[UNK_DOWN]) break;

                        dispatchEvent(WidgetEventType::UpPressed);
                        break;
                    }
                    case UNK_DOWN: {
                        if (((field_0x7c ^ 1) & 1) == 0 && v39[UNK_UP]) break;

                        dispatchEvent(WidgetEventType::DownPressed);
                        break;
                    }
                    case UNK_LEFT: {
                        if ((field_0x7c & 2) && v39[UNK_RIGHT]) break;

                        dispatchEvent(WidgetEventType::LeftPressed);
                        break;
                    }
                    case UNK_RIGHT: {
                        if ((field_0x7c & 2) && v39[UNK_LEFT]) break;

                        dispatchEvent(WidgetEventType::RightPressed);
                        break;
                    }
                    case UNK_START: {
                        dispatchEvent(WidgetEventType::StartPressed);
                        break;
                    }
                    case UNK_SELECT: {
                        dispatchEvent(WidgetEventType::SelectPressed);
                        break;
                    }
                    default: break;
                }
                        
                if (field_0xa8[i]) field_0xe8[i] = 0;
                else field_0xe8[i] = 1;
                m_pressedAtTime[i] = elapsed;
                field_0xa8[i] = 1;
            }
            
            if (!v39[i]) field_0xa8[i] = 0;
        }
    }

    void ui2Input::processKeyboard(u32* p1) {
        Button buttons[2] = {
            ioPad::All[0].buttons,
            ioPad::All[1].buttons
        };
        bool* keys = ioKeyboard::gKeyboard.isActive ? ioKeyboard::gKeyboard.keys.active : ioKeyboard::gKeyboard.keys.inactive;
        
        for (u32 i = 0;i < 8;i++) {
            p1[i] = 0;

            if ((field_0x78 & 1) && keys[uiKeyboardActionMap[i]]) p1[i] = 1;

            u32 uVar2 = 0;
            if (i < 2) {
                if ((field_0x78 & 0x30) && (buttons[0] & uiControllerActionMap[i])) p1[i] = 1;
                uVar2 = field_0x78 & 0x300;
            } else {
                if ((field_0x78 & 0x10) && (buttons[0] & uiControllerActionMap[i])) p1[i] = 1;
                uVar2 = field_0x78 & 0x100;
            }

            if (uVar2 && (buttons[1] & uiControllerActionMap[i])) p1[i] = 1;
        }

        FUN_001ff4b8(0, p1);
        FUN_001ff4b8(1, p1);
    }
    
    void ui2Input::processWheel(u32* p1) {
        // something involving ioFFWheel
        // I'll choose to ignore it since it probably
        // only has an effect if a wheel-type controller
        // is connected
    }
    
    void ui2Input::FUN_001ff4b8(u32 p1, u32* p2) {
        if (p1 == 0 && (field_0x78 & 0x20) == 0) return;
        if (p1 == 1 && (field_0x78 & 0x200) == 0) return;
        if (ioPad::All[p1].state != IO_PAD_STATE::Stable) return;
        if ((u32(ioPad::All[p1].mode) & u32(PAD_MODE::UNK0)) == 0x40) return;

        vec2f analog = {
            math::clamp((f32(ioPad::All[p1].leftStick.x) - 127.5f) * 0.007843138f, -1.0f, 1.0f),
            math::clamp((f32(ioPad::All[p1].leftStick.y) - 127.5f) * 0.007843138f, -1.0f, 1.0f)
        };

        if (analog.x < -m_someDeadzone) p2[UNK_LEFT] = 1;
        if (analog.x > m_someDeadzone) p2[UNK_RIGHT] = 1;
        if (analog.y < -m_someDeadzone) p2[UNK_UP] = 1;
        if (analog.y > m_someDeadzone) p2[UNK_DOWN] = 1;
    }
    
    u32 ui2Input::processController() {
        for (u32 i = 0;i < 8;i++) m_pressedAmount[i] = 0.0f;

        if (field_0x78 & 1) {          
            bool* keys = ioKeyboard::gKeyboard.isActive ? ioKeyboard::gKeyboard.keys.active : ioKeyboard::gKeyboard.keys.inactive;
            for (u32 i = 0;i < 8;i++) {
                if (keys[uiKeyboardActionMap[i]]) {
                    m_pressedAmount[i] = math::max(m_pressedAmount[i], field_0x84);
                }
            }
        }

        if ((field_0x78 & 0x20) && (u32(ioPad::All[0].mode) & u32(PAD_MODE::UNK0)) != 0x40 && ioPad::All[0].state == IO_PAD_STATE::Stable) {
            vec2f analog = {
                math::clamp((f32(ioPad::All[0].leftStick.x) - 127.5f) * 0.007843138f, -1.0f, 1.0f),
                math::clamp((f32(ioPad::All[0].leftStick.y) - 127.5f) * 0.007843138f, -1.0f, 1.0f)
            };

            vec2f transformed = {
                ioAddDeadZone(analog.x, m_someDeadzone),
                ioAddDeadZone(analog.y, m_someDeadzone)
            };

            FUN_001ffba8(transformed.x, transformed.y);
        }

        if ((field_0x78 & 0x200) && (u32(ioPad::All[1].mode) & u32(PAD_MODE::UNK0)) != 0x40 && ioPad::All[1].state == IO_PAD_STATE::Stable) {
            vec2f analog = {
                math::clamp((f32(ioPad::All[1].leftStick.x) - 127.5f) * 0.007843138f, -1.0f, 1.0f),
                math::clamp((f32(ioPad::All[1].leftStick.y) - 127.5f) * 0.007843138f, -1.0f, 1.0f)
            };

            vec2f transformed = {
                ioAddDeadZone(analog.x, m_someDeadzone),
                ioAddDeadZone(analog.y, m_someDeadzone)
            };

            FUN_001ffba8(transformed.x, transformed.y);
        }

        if (field_0x78 & 0x10) {
            m_pressedAmount[UNK_UP] = math::max(m_pressedAmount[UNK_UP], FUN_001ffc18(0, UNK_UP));
            m_pressedAmount[UNK_DOWN] = math::max(m_pressedAmount[UNK_DOWN], FUN_001ffc18(0, UNK_DOWN));
            m_pressedAmount[UNK_LEFT] = math::max(m_pressedAmount[UNK_LEFT], FUN_001ffc18(0, UNK_LEFT));
            m_pressedAmount[UNK_RIGHT] = math::max(m_pressedAmount[UNK_RIGHT], FUN_001ffc18(0, UNK_RIGHT));
        }

        if (field_0x78 & 0x100) {
            m_pressedAmount[UNK_UP] = math::max(m_pressedAmount[UNK_UP], FUN_001ffc18(1, UNK_UP));
            m_pressedAmount[UNK_DOWN] = math::max(m_pressedAmount[UNK_DOWN], FUN_001ffc18(1, UNK_DOWN));
            m_pressedAmount[UNK_LEFT] = math::max(m_pressedAmount[UNK_LEFT], FUN_001ffc18(1, UNK_LEFT));
            m_pressedAmount[UNK_RIGHT] = math::max(m_pressedAmount[UNK_RIGHT], FUN_001ffc18(1, UNK_RIGHT));
        }
        
        if (field_0x78 & 0x30) {
            m_pressedAmount[UNK_ACCEPT] = math::max(m_pressedAmount[UNK_ACCEPT], FUN_001ffc18(0, UNK_ACCEPT));
            m_pressedAmount[UNK_BACK] = math::max(m_pressedAmount[UNK_BACK], FUN_001ffc18(0, UNK_BACK));
        }
        
        u32 ret = field_0x78 & 0x300;
        if (ret != 0) {
            m_pressedAmount[UNK_ACCEPT] = math::max(m_pressedAmount[UNK_ACCEPT], FUN_001ffc18(1, UNK_ACCEPT));
            m_pressedAmount[UNK_BACK] = math::max(m_pressedAmount[UNK_BACK], FUN_001ffc18(1, UNK_BACK));
        }

        return ret;
    }
    
    void ui2Input::FUN_001ffba8(f32 ax, f32 ay) {
        if (ax < 0.0f) m_pressedAmount[UNK_LEFT] = math::max(-ax, m_pressedAmount[UNK_LEFT]);
        else m_pressedAmount[UNK_RIGHT] = math::max(ax, m_pressedAmount[UNK_RIGHT]);

        if (ay < 0.0f) m_pressedAmount[UNK_UP] = math::max(-ay, m_pressedAmount[UNK_UP]);
        else m_pressedAmount[UNK_DOWN] = math::max(ay, m_pressedAmount[UNK_DOWN]);
    }
    
    f32 ui2Input::FUN_001ffc18(i32 padIdx, i32 p2) {
        f32 ret = 0.0;
        u32 btns = ioPad::All[padIdx].buttons;

        if (ioPad::All[padIdx].field_0x178 == 1) {
            btns &= Button::SELECT | Button::L3 | Button::R3 | Button::START;

            u8 maxUnk = 0;
            for (u32 i = 0;i < 12;i++) {
                if (uiControllerActionMap[p2] & DAT_008d5380[i].btn) {
                    u8 val = ioPad::All[padIdx].padUnk[DAT_008d5380[i].padUnkIdx];
                    if (val > maxUnk) maxUnk = val;
                }
            }

            ret = f32(maxUnk) / 255.0f;
        }

        if (btns & uiControllerActionMap[p2]) ret = math::max(field_0x84, ret);

        return ret;
    }

    void ui2Input::FUN_001fee50() {
        f32 t = datTimeManager::ElapsedTime;
        for (u32 i = 0;i < 8;i++) {
            m_pressedAtTime[i] = t;
            field_0xa8[i] = 1;
            field_0xe8[i] = 1;
        }
    }
    
    f32 ui2Input::FUN_001fee98(u32 idx) {
        return m_pressedAmount[idx];
    }

    void ui2Input::FUN_001fe638() {
        FLOAT_00362d84 = 0.15f;
        FLOAT_00362d88 = 0.5f;
        FLOAT_00362d8c = 0.7f;
        FLOAT_00362d90 = 0.7f;

        DAT_00362d78 = 1;
        DAT_00362d7c = 0x331;
        DAT_00362d80 = 7;

        DAT_008d5380[0]  = { Button::RIGHT, 0 };
        DAT_008d5380[1]  = { Button::LEFT, 1 };
        DAT_008d5380[2]  = { Button::UP, 2 };
        DAT_008d5380[3]  = { Button::DOWN, 3 };
        DAT_008d5380[4]  = { Button::TRIANGLE, 4 };
        DAT_008d5380[5]  = { Button::CIRCLE, 5 };
        DAT_008d5380[6]  = { Button::CROSS, 6 };
        DAT_008d5380[7]  = { Button::SQUARE, 7 };
        DAT_008d5380[8]  = { Button::L1, 8 };
        DAT_008d5380[9]  = { Button::R1, 9 };
        DAT_008d5380[10] = { Button::L2, 10 };
        DAT_008d5380[11] = { Button::R2, 11 };

        uiKeyboardActionMap[UNK_ACCEPT] = KEY_CODE::UNK28;
        uiKeyboardActionMap[UNK_BACK] = KEY_CODE::UNK14;
        uiKeyboardActionMap[UNK_UP] = KEY_CODE::UNK200;
        uiKeyboardActionMap[UNK_DOWN] = KEY_CODE::UNK208;
        uiKeyboardActionMap[UNK_LEFT] = KEY_CODE::UNK203;
        uiKeyboardActionMap[UNK_RIGHT] = KEY_CODE::UNK205;
        uiKeyboardActionMap[UNK_START] = KEY_CODE::UNK57;
        uiKeyboardActionMap[UNK_SELECT] = KEY_CODE::UNK15;

        uiControllerActionMap[UNK_ACCEPT] = Button::CROSS;
        uiControllerActionMap[UNK_BACK] = Button::TRIANGLE;
        uiControllerActionMap[UNK_UP] = Button::UP;
        uiControllerActionMap[UNK_DOWN] = Button::DOWN;
        uiControllerActionMap[UNK_LEFT] = Button::LEFT;
        uiControllerActionMap[UNK_RIGHT] = Button::RIGHT;
        uiControllerActionMap[UNK_START] = Button::START;
        uiControllerActionMap[UNK_SELECT] = Button::SELECT;
    }
};