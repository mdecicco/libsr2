#include <libsr2/ui/ui2Variable.h>

#include <string.h>

namespace sr2 {
    ui2Variable::ui2Variable(
        const char* name,
        u32 sz,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true), m_stringData(sz) {
        m_integerData = 0;
        m_type = VariableType::Unknown;

        init();
    }

    ui2Variable::~ui2Variable() {
    }
    
    void ui2Variable::init() {
        m_intEventData = new IntegerEventData();
        m_strEventData = new ASCIIStringEventData();
    }

    void ui2Variable::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) {
            ui2Widget::onEvent(source, event, data);
            return;
        }

        if (event == WidgetEventType::UNK26) {
            if (!data) return;
            setStringData(data.cast<ASCIIStringEventData>());
        } else {
            if (event < WidgetEventType::UNK43) {
                if (event == WidgetEventType::UNK37) {
                    if (!data) return;
                    setIntegerData(data.cast<IntegerEventData>());
                    return;
                } else if (event == WidgetEventType::UNK44) {
                    maybeClearValue();
                    return;
                }
            }

            ui2Widget::onEvent(source, event, data);
        }
    }

    void ui2Variable::setIntegerData(i32 data) {
        m_intEventData->data = data;
        setIntegerData(m_intEventData);
    }
    
    void ui2Variable::setIntegerData(const WidgetRef<IntegerEventData>& data) {
        if (m_type == VariableType::Integer && m_integerData == data->data) return;

        m_type = VariableType::Integer;
        m_integerData = data->data;

        dispatchEvent(WidgetEventType::UNK39, data);
    }
    
    void ui2Variable::setStringData(const char* data) {
        m_strEventData->data = data;
        setStringData(m_strEventData);
    }
    
    void ui2Variable::setStringData(const WidgetRef<ASCIIStringEventData>& data) {
        const char* v = data->data;
        if (m_type == VariableType::String && strcmp(m_stringData.get(), v) == 0) return;

        m_stringData.set(v);
        m_type = VariableType::String;

        dispatchEvent(WidgetEventType::UNK40, data);
    }

    void ui2Variable::maybeClearValue() {
        if (m_type == VariableType::Unknown) return;
        m_type = VariableType::Unknown;

        dispatchEvent(WidgetEventType::UNK45, nullptr);
    }

    VariableType ui2Variable::getType() {
        return m_type;
    }

    i32 ui2Variable::getIntegerData() {
        return m_integerData;
    }

    const char* ui2Variable::getStringData() {
        return m_stringData.get();
    }
    
    WidgetRef<ui2EventData> ui2Variable::getEvent() {
        if (m_type == VariableType::Integer) {
            m_intEventData->data = m_integerData;
            return m_intEventData;
        } else if (m_type == VariableType::String) {
            m_strEventData->data = m_stringData.get();
            return m_strEventData;
        }

        return nullptr;
    }
};