#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehDriver : public parFileIO {
        public:
            vehDriver();
            ~vehDriver();
    };
};