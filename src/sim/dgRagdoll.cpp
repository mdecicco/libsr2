#include <libsr2/sim/dgRagdoll.h>
#include <libsr2/sim/dgLinkData.h>
#include <libsr2/sim/PhysEntity.h>
#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/sim/SpatialPartitioner.h>
#include <libsr2/creature/ragdoll.h>
#include <libsr2/creature/ragJoint.h>
#include <libsr2/math/quat.h>
#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    dgRagdollData::dgRagdollData() {
        unk0 = nullptr;
        links = nullptr;
    }

    dgRagdollData::~dgRagdollData() {
        if (links) delete[] links;
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
    void startRagdoll(crSkeleton* skeleton, const mat3x4f& transform);



    dgRagdoll::dgRagdoll() {
        col.read_matrix = nullptr;
        col.unk0 = 0;
        col.last = col.next = nullptr;
        col.active = true;
        col.reset();

        sleep.init(nullptr, nullptr);
        field28_0x190 = 0;
        bones0 = nullptr;
        bones1 = nullptr;
        data = nullptr;
    }

    dgRagdoll::~dgRagdoll() {
    }

    void dgRagdoll::reset() {
        FUN_001ab2d8();
    }

    void dgRagdoll::init(u32 boneCount, u32 linkCount) {
        entity = new PhysEntity();
        entity->bound = new phBoundComposite(boneCount);
        bones0 = new mat3x4f[boneCount];
        bones1 = new mat3x4f[boneCount];
        instance0.unk = entity;
        ((phBoundComposite*)entity->bound)->field6_0x6c = bones0;
        ((phBoundComposite*)entity->bound)->field7_0x70 = bones1;
        data = new dgRagdollData();
        data->init(linkCount);
        sleep.init(data, &col);
        math::identity(instance0.transform);
        col.init(&instance0, data, &sleep);
        spIndex = SpatialPartitioner::get()->insertCollider(&col);
        SpatialPartitioner::get()->maybeReinsertAny(spIndex);
    }

    void dgRagdoll::setBoundsFromUnk1(ragUnk1* unk) {
    }

    void dgRagdoll::FUN_001ab2d8() {
        SpatialPartitioner::get()->maybeReinsertAny(spIndex);
        field28_0x190 = 0;
    }

    void dgRagdoll::startRagdoll(crSkeleton* skeleton, const mat3x4f& transform) {
    }
};