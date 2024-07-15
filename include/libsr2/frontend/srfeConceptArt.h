#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/srui2SRScreen.h>

namespace sr2 {
    class srfeConceptArt : public srui2SRScreen {
        public:
            srfeConceptArt(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeConceptArt();
    };
};