#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/srui2SRScreen.h>

namespace sr2 {
    class srfeMovieBase : public srui2SRScreen {
        public:
            srfeMovieBase(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeMovieBase();

            static bool isWidgetAMovie(const char* someWidgetName);
    };
};