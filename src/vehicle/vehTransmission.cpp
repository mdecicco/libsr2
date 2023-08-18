#include <libsr2/vehicle/vehTransmission.h>

namespace sr2 {
    vehTransmission::vehTransmission() {

    }

    vehTransmission::~vehTransmission() {

    }

    void vehTransmission::setGearChangeFlag(u32 flag) {

    }

    void vehTransmission::reset() {

    }

    void vehTransmission::update() {

    }
    
    void vehTransmission::computeConstants() {

    }
    
    f32 vehTransmission::getCurrentGearRatio() {
        if (mode == tm_manual) return manual_gear_ratios[current_gear];
        return auto_gear_ratios[current_gear];
    }
};