#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer1Movie : public srfeMovieBase {
        public:
            srfeCareer1Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer1Movie();
    };
};