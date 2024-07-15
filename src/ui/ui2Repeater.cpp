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

    void ui2Repeater::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        bool someCond = false;

        if (!m_isActive) {
            if (event == WidgetEventType::UNK0) {
                ui2Widget::onEvent(source, event, data);
            }

            someCond = m_isActive;
            
            if (!m_isActive) {
                if (!field_0x7c || event != WidgetEventType::UNK2) {
                    ui2Widget::onEvent(source, event, data);
                }
                return;
            }
        }

        if (field_0x78 == 1) dispatchEvent(event, data, source);
        else dispatchEvent(event, data);
            
        if (!someCond && !field_0x7c || event != WidgetEventType::UNK2) {
            ui2Widget::onEvent(source, event, data);
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