#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer7Movie : public srfeMovieBase {
        public:
            srfeCareer7Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer7Movie();

            virtual void initScreen();
    };
};