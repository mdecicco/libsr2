#include <libsr2/creature/ragJoint.h>
#include <libsr2/sim/dgLinkData.h>

#include <libsr2/math/vec3f.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/quat.h>

namespace sr2 {
    ragJoint::ragJoint() {
        unk0 = 1.0f;
        linkIdx = -1;
        parent = nullptr;
        flex = 0.0f;
        damping = 0.0f;
        boneIdx = -1;
    }

    ragJoint::~ragJoint() {
    }

    void ragJoint::reset(dgLinkData* data) {
        data->center.position = { 0.0f, 0.0f, 0.0f };
        data->velocity = { 0.0f, 0.0f, 0.0f };
        data->unk2.orient = data->rotated.orient;
        data->unk2.position = data->rotated.position;

        math::rotate(data->unk0->orient, data->init.position, data->rotated.position);
        math::add(data->rotated.position, data->unk0->position);
        math::mult(data->rotated.orient, data->unk0->orient, data->init.orient);
        data->unk2.orient = data->rotated.orient;
        data->unk2.position = data->rotated.position;
        data->unk5 = false;
    }

    void ragJoint::getTransform(mat3x4f& out) {
        out.w = position;
        if (!parent) return;

        mat3x4f t;
        parent->getTransform(t);

        math::mult_with_translation(out, t);
    }

    void ragJoint::method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4) {
        if (!parent) return;
        parent->method_0x50(links, param_2, param_3, param_4);
    }

    void ragJoint::method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4) {
        if (!parent) return;
        parent->method_0x58(links, param_2, param_3, param_4);
    }

    void ragJoint::method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2) {
        if (!parent) return;
        parent->method_0x20(links, param_1, param_2);
    }

    void ragJoint::method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2) {
        if (!parent) return;
        parent->method_0x28(links, param_1, param_2);
    }
    
    void ragJoint::FUN_00284238(f32 length, f32 inertia) {
        unk0 = 1.0f / (length * (inertia + 1.0f));
    }

    void ragJoint::resetSomething(dgLinkData* data) {
        math::zero(data->center.position);
        math::zero(data->velocity);
    }
};