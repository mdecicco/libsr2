#include <libsr2/sim/dgRagdoll.h>
#include <libsr2/sim/dgLinkData.h>
#include <libsr2/creature/ragdoll.h>
#include <libsr2/math/quat.h>
#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    dgRagdollData::dgRagdollData() {
        unk0 = nullptr;
        links = nullptr;
    }

    dgRagdollData::~dgRagdollData() {
        if (links) delete [] links;
        links = nullptr;
    }

    void dgRagdollData::resetTransform() {

    }

    void dgRagdollData::zero() {

    }

    void dgRagdollData::freeze() {

    }

    void dgRagdollData::updateInner(f32 dt) {
        phInertialCS::updateInner(dt);
        updateTransforms();
    }

    void dgRagdollData::method_0x40() {

    }

    void dgRagdollData::calcNetPush(const vec3f& unk) {

    }

    void dgRagdollData::moveICS() {

    }

    void dgRagdollData::getLocalVelocity(vec3f& some_position, vec3f& velocity) {

    }

    void dgRagdollData::method_0x68() {

    }

    void dgRagdollData::init(u32 linkCount) {
        unk1 = 0;
        unk2 = 0;
        worldTrans.orient = {
            { 0.0f, 0.0f, 0.0f },
            1.0f
        };
        worldTrans.position = {
            0.0f, 0.0f, 0.0f
        };
        prevWorldTrans = worldTrans;

        links = new dgLinkData[linkCount];
    }
    
    void dgRagdollData::FUN_00282e08() {
        if (!unk0) return;

        for (u32 i = 0;i < unk0->count;i++) {
            auto* l = unk0->links[i];

            if (l->parent) {
                links[i].unk0 = &links[l->parent->linkIdx].rotated;
                links[i].unk1 = &links[l->parent->linkIdx].unk2;
                continue;
            }

            links[i].unk0 = &worldTrans;
            links[i].unk1 = &prevWorldTrans;
        }
    }

    void dgRagdollData::updateTransforms() {
        prevWorldTrans = worldTrans;
        math::from_matrix(worldTrans.orient, world_transform);
        worldTrans.position = world_transform.w;
    }
    
    void dgRagdollData::FUN_00282f80(f32 dt, mat3x4f* bones) {
        f32 grav = 0.0f;

        if (!unk1) grav = unk0->unk0 * -9.8f * dt;

        for (u32 i = 0;i < unk0->count;i++) {
            vec3f vel;
            getLocalVelocity(links[i].rotated.position, vel);

            vec3f boneUp;
            math::mult(boneUp, world_transform, bones[unk0->links[i]->boneIdx].y);

            unk0->links[i]->method_0x18(dt, grav, links, vel, links[i].center.position, boneUp);
        }
    }
};