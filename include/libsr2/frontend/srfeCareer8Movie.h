#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer8Movie : public srfeMovieBase {
        public:
            srfeCareer8Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer8Movie();

            virtual void initScreen();
    };
};