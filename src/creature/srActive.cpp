#include <libsr2/creature/srActive.h>
#include <libsr2/creature/srMover.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/creature/crAnimBlender.h>
#include <libsr2/creature/crAnimPlayer.h>
#include <libsr2/creature/crAnimation.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/creature/srCreatureGroup.h>
#include <libsr2/creature/srType.h>
#include <libsr2/creature/ragdoll.h>
#include <libsr2/vehicle/vehicle.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/sim/dgRagdoll.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/audio/audImpact.h>
#include <libsr2/math/math.h>

namespace sr2 {
    srActive::srActive() {
        group = nullptr;
        ragdoll = nullptr;

        field0_0x0 = 0;
        field1_0x4 = 0;
        field5_0x14 = 0xffffffff;
        rollingAverageIdx = 0;

        transform = mat3x4f::identity;
        repulsion.zero();
        force.zero();

        mover = new srMover();
        skeleton = new crSkeleton(100);
        blender = new crAnimBlender(skeleton, 3, false, 1);
        ragdollFrames = 0;

        for (u32 i = 0; i < 10; i++)
            prevRepulsion[i].zero();

        deactivate();
    }

    srActive::~srActive() {
        if (mover) {
            delete mover;
            mover = nullptr;
        }

        if (skeleton) {
            delete skeleton;
            skeleton = nullptr;
        }

        if (blender) {
            delete blender;
            blender = nullptr;
        }
    }

    void srActive::reset() {
        deactivate();
        mover->reset();
        blender->reset();

        field1_0x4 = 0;
    }

    void srActive::deactivate() {
        if (ragdollFrames > 0)
            stopRagdoll();
        transform = mat3x4f::identity;
        force.zero();
        repulsion.zero();
        field5_0x14 = 0;
        field0_0x0 = 0;
        group = nullptr;
    }

    void srActive::activate(srCreatureGroup* _group, i32 idx) {
        group = _group;

        vec3f positionOffset = {
            math::frand() * (2.0f * group->spawnRadius) - group->spawnRadius,
            0.0f,
            math::frand() * (2.0f * group->spawnRadius) - group->spawnRadius };

        transform.w = group->currentPosition + positionOffset;

        mover->init(group->type, transform);
        skeleton->init(group->type->skel, &transform);
        blender->init(skeleton->data);

        for (u32 i = 0; i < group->type->animationCount; i++) {
            blender->players[i].playAnimation(group->type->animations[i]);
            blender->players[i].doLoop = true;
            if (i > 0)
                blender->players[i].field10_0x28 = 0.0f;
        }

        if (idx != -1) {
            field5_0x14 = idx % group->type->field9_0x2c;
        }

        field0_0x0 = 1;
    }

    void srActive::setAnimation() {
        srType* type = group->type;

        f32 fMag = force.length();
        if (fMag < type->field6_0x20 * 0.25f) {
            blender->players[1].field10_0x28 = 0.0f;
            blender->players[2].field10_0x28 = 0.0f;
            field1_0x4 = 0;
            return;
        }

        if (fMag >= (type->field6_0x20 + type->maxForce) * 0.5f) {
            blender->players[1].field10_0x28 = 0.0f;
            blender->players[2].field10_0x28 = 0.0f;
            blender->players[2].percentPerSec = fMag / type->maxForce;

            if (!field1_0x4) {
                FUN_001aef88();
                blender->players[2].time = blender->players[1].time;
            }

            field1_0x4 = 1;
            return;
        }

        blender->players[1].field10_0x28 = 1.0f;
        blender->players[2].field10_0x28 = 0.0f;
        blender->players[1].percentPerSec = fMag / type->field6_0x20;
        if (field1_0x4) {
            blender->players[1].time = blender->players[2].time;
        }

        field1_0x4 = 0;
    }

    void srActive::calcGroupCenterForce() {
        vec3f dp = group->currentPosition - transform.w;

        if (group->pointCount == 2 && group->currentPointIdx == 1) {
            vec3f t0 = (group->points[1] - group->points[0]).normalized();

            if (dp.lengthSq() > 25.0f || t0.dot(-transform.z) < 0.0f) {
                group->currentPointIdx = 0;
            }
        }

        f32 rx = fabsf(dp.x) - group->spawnRadius;
        f32 rz = fabsf(dp.z) - group->spawnRadius;

        if (rx > 0.0f) {
            if (dp.x > 0.0f)
                repulsion.x += group->type->field2_0x8 * 100.0f * rx;
            else
                repulsion.x -= group->type->field2_0x8 * 100.0f * rx;
        }

        if (rz > 0.0f) {
            if (dp.z > 0.0f)
                repulsion.z += group->type->field2_0x8 * 100.0f * rz;
            else
                repulsion.z -= group->type->field2_0x8 * 100.0f * rz;
        }
    }

    void srActive::calcPlayerRepulsion() {
        for (u32 i = 0; i < Vehicle::GetCount(); i++) {
            Vehicle* v = Vehicle::GetVehicle(i);
            phInst* inst = nullptr; // veh->instance->base.base.car.sim->veh.sim.col.instance; (lol)

            vec3f dp = inst->transform.w - transform.w;
            f32 r = (group->type->field3_0xc * 100.0f) / dp.length();
            repulsion.x -= dp.x * r;
            repulsion.z -= dp.z * r;
        }
    }

    void srActive::resolveForces() {
        smoothRepulsion();

        force = repulsion * (1.0f / group->type->field4_0x10) * datTimeManager::Seconds;

        f32 forceMag = force.length();
        f32 maxForce = group->type->maxForce * 1.25f;
        if (forceMag > maxForce)
            force *= maxForce / forceMag;
    }

    void srActive::smoothRepulsion() {
        prevRepulsion[rollingAverageIdx] = repulsion;
        repulsion.zero();

        for (u32 i = 0; i < 10; i++)
            repulsion += prevRepulsion[i];
        repulsion *= 0.1f;

        rollingAverageIdx++;
        if (rollingAverageIdx >= 10)
            rollingAverageIdx = 0;
    }

    void srActive::stopRagdoll() {
        ragdollFrames = 0;
        ragdoll->reset();
        skeleton->transform = &transform;
        skeleton->bones[0].transform.w -= ragdoll->unk1->offset;
        mover->field1_0x4 = mat3x4f::identity;
        mover->field1_0x4.w = skeleton->boneTransforms[0].w;
        mover->setElevationFromPosition(mover->field1_0x4.w);
    }

    void srActive::FUN_001aef08(audImpact* aud, f32 u0) {
        aud->play(u0, 500.0f, 0x11);

        void* unk = FUN_001af288();
        if (unk) {
            // unk->FUN_00324128(transform.w);
            // unk->FUN_0031e938(50.0f);
            // unk->FUN_0031ea00(u0);
        }
    }

    void srActive::FUN_001aef88() {
        void* unk = FUN_001af288();
        if (unk) {
            // unk->FUN_0031e938(50.0f);
            // unk->FUN_0031ea58();
        }
    }

    bool srActive::onImpact(const mat3x4f& u0) {
        audImpact* impactAudio = nullptr;

        f32 nearestDistSq = 10000.0f;
        for (u32 i = 0; i < Vehicle::GetCount(); i++) {
            Vehicle* veh = Vehicle::GetVehicle(i);
            phInst* inst = nullptr; // veh->instance->base.base.car.sim->veh.sim.col.instance (lol)
            f32 distSq = (transform.w - inst->transform.w).lengthSq();

            if (distSq >= nearestDistSq) continue;
            nearestDistSq = distSq;

            phInertialCS* ics = nullptr; // veh->instance->base.base.car.sim->veh.sim.col.ics (lol)
            worldVelocity = ics->world_velocity;

            if (false /* Managers::Aud3DObjectManager::IsAlive() */) {
                impactAudio = nullptr; // veh->instance->base.base.car.audio->getAudImpactPtr();
            }
        }

        if (impactAudio) FUN_001aef08(impactAudio, worldVelocity.lengthSq());
        ragdoll = group->type->FUN_001ae018();

        if (ragdoll) {
            ragdoll->startRagdoll(skeleton, u0);
            ragdollFrames = 1;
            return true;
        }

        return false;
    }
};