#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer9Movie : public srfeMovieBase {
        public:
            srfeCareer9Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer9Movie();

            virtual void initScreen();
    };
};