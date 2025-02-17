#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2EventData.h>

namespace sr2 {
    class ASCIIStringEventData : public ui2EventData {
        public:
            ASCIIStringEventData();
            virtual ~ASCIIStringEventData();

            virtual const char* getTypeName() const;
            virtual bool isEqualTo(ui2EventData* p1);
            virtual bool isLessThan(ui2EventData* p1);

            const char* data;
    };
};