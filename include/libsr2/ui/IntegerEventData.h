#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2EventData.h>

namespace sr2 {
    class IntegerEventData : public ui2EventData {
        public:
            IntegerEventData();
            virtual ~IntegerEventData();

            i32 data;
    };
};