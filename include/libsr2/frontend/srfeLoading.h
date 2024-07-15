#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/srui2SRScreen.h>

namespace sr2 {
    class srfeLoading : public srui2SRScreen {
        public:
            srfeLoading(const char* name, const WidgetRef<ui2Master>& master);
            virtual ~srfeLoading();

            void FUN_00135560();
    };
};