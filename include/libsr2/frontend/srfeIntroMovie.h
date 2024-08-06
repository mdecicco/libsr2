#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeIntroMovie : public srfeMovieBase {
        public:
            srfeIntroMovie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeIntroMovie();

            virtual void initScreen();
    };
};