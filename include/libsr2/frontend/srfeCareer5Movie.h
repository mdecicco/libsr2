#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeMovieBase.h>

namespace sr2 {
    class srfeCareer5Movie : public srfeMovieBase {
        public:
            srfeCareer5Movie(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCareer5Movie();
    };
};