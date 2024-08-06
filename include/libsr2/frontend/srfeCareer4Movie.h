#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer4Movie : public srfeMovieBase {
        public:
            srfeCareer4Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer4Movie();

            virtual void initScreen();
    };
};