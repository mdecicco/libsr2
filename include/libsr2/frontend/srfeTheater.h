#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen3.h>

namespace sr2 {
    class srfeTheater : public srfeUnknownScreen3 {
        public:
            srfeTheater(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeTheater();
    };
};