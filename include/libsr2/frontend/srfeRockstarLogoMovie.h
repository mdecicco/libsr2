#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeRockstarLogoMovie : public srfeMovieBase {
        public:
            srfeRockstarLogoMovie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeRockstarLogoMovie();
    };
};