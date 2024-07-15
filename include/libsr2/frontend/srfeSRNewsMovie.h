#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeSRNewsMovie : public srfeMovieBase {
        public:
            srfeSRNewsMovie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeSRNewsMovie();
    };
};