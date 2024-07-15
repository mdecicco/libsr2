#include <libsr2/io/ioPad.h>

namespace sr2 {
    ioPad ioPad::All[4] = {
        ioPad(),
        ioPad(),
        ioPad(),
        ioPad()
    };

    bool ioPad::MultitapConnected = false;

    ioPad::ioPad() {
        buttons = Button::None;
        lastButtons = Button::None;
        buttonsUnk = 0xffff;
    }

    ioPad::~ioPad() {
    }

    u32 ioPad::attach() {
        // todo?
        state = IO_PAD_STATE::Stable;
        return 7;
    }

    void ioPad::update() {
        if (state == IO_PAD_STATE::UNK0 || attach() != 7) {
            buttons = lastButtons = Button::None;
            buttonsUnk = 0xffff;
            return;
        }
        
        read();
    }

    void ioPad::read() {
        // todo
    }

    void ioPad::UpdateAll() {
        for (u32 i = 0;i < 4;i++) {
            if (ioPad::MultitapConnected || i < 2) {
                ioPad::All[i].update();
            }
        }
    }

    
    f32 ioAddDeadZone(f32 p1, f32 p2) { 
        f32 fVar1;
        
        if (p2 < p1) fVar1 = p1 - p2;
        else {
            if (-p2 <= p1) return 0.0f;
            fVar1 = p1 + p2;
        }

        return fVar1 / (1.0f - p2);
    }
};