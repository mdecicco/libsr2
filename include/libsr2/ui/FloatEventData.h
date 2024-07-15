#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2EventData.h>

namespace sr2 {
    class FloatEventData : public ui2EventData {
        public:
            FloatEventData();
            virtual ~FloatEventData();

            f32 data;
    };
};