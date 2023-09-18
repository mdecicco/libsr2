#pragma once
#include <libsr2/types.h>

namespace sr2 {
    struct some_vehicle_thing;

    class VehicleCameraData;
    class UnkMenu;
    class gfxViewport;

    class Vehicle {
        public:
            Vehicle();
            ~Vehicle();

            i32 index;
            some_vehicle_thing* instance;
            undefined* unk0;
            PLAYER_TYPE type;
            VehicleCameraData* cam_data;
            UnkMenu* some_menu;
            i32 unk1, unk2;
            gfxViewport* viewport;
            f32 mass;
            f32 max_horsepower;

            static Vehicle* GetVehicle(u16 idx);
            static u32 GetCount();
            static u32 Count;
            static Vehicle* Vehicles;
    };
};