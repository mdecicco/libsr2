#include <libsr2/vehicle/vehicle.h>

namespace sr2 {
    Vehicle* Vehicle::Vehicles = nullptr;
    u32 Vehicle::Count = 0;

    Vehicle::Vehicle() {
    }

    Vehicle::~Vehicle() {
        
    }

    Vehicle* Vehicle::GetVehicle(u16 idx) {
        return &Vehicle::Vehicles[idx];
    }

    u32 Vehicle::GetCount() {
        return Vehicle::Count;
    }
};