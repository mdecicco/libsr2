#include <libsr2/vehicle/vehCarSim.h>

namespace sr2 {
    vehCarSim::vehCarSim() {
        sss_value = 0.0f;
        sss_threshold = 0.0f;
        hill_horsepower_factor = 0.0f;
        car_friction_handling = 0.0f;
    }

    vehCarSim::~vehCarSim() {

    }
};