#include <libsr2/ui/ui2UnknownMenu0.h>
#include <string.h>

namespace sr2 {
    ui2UnknownMenu0::ui2UnknownMenu0(
        const char* name,
        i32 rowCount,
        i32 colCount,
        const WidgetRef<ui2Master>& master
    ) : ui2Menu(name, rowCount, colCount, master) {
        init(name);
    }

    ui2UnknownMenu0::~ui2UnknownMenu0() {
    }

    void ui2UnknownMenu0::init(const char* name) {
        u32 len = strlen(name) + 20;
        char* buffer = new char[len];
        snprintf(buffer, len, "%s%s", name, "_sel_l_snd");

        m_selectL = new ui2Sound(buffer, 62, nullptr);
        m_selectL->FUN_00208068(-1.0f);
        m_selectL->addEventMapper(WidgetEventType::UNK29, WidgetEventType::UNK33, nullptr);
        addListener(m_selectL, WidgetEventType::UNK29);
        m_selectL->FUN_00207fc8();

        snprintf(buffer, len, "%s%s", name, "_sel_r_snd");
        m_selectR = new ui2Sound(buffer, 63, nullptr);
        m_selectR->FUN_00208068(1.0f);
        m_selectR->addEventMapper(WidgetEventType::UNK29, WidgetEventType::UNK33, nullptr);
        addListener(m_selectR, WidgetEventType::UNK29);
        m_selectR->FUN_00207fc8();

        delete [] buffer;
    }

    void ui2UnknownMenu0::reset() {
        m_selectL = nullptr;
        m_selectR = nullptr;
        ui2Menu::reset();
    }

    void ui2UnknownMenu0::FUN_001f1a20() {
        m_selectL->FUN_00207fc8();
        m_selectR->FUN_00207fc8();
    }

    void ui2UnknownMenu0::FUN_001f1a50() {
        m_selectL->FUN_00207fd8();
        m_selectR->FUN_00207fd8();
    }
}