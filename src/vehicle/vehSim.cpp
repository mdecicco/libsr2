#include <libsr2/vehicle/vehSim.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>

namespace sr2 {
    vehSim::vehSim() {
        read_matrix = nullptr;
        unk0 = 0;
        last = next = nullptr;
        active = true;
        phColliderBase::reset();

        model_offset = { 0.0f, -0.75f, 0.0f };
        center_of_mass = { 0.0f, 0.2f, 0.0f };
        ics = nullptr;
        bound = nullptr;
        field24_0x144 = 0.0f;
        field25_0x148 = 0.0f;
        some_horsepower_factor = 0.0f;
        field27_0x150 = 0.0f;
        velocity_mag = 0.0f;
        aero = nullptr;
        fluid = nullptr;
        engine = nullptr;
        transmission = nullptr;
        wheel_count = 0;
        wheelArray = nullptr;
        drivetrain_count = 0;
        drivetrainArray = nullptr;
        axle_count = 0;
        axleArray = nullptr;
        suspension_item_count = 0;
        suspensionItemArray = nullptr;
        eulers = { 0.0f, 0.0f, 0.0f };
        initial_pos = { 0.0f, 0.0f, 0.0f };
        field21_0x120 = { 0.0f, 0.0f, 0.0f };
    }

    vehSim::~vehSim() {
    }
    
    void vehSim::setResetPos(const vec3f& pos) {
        math::sub(initial_pos, pos, model_offset);
    }

    void vehSim::setEulers(const vec3f& _eulers) {
        eulers = _eulers;
    }
    
    void vehSim::updateModelOffset() {
        if (!ics) return;

        math::copy(transform, ics->world_transform);

        vec3f modelOffsetWorldSpace;
        math::mult(modelOffsetWorldSpace, ics->world_transform, model_offset);
        math::add(transform.w, modelOffsetWorldSpace);
    }
    
    void vehSim::setTransform(const mat3x4f& tform) {
        if (!ics) return;

        math::copy(transform, tform);

        vec3f modelOffsetWorldSpace;
        math::mult(modelOffsetWorldSpace, ics->world_transform, model_offset);
        math::sub(transform.w, modelOffsetWorldSpace);
    }

    void vehSim::method_0x48() { }

    void vehSim::method_0x50() {
        math::add(transform.w, ics->push);
    }

    void vehSim::checkWheelIntersections() { }

    void vehSim::method_0x60() { }

    void vehSim::method_0x68() { }

    i32 vehSim::method_0x78() {
        return 1;
    }
    
    void vehSim::reset() {
        field21_0x120 = { 0.0f, 0.0f, 0.0f };
        field24_0x144 = 0.0f;
        field25_0x148 = 0.0f;
        some_horsepower_factor = 0.0f;
        field27_0x150 = 0.0f;
        velocity_mag = 0.0f;

        if (ics) ics->ResetTransform();
        updateModelOffset();

        // Not sure why this is there...
        // math::copy(transform, transform);

        phColliderBase::reset();
    }
    
    void vehSim::update() {
        phColliderBase::update();
        updateModelOffset();

        velocity_mag = math::magnitude(ics->world_velocity);
    }
    
    void vehSim::move() {
        phColliderBase::move();
        updateModelOffset();

        if (ics->pushed_last_frame) math::add(field21_0x120, ics->last_push);
        math::mult(field21_0x120, 0.8f);
    }

    const char* vehSim::file_type() const {
        return "vehSim";
    }
};