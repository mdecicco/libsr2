#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeUIIntroMovie : public srfeMovieBase {
        public:
            srfeUIIntroMovie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeUIIntroMovie();

            virtual void initScreen();
    };
};