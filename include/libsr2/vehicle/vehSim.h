#pragma once
#include <libsr2/io/parFileIO.h>
#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/phInst.h>

namespace sr2 {
    class vehAero;
    class vehFluid;
    class vehEngine;
    class vehTransmission;
    class vehWheel;
    class vehDrivetrain;
    class vehAxle;
    class vehSuspension;

    class vehSim : public parFileIO, public phColliderBase, public phInst {
        public:
            vehSim();
            virtual ~vehSim();

            // parFileIO
            virtual const char* file_type() const;

            // phColliderBase
            virtual void reset();
            virtual void update();
            virtual void move();

            // phInst
            virtual void method_0x30() = 0;

            // own
            virtual void method_0x48();
            virtual void method_0x50();
            virtual void checkWheelIntersections();
            virtual void method_0x60();
            virtual void method_0x68();
            virtual i32 method_0x78();

            void setResetPos(const vec3f& pos);
            void setEulers(const vec3f& eulers);
            void updateModelOffset();
            void setTransform(const mat3x4f& tform);

            vehAero* aero;
            vehFluid* fluid;
            vehEngine* engine;
            vehTransmission* transmission;
            u32 wheel_count;
            vehWheel* wheelArray;
            u32 drivetrain_count;
            vehDrivetrain* drivetrainArray;
            u32 axle_count;
            vehAxle* axleArray;
            u32 suspension_item_count;
            vehSuspension* suspensionItemArray;
            vec3f model_offset;
            vec3f center_of_mass;
            f32 some_horsepower_factor;
            vec3f initial_pos;
            vec3f eulers;
            f32 field24_0x144;
            f32 field25_0x148;
            f32 field27_0x150;
            f32 velocity_mag;
            vec3f field21_0x120;
    };
};