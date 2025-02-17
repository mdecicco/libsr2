#include <libsr2/ui/ASCIIStringEventData.h>
#include <string.h>

namespace sr2 {
    ASCIIStringEventData::ASCIIStringEventData() {
        data = "";
    }

    ASCIIStringEventData::~ASCIIStringEventData() {
    }

    const char* ASCIIStringEventData::getTypeName() const {
        return "ASCIIStringEventData";
    }

    bool ASCIIStringEventData::isEqualTo(ui2EventData* p1) {
        if (getTypeName() != p1->getTypeName()) return false;
        return strcmp(data, ((ASCIIStringEventData*)p1)->data) == 0;
    }

    bool ASCIIStringEventData::isLessThan(ui2EventData* p1) {
        if (getTypeName() != p1->getTypeName()) return false;
        return strcmp(data, ((ASCIIStringEventData*)p1)->data) < 0;
    }
};