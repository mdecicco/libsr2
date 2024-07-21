#include <libsr2/ui/ui2EventData.h>

namespace sr2 {
    ui2EventData::ui2EventData() {
    }

    ui2EventData::~ui2EventData() {
    }

    const char* ui2EventData::getTypeName() const {
        return "ui2EventData";
    }
    
    bool ui2EventData::isEqualTo(ui2EventData* p1) {
        return false;
    }
};