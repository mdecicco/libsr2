#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/UnkWidgetBinTree0.h>

namespace sr2 {
    class ui2WidgetBase {
        public:
            ui2WidgetBase();
            virtual ~ui2WidgetBase();

            virtual void addRef();
            virtual void baseReleaseRef();

            u32 getRefCount() const;

        protected:
            u32 m_refCount;
            undefined* field_0xc;
    };
};