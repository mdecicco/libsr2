#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2EventData.h>

namespace sr2 {
    class ASCIIStringEventData : public ui2EventData {
        public:
            ASCIIStringEventData();
            virtual ~ASCIIStringEventData();

            const char* data;
    };
};