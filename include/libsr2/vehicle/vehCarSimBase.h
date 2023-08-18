#pragma once
#include <libsr2/vehicle/vehSim.h>

namespace sr2 {
    class vehDrivetrain;
    class vehWheel;
    class vehAxle;
    class vehSuspension;

    class vehCarSimBase : public vehSim {
        public:
            vehCarSimBase();
            ~vehCarSimBase();
        
            // parFileIO
            virtual const char* directory() const;
            virtual const char* file_type() const;
            virtual datParserNode* prep_parser(datParser* parser);

            // phColliderBase
            virtual void reset();
            virtual void update();
            virtual void move();

            // vehSim
            virtual void method_0x48();
            virtual void method_0x50();
            virtual void checkWheelIntersections();
            virtual i32 getGroundedWheelCount();

            // own
            virtual void initPhysics(char* vehicleName, phInst* inst);
            virtual void initAero();
            virtual void initFluid();
            virtual void initEngine(char* vehicleName);
            virtual void initTransmission();
            virtual void initWheels(char* vehicleName);
            virtual void initDrivetrains();
            virtual void initAxles(char* vehicleName, f32 unk);
            virtual void initSuspension(char* vehicleName);

            i32 getBottomedOutWheelCount();
            bool FUN_0025a6e0();
            bool load(const char* vehicleName);
            void FUN_0025a8c8();
            void init(char* vehicleName, phInst* inst, f32 unk);


            u32 spatial_partitioner_idx;
            f32 mass;
            vec3f inertia_box_copied;
            vec3f inertia_box;
            f32 bound_friction;
            f32 bound_elasticity;
            f32 bound_gravity;
            f32 air_gravity;
            i32 drivetrain_type;
            bool should_brake;

            vehDrivetrain* freetrain;
            vehWheel* wheels[6];
            vehAxle* axles[2];
            vehSuspension* shafts[2];
            vehSuspension* arms[4];
            vehSuspension* shocks[4];

            undefined4 field2_0x174;
    };
};