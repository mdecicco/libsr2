#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer6Movie : public srfeMovieBase {
        public:
            srfeCareer6Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer6Movie();

            virtual void initScreen();
    };
};