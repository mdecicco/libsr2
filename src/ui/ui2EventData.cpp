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

    bool ui2EventData::isNotEqualTo(ui2EventData* p1) {
        return !isEqualTo(p1);
    }

    bool ui2EventData::isLessThan(ui2EventData* p1) {
        return false;
    }

    bool ui2EventData::isLessThanOrEqualTo(ui2EventData* p1) {
        if (isLessThan(p1)) return true;
        return isEqualTo(p1);
    }

    bool ui2EventData::isGreaterThan(ui2EventData* p1) {
        return p1->isLessThan(this);
    }

    bool ui2EventData::isGreaterThanOrEqualTo(ui2EventData* p1) {
        if (p1->isLessThan(this)) return true;
        return isEqualTo(p1);
    }
};