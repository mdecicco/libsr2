#include <libsr2/vehicle/vehCarSimBase.h>
#include <libsr2/vehicle/vehAero.h>
#include <libsr2/vehicle/vehFluid.h>
#include <libsr2/vehicle/vehEngine.h>
#include <libsr2/vehicle/vehTransmission.h>
#include <libsr2/vehicle/vehDrivetrain.h>
#include <libsr2/vehicle/vehWheel.h>
#include <libsr2/vehicle/vehAxle.h>
#include <libsr2/vehicle/vehSuspension.h>
#include <libsr2/vehicle/vehBound.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phSegment.h>
#include <libsr2/sim/phIntersectionPoint.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/managers/VehicleMgr.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/io/datParser.h>

namespace sr2 {
    vehCarSimBase::vehCarSimBase() {
        bound_elasticity = 0.5f;
        bound_friction = 0.75f;
        bound_gravity = 2.0f;
        air_gravity = 0.0f;
        mass = 2000.0f;
        drivetrain_type = 0;
        inertia_box = { 2.0f, 1.0f, 3.0f };
        inertia_box_copied = { 2.0f, 1.0f, 3.0f };
        spatial_partitioner_idx = 0xffff;
        field2_0x174 = 0x1f;

        freetrain = nullptr;
        for (u8 i = 0;i < 6;i++) wheels[i] = nullptr;
        
        for (u8 i = 0;i < 2;i++) {
            axles[i] = nullptr;
            shafts[i] = nullptr;
        }
        
        for (u8 i = 0;i < 4;i++) {
            arms[i] = nullptr;
            shocks[i] = nullptr;
        }
    }
    
    vehCarSimBase::~vehCarSimBase() {
    }
        
    const char* vehCarSimBase::directory() const {
        return "tune/vehicle";
    }

    const char* vehCarSimBase::file_type() const {
        return "vehCarSim";
    }

    datParserNode* vehCarSimBase::prep_parser(datParser* parser) {
        parser->add(PARSE_TYPE::FLOAT, "Mass", &mass, 1, nullptr);    
        parser->add(PARSE_TYPE::VEC3F, "InertiaBox", &inertia_box, 1, nullptr);
        parser->add(PARSE_TYPE::VEC3F, "CenterOfMass", &center_of_mass, 1, nullptr);
        parser->add(PARSE_TYPE::VEC3F, "ModelOffset", &model_offset, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "BoundFriction", &bound_friction, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "BoundElasticity", &bound_elasticity, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "BoundGravity", &bound_gravity, 1, nullptr);
        parser->add(PARSE_TYPE::FLOAT, "AirGravity", &air_gravity, 1, nullptr);
        parser->add(PARSE_TYPE::INT32, "DrivetrainType", &drivetrain_type, 1, nullptr);
        aero->prep_parser(parser->add_parser("Aero", nullptr));
        fluid->prep_parser(parser->add_parser("Fluid", nullptr));
        engine->prep_parser(parser->add_parser("Engine", nullptr));
        transmission->prep_parser(parser->add_parser("Trans", nullptr));
        drivetrainArray->prep_parser(parser->add_parser("Drivetrain", nullptr));
        if (freetrain) freetrain->prep_parser(parser->add_parser("Freetrain", nullptr));
        wheels[0]->prep_parser(parser->add_parser("WheelFront", nullptr));
        wheels[2]->prep_parser(parser->add_parser("WheelBack", nullptr));
        if (axles[0]) axles[0]->prep_parser(parser->add_parser("AxleFront", nullptr));
        if (axles[1]) axles[1]->prep_parser(parser->add_parser("AxleBack", nullptr));
    }

    void vehCarSimBase::reset() {
        vehSim::reset();
        aero->reset();
        fluid->reset();
        engine->reset();
        transmission->reset();
        for (u32 i = 0;i < drivetrain_count;i++) drivetrainArray[i].reset();
        for (u32 i = 0;i < wheel_count;i++) wheels[i]->reset();
        for (u32 i = 0;i < axle_count;i++) axles[i]->reset();
        for (u32 i = 0;i < suspension_item_count;i++) suspensionItemArray[i].reset();

        if (ics) {
            ics->ResetTransform();
            ics->world_transform.w = initial_pos;
            math::from_eulers_zxy(ics->world_transform, eulers);
            updateModelOffset();
        }

        /*
         * Probably unnecessary:

        if (Physics::SpatialPartitioner::Instance) {
            Physics::SpatialPartitioner::Free(spatial_partitioner_idx);
        }

        */
    }

    void vehCarSimBase::update() {
        vehSim::update();
    }

    void vehCarSimBase::move() {
        vehSim::move();

        void* vm = nullptr;
        // vm = Managers::VehicleMgr::Get();
        if (!vm) {
            update();
            engine->update();
            transmission->update();
            aero->update();
            fluid->update();

            for (u32 i = 0;i < wheel_count;i++) wheels[i]->update();
            for (u32 i = 0;i < wheel_count;i++) {
                // cast rays from each wheel to the terrain, doing who knows what with the result
            }

            drivetrainArray[0].update();
            if (freetrain) freetrain->update();

            for (u32 i = 0;i < wheel_count;i++) wheels[i]->update();
            for (u32 i = 0;i < drivetrain_count;i++) drivetrainArray[i].update();
            for (u32 i = 0;i < axle_count;i++) axles[i]->update();
            for (u32 i = 0;i < suspension_item_count;i++) suspensionItemArray[i].update();
            
            //
            // if (DAT_00363660 != 0) {
            //     self_vtb = (vehCarSim_1_vtb *)pfio.vtb;
            //     (*(code *)self_vtb->unk_0025b420)((int)&pfio.vtb + (int)self_vtb->field45_0x50);
            // }
            //
        }
    }
    
    void vehCarSimBase::method_0x48() {
        f32 unk0 = field27_0x150 > 0.0f ? field27_0x150 : 0.0f;
        f32 unk1 = field27_0x150 < 0.0f ? -field27_0x150 : 0.0f;
        f32 unk2 = field24_0x144 > 0.0f ? unk0 * (1.0f - field24_0x144) : unk0;
        f32 unk3 = field24_0x144 < 0.0f ? field24_0x144 * (unk0 * 2.0f) : unk0;

        wheels[0]->setInputs(field24_0x144, some_horsepower_factor, unk1);
        if (wheels[1]) wheels[1]->setInputs(field24_0x144, some_horsepower_factor, unk1);
        

        wheels[2]->setInputs(-field24_0x144, some_horsepower_factor, unk2);

        if (wheels[3]) wheels[3]->setInputs(-field24_0x144, some_horsepower_factor, unk3);
        if (wheels[4]) wheels[4]->setInputs(-field24_0x144, some_horsepower_factor, unk3);
        if (wheels[5]) wheels[5]->setInputs(-field24_0x144, some_horsepower_factor, unk3);

        transmission->clutch = 0;

        if (field25_0x148 <= 0.01f && velocity_mag >= 1.0f) should_brake = true;
        else if (0.01f < field25_0x148 || velocity_mag > 2.0f) should_brake = false;
        
        if (should_brake) {
            wheels[0]->setBrake(1.0f);
            wheels[2]->setBrake(1.0f);

            if (wheels[1]) wheels[1]->setBrake(1.0f);
            if (wheels[3]) wheels[3]->setBrake(1.0f);
            if (wheels[4]) wheels[4]->setBrake(1.0f);
            if (wheels[5]) wheels[5]->setBrake(1.0f);
        }

        if (field25_0x148 > 0.95f && some_horsepower_factor > 0.95f && (transmission->current_gear == 2 || transmission->current_gear == 0)) {
            if (drivetrainArray->wheel_count == wheel_count) {
                transmission->clutch = 1;
                transmission->setGearChangeFlag(1);
            } else {
                if (freetrain) {
                    for (u32 i = 0;i < freetrain->wheel_count;i++) {
                        freetrain->wheels[i]->setBrake(1.0f);
                    }
                }

                for (u32 i = 0;i < drivetrainArray->wheel_count;i++) {
                    drivetrainArray->wheels[i]->setBrake(0.0f);
                }
            }
        }

        engine->torque_lerp_factor = field25_0x148;
        if (engine->boost_remaining > 0.0f && (some_horsepower_factor > 0.0f || field27_0x150 > 0.0f || field25_0x148 <= 0.0f)) {
            engine->stopBoosting();
        }
    }

    void vehCarSimBase::method_0x50() {
        i32 count = getBottomedOutWheelCount();
        if (count == 0) return;

        math::add(instance->transform.w, ics->push);
        if (engine->instance_transform) math::add(engine->instance_transform->w, ics->push);

        for (u32 i = 0;i < wheel_count;i++) {
            math::add(wheels[i]->world_transform.w, ics->push);
        }

        for (u32 i = 0;i < axle_count;i++) {
            math::add(axles[i]->world_transform.w, ics->push);
        }

        for (u32 i = 0;i < suspension_item_count;i++) {
            math::add(suspensionItemArray[i].world_transform.w, ics->push);
        }
    }
    
    void vehCarSimBase::checkWheelIntersections() {
        // if (!Physics::SpatialPartitioner::Instance) return;

        phSegment* segments[6] = { nullptr };
        phIntersectionPoint* intersections[6] = { nullptr };

        for (u32 i = 0;i < wheel_count;i++) {
            segments[i] = &wheels[i]->seg;
            intersections[i] = &wheels[i]->ground_contact_pt;
        }

        // Physics::SpatialPartitioner::Instance->TestSegments(wheel_count, segments, intersections, instance, field2_0x174);

        for (u32 i = 0;i < wheel_count;i++) {
            wheels[i]->has_intersection = intersections[i]->instance != nullptr;
        }
    }
    
    i32 vehCarSimBase::getGroundedWheelCount() {
        i32 count = 0;
        for (u32 i = 0;i < wheel_count;i++) {
            if (wheels[i]->is_on_ground) count++;
        }

        return count;
    }
    
    i32 vehCarSimBase::getBottomedOutWheelCount() {
        i32 count = 0;
        for (u32 i = 0;i < wheel_count;i++) {
            if (wheels[i]->is_bottomed_out) count++;
        }

        return count;
    }
    
    void vehCarSimBase::initAero() {
        aero = new vehAero();
        aero->init(this);
    }
    
    void vehCarSimBase::initFluid() {
        fluid = new vehFluid();
        fluid->init(this, bound->aabb_min, bound->aabb_max);
    }
    
    void vehCarSimBase::initEngine(char* vehicleName) {
        engine = new vehEngine();
        engine->init(this, vehicleName, "engine");
    }
    
    void vehCarSimBase::initTransmission() {
        transmission = new vehTransmission();
        // transmission->init();
    }
    
    void vehCarSimBase::initWheels(char* vehicleName) {
        i32 count = GetPivotCount(vehicleName, "whl", 6);
        wheelArray = new vehWheel[count];

        for (u32 i = 0;i < 6;i++) {
            std::string partName = format("whl%d", i);
            if (IsPivot(vehicleName, partName.data())) {
                wheels[i] = &wheelArray[i];
                // wheels[i]->init(this, vehicleName, partName.c_str(), ics);
                // LoadBounds(some_hashtable, 0.3, format("%s_whl%d", vehicleName, i));
                wheel_count++;
            }
        }
    }
    
    void vehCarSimBase::initDrivetrains() {
        drivetrainArray = new vehDrivetrain[2];
        drivetrainArray[0].init(this, transmission, engine);
        drivetrainArray[1].init(this, transmission, engine);
    }
    
    void vehCarSimBase::initAxles(char* vehicleName, f32 unk) {
        axleArray = new vehAxle[2];
        axles[0] = &axleArray[0];
        axles[1] = &axleArray[1];
        axles[0]->init(this, vehicleName, "axle0", wheels[0], wheels[1], unk);
        axles[0]->init(this, vehicleName, "axle1", wheels[2], wheels[3], unk);
    }
    
    void vehCarSimBase::initSuspension(char* vehicleName) {
        i32 shockCount = GetPivotCount(vehicleName, "shock", 6);
        i32 armCount = GetPivotCount(vehicleName, "arm", 6);
        i32 shaftCount = GetPivotCount(vehicleName, "shaft", 6);
        i32 totalCount = shockCount + armCount + shaftCount;

        suspensionItemArray = new vehSuspension[totalCount];

        char buf[64];
        for (u32 i = 0;i < 4;i++) {
            buf[0] = 0;
            if (i < 4) {
                snprintf(buf, 64, "shock%d", i);
                if (IsPivot(vehicleName, buf)) {
                    shocks[i] = &suspensionItemArray[suspension_item_count++];
                    shocks[i]->init();
                }
            }
        }

        for (u32 i = 0;i < 4;i++) {
            buf[0] = 0;
            if (i < 4) {
                snprintf(buf, 64, "arm%d", i);
                if (IsPivot(vehicleName, buf)) {
                    arms[i] = &suspensionItemArray[suspension_item_count++];
                    arms[i]->init();
                }
            }
        }

        // shaft2, shaft3 (don't ask why, noone ask why)
        for (u32 i = 2;i < 4;i++) {
            buf[0] = 0;
            if (i < 4) {
                snprintf(buf, 64, "shaft%d", i);
                if (IsPivot(vehicleName, buf)) {
                    shafts[i] = &suspensionItemArray[suspension_item_count++];
                    shafts[i]->init();
                }
            }
        }
    }

    void vehCarSimBase::initPhysics(char* vehicleName, phInst* inst) {
        ics = new phInertialCS();
        ics->max_ang_velocity = { 12.56637f, 12.56637f, 12.56637f };
        inertia_box_copied = inertia_box;

        ics->initBoxMass(mass, inertia_box.x, inertia_box.y, inertia_box.z);

        inertia_box_copied = inertia_box;

        vehBound* bnd = new vehBound();
        bool loadedBound = false;
        // loadedBound = bnd->load(format("%s_bound", vehicleName).c_str(), 0);
        if (loadedBound) {
            bnd->init();
            bound = bnd;
        }

        // some hash map nonsense...

        vehSim::init(bnd, inst, ics);
        math::identity(instance->transform);

        // some spatial partitioner nonsense...
    }
    
    bool vehCarSimBase::FUN_0025a6e0() {
        bool result = true;

        if (engine) {
            engine->transmission = transmission;
            engine->drivetrains = drivetrainArray;
        }

        if (transmission) {
            transmission->engine = engine;
            transmission->drivetrains = drivetrainArray;
        }

        drivetrainArray->wheel_count = 0;
        freetrain->wheel_count = 0;

        switch (drivetrain_type) {
            case 0: {
                freetrain->addWheel(wheels[0]);
                if (wheels[1]) freetrain->addWheel(wheels[1]);

                result = drivetrainArray->addWheel(wheels[2]);

                if (wheels[3]) result = drivetrainArray->addWheel(wheels[3]);
                if (wheels[4]) result = drivetrainArray->addWheel(wheels[4]);
                if (wheels[5]) result = drivetrainArray->addWheel(wheels[5]);

                return result;
            }
            case 1: {
                freetrain->addWheel(wheels[2]);
                freetrain->addWheel(wheels[3]);
                drivetrainArray->addWheel(wheels[0]);
                result = drivetrainArray->addWheel(wheels[1]);

                return result;
            }
            case 2: {
                drivetrainArray->addWheel(wheels[0]);
                result = drivetrainArray->addWheel(wheels[1]);

                if (wheels[2]) result = drivetrainArray->addWheel(wheels[2]);
                if (wheels[3]) result = drivetrainArray->addWheel(wheels[3]);
                if (wheels[4]) result = drivetrainArray->addWheel(wheels[4]);
                if (wheels[5]) result = drivetrainArray->addWheel(wheels[5]);

                return result;
            }
        }

        return true;
    }
    
    bool vehCarSimBase::load(const char* vehicleName) {
        set_name(vehicleName);
        m_flags |= 1;
        return parFileIO::load();
    }

    void vehCarSimBase::FUN_0025a8c8() {
        if (wheels[1]) wheels[1]->copyFrom(wheels[0]);
        if (wheels[3]) wheels[3]->copyFrom(wheels[2]);
        if (wheels[4]) wheels[3]->copyFrom(wheels[2]);
        if (wheels[5]) wheels[3]->copyFrom(wheels[2]);

        FUN_0025a6e0();
        ics->initBoxMass(mass, inertia_box.x, inertia_box.y, inertia_box.z);
        ((vehBound*)bound)->setFriction(bound_friction);
        ((vehBound*)bound)->setElasticity(bound_elasticity);
        bound->gravity_multiplier = bound_gravity;
        engine->computeConstants();
        transmission->computeConstants();

        if (axles[0]) axles[0]->computeConstants();
        if (axles[1]) axles[1]->computeConstants();

        wheels[0]->computeConstants();
        wheels[2]->computeConstants();
        if (wheels[1]) wheels[1]->computeConstants();
        if (wheels[3]) wheels[3]->computeConstants();
        
        if (wheels[4]) {
            wheels[4]->computeConstants();
            f32 newLoad = (wheels[4]->normal_load + wheels[2]->normal_load) * 0.5f;
            wheels[4]->setNormalLoad(newLoad);
            wheels[2]->setNormalLoad(newLoad);
        }
        
        if (wheels[5]) {
            wheels[5]->computeConstants();
            f32 newLoad = (wheels[5]->normal_load + wheels[3]->normal_load) * 0.5f;
            wheels[5]->setNormalLoad(newLoad);
            wheels[3]->setNormalLoad(newLoad);
        }
    }
    
    void vehCarSimBase::init(char* vehicleName, phInst* inst, f32 unk) {
        initPhysics(vehicleName, inst);
        initAero();
        initFluid();
        initTransmission();
        initEngine(vehicleName);
        initWheels(vehicleName);
        initDrivetrains();
        initAxles(vehicleName, unk);
        initSuspension(vehicleName);
        load(vehicleName);
        setResetPos({ 0.0f, 0.0f, 0.0f });
        reset();
    }
};