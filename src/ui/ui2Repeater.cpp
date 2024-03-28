#include <libsr2/ui/ui2Repeater.h>

#include <string.h>

namespace sr2 {
    ui2Repeater::ui2Repeater(char* name, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        field_0x78 = 1;
        field_0x7c = 1;

        addToMasterUnk0(5000, 0x800090000);
    }

    ui2Repeater::~ui2Repeater() {
    }

    void ui2Repeater::onEvent(const ui::BaseRef& p1, WidgetEventType p2, const ui::BaseRef& p3) {
        bool someCond = false;

        if (!field_0x1c) {
            if (p2 == WidgetEventType::UNK0) {
                ui2Widget::onEvent(p1, p2, p3);
            }

            someCond = field_0x1c;
            
            if (!field_0x1c) {
                if (!field_0x7c || p2 != WidgetEventType::UNK2) {
                    ui2Widget::onEvent(p1, p2, p3);
                }
                return;
            }
        }

        if (field_0x78 == 1) {
            ui::BaseRef w = p1;
            method_0x98(p2, p3, w);
        } else {
            ui::BaseRef w;
            method_0x98(p2, p3, w);
        }
            
        if (!someCond && !field_0x7c || p2 != WidgetEventType::UNK2) {
            ui2Widget::onEvent(p1, p2, p3);
        }
    }

    const char* ui2Repeater::getType() const {
        return "ui2Repeater";
    }

    bool ui2Repeater::isA(const char* type) const {
        if (strcmp("ui2Repeater", type) == 0) return true;
        return strcmp(getType(), type) == 0;
    }
};