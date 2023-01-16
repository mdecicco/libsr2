#include <libsr2/vehicle/vehicle.h>

namespace sr2 {
    Vehicle* Vehicle::Vehicles = nullptr;

    Vehicle::Vehicle() {
    }

    Vehicle::~Vehicle() {
        
    }

    Vehicle* Vehicle::GetVehicle(u16 idx) {
        return &Vehicles[idx];
    }
};