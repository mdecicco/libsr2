#pragma once
#include <libsr2/vehicle/vehCarSimBase.h>

namespace sr2 {
    class vehCarSim : public vehCarSimBase {
        public:
            vehCarSim();
            ~vehCarSim();

            f32 hill_horsepower_factor;
            f32 car_friction_handling;
            f32 sss_value;
            f32 sss_threshold;
    };
};