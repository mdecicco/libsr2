#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeAngelLogoMovie : public srfeMovieBase {
        public:
            srfeAngelLogoMovie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeAngelLogoMovie();
    };
};