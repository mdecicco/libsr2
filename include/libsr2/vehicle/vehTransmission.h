#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehEngine;
    class vehDrivetrain;

    enum TRANSMISSION_MODE {
        tm_manual = 0,
        tm_automatic = 1
    };

    class vehTransmission : public parFileIO {
        public:
            vehTransmission();
            ~vehTransmission();

            void setGearChangeFlag(u32 flag);
            virtual void reset();
            virtual void update();
            void computeConstants();
            f32 getCurrentGearRatio();

            vehEngine* engine;
            vehDrivetrain* drivetrains;
            TRANSMISSION_MODE mode;
            u32 current_gear;
            f32 gear_change_time;
            f32 manual_gear_ratios[8];
            f32 auto_gear_ratios[8];
            u32 manual_gear_count;
            u32 auto_gear_count;
            f32 upshift_gear_rpms[8];
            f32 downshift_gear_rpms[8];
            f32 max_speed_reverse; // m/s
            f32 max_speed_low; // m/s
            f32 max_speed_high; // m/s
            f32 upshift_bias;
            f32 min_downshift_bias;
            f32 max_downshift_bias;
            f32 gear_bias;
            i32 field5_0x14;
            undefined4 clutch;
    };
};