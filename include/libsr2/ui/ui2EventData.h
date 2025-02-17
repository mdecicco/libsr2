#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2WidgetBase.h>

namespace sr2 {
    class ui2EventData : public ui2WidgetBase {
        public:
            ui2EventData();
            virtual ~ui2EventData();

            virtual const char* getTypeName() const;
            virtual bool isEqualTo(ui2EventData* p1);
            virtual bool isNotEqualTo(ui2EventData* p1);
            virtual bool isLessThan(ui2EventData* p1);
            virtual bool isLessThanOrEqualTo(ui2EventData* p1);
            virtual bool isGreaterThan(ui2EventData* p1);
            virtual bool isGreaterThanOrEqualTo(ui2EventData* p1);
    };
};