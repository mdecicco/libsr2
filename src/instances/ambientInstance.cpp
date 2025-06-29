#include <libsr2/instances/ambientInstance.h>
#include <libsr2/ambient/ambientAgent.h>

namespace sr2 {
    ambientInstance::ambientInstance() {
    }

    ambientInstance::~ambientInstance() {
    }

    mat3x4f ambientInstance::getTransform() const {
        return transform;
    }

    void ambientInstance::setTransform(const mat3x4f& _transform) {
        m_agent->maybeInitialTransform = _transform;
        transform = _transform;
    }

    ambientData* ambientInstance::getData() const {
        return nullptr;
    }

    aiAmbientVehicleAudio* ambientInstance::getImpactSound() const {
        return m_impactSound;
    }

    void ambientInstance::FUN_0017de08(bool param_1) {
        field_0xac = param_1;
        flags.unk6 = !param_1;
    }
};