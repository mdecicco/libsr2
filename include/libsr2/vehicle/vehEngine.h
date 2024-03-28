#pragma once
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class vehCarSimBase;
    class vehDrivetrain;
    class vehTransmission;
    class phInertialCS;
    class phInst;

    class vehEngine : public parFileIO {
        public:
            vehEngine();
            ~vehEngine();

            virtual void reset();
            void init(vehCarSimBase* veh, const char* modelname, const char* partname);
            f32 getMaxHorsepower();
            void computeConstants();
            f32 calcTorqueAtFullThrottle(f32 rpm_fac);
            f32 calcTorqueAtZeroThrottle();
            void startBoosting(f32 duration = 0.0f);
            void stopBoosting();
            void update();

            virtual datParserNode* prepParser(datParser* parser);
            virtual const char* getFileType();

            vehDrivetrain* drivetrains;
            vehTransmission* transmission;
            phInertialCS* ics;
            phInst* instance;
            mat3x4f* pivot_transform;
            mat3x4f* instance_transform;
            f32 torque_at_opt_rps;
            f32 unk_computed_1;
            f32 idle_rpm; // rotations/min
            f32 idle_rps; // radians/sec
            f32 opt_rpm; // rotations/min
            f32 opt_rps; // radians/sec
            f32 max_rpm; // rotations/min
            f32 max_rps; // radians/sec
            f32 rpm; // rotations/min
            f32 rps; // radians/sec
            f32 gear_change_duration; // seconds
            f32 gear_change_time; // seconds
            f32 boost_duration;
            f32 boost_remaining;
            f32 boost_horsepower;
            f32 max_horsepower;
            f32 horsepower_scale;
            f32 angular_inertia;
            f32 torque;
            f32 jiggle_angle;
            f32 unk_rpm_0_0;
            f32 inv_opt_rps_diff_sq;
            f32 torque_lerp_factor;
            bool changing_gears;

            f32 fib;
            f32 one_minus_fib;
            f32 field7_0x24;
            f32 field26_0x6c;
            u16 field21_0x5a;
    };
};