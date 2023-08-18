#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehCarSimBase;
    class vehEngine;
    class vehTransmission;
    class vehWheel;
    
    class vehDrivetrain : public parFileIO {
        public:
            vehDrivetrain();
            ~vehDrivetrain();

            void init(vehCarSimBase* veh, vehTransmission* trans, vehEngine* eng);
            void reset();
            void update();
            void attach();
            void detach();
            bool addWheel(vehWheel* wheel);

            vehCarSimBase* vehicle;
            vehEngine* engine;
            vehTransmission* transmission;
            vehWheel* wheels[6];
            u32 wheel_count;
            f32 angular_inertia;
            f32 brake_damping_coefficient;
            f32 brake_static_coefficient;
            f32 some_rps;
            f32 field7_0x38;

            static f32 diffRatioMax;
            static f32 diffRatioMaxHighSpeed;
            static f32 diffRatioHighSpeedLevel;
            static f32 dRHSLinV;
    };
};