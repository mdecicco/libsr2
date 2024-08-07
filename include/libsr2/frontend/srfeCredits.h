#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>

namespace sr2 {
    class srfeCredits : public srfeBasicLayout2 {
        public:
            srfeCredits(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCredits();

            void FUN_0011c260();
            void FUN_0011c270();
        
        protected:
            undefined4 field_0xd0;
            undefined4 field_0xcc;
    };
};