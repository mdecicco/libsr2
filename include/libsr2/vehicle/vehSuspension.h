#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehSuspension : public parFileIO {
        public:
            vehSuspension();
            ~vehSuspension();
            
            virtual void reset();
            virtual void update();
            virtual void init();

            mat3x4f world_transform;
    };
};