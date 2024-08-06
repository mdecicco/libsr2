#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer3Movie : public srfeMovieBase {
        public:
            srfeCareer3Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer3Movie();

            virtual void initScreen();
    };
};