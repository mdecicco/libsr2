#include <libsr2/ambient/ambientEntity.h>
#include <libsr2/ambient/aiAmbientVehicleAudio.h>
#include <libsr2/ambient/ambientData.h>
#include <libsr2/utilities/datCallback.h>
#include <libsr2/sim/SpatialPartitioner.h>
#include <libsr2/sim/phImpact.h>
#include <libsr2/sim/phBound.h>
#include <libsr2/math/math.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/audio/audImpact.h>
#include <libsr2/instances/ambientInstance.h>
#include <libsr2/managers/Aud3DObjectManager.h>
#include <libsr2/managers/missManager.h>
#include <libsr2/managers/ambientManager.h>
#include <libsr2/mission/Mission.h>
#include <libsr2/terrain/lvlLevel.h>
#include <libsr2/terrain/orTerrainGrid.h>

namespace sr2 {
    ambientEntity::ambientEntity() {
        phColliderBase::reset();

        initFromCtor(nullptr, &m_ics, &m_sleep);
        m_sleep.init(&m_ics, this);

        phSleep::MinVelMagSq = 0.1f;
        phSleep::MinAngVelMagSq = 0.05f;
        phSleep::DAT_00363780 = 0x3c;

        m_impactCb = new datCallback(ImpactCB, this);
        m_spatialPartitionerIdx = SpatialPartitioner::get()->insertCollider(this);
        m_ambInst = nullptr;
        reset();
    }

    ambientEntity::~ambientEntity() {
        delete m_impactCb;
        m_impactCb = nullptr;
    }

    void ambientEntity::reset() {
        m_ambInst = nullptr;
        m_currentDamage = 0.0f;
        m_sleep.reset();
        SpatialPartitioner::get()->maybeReinsertAny(m_spatialPartitionerIdx);
    }

    void ambientEntity::update() {
        phColliderBase::update();

        ambientData* data = m_ambInst->getData();
        
        mat3x4f wt = m_ics.world_transform;
        wt.w -= wt.basis() * data->field_0x18;

        m_ambInst->setTransform(wt);
    }

    void ambientEntity::move() {
        if (ambientManager::get()->antiGravityEnabled) {
            phInertialCS* ics = getICS();
            f32 gravMult = getBound()->gravityMultiplier;
            f32 mass = ics->mass;
            ics->force.y += -gravMult * -9.8f * mass;
        }

        for (i32 i = 3;i >= 0;i--) {
            m_wheels[i].FUN_001818a0();
        }

        orTerrainGrid* terrain = lvlLevel::get()->getTerrain();
        SpatialPartitioner* sp = SpatialPartitioner::get();

        for (u32 i = 0;i < 4;i++) {
            if (m_wheels[i].terrain && m_wheels[i].terrain != terrain) {
                phInst* inst = sp->getPhInst(m_wheels[i].spatialPartitionerIdx);
                if (!inst) continue;

                SpatialPartitionerElementType type = sp->getElementType(m_wheels[i].spatialPartitionerIdx);
                if (type == SpatialPartitionerElementType::UNK3_ARR) continue;

                // TODO: finish this
            }
        }

        m_ambInst->FUN_0017de08(false);
    }

    void ambientEntity::initFromCtor(phBound* bnd, phInertialCS* ics, phSleep* sleep) {
    }

    void ambientEntity::ImpactCB(ambientEntity* self, phColliderImpactCallbackData* data) {
        if (!data) return;

        phImpact* impact = data->impact;
        phColliderBase* collider0 = impact->colliderA;
        phColliderBase* collider1 = impact->colliderB;
        ambientData* aData = nullptr;

        if (collider0 == self) {
            collider0 = impact->colliderB;
            collider1 = impact->colliderA;
        }

        undefined2 unk2 = collider1->getUnk2();
        if (unk2 > 1000) unk2 = 0;

        vec3f unk = math::abs(*data->field0_0x0);
        f32 unkSum = unk.x + unk.y + unk.z;
        self->m_currentDamage += unkSum;

        if (self->m_ambInst) {
            aData = self->m_ambInst->getData();
            if (self->m_currentDamage > aData->ptxThreshold) {
                self->m_ambInst->field_0xa4 = FLT_EPSILON;

                if (self->m_currentDamage > aData->maxDamage) {
                    self->m_ambInst->field_0xb0 = 2;
                }
            }

            audImpact* impactPlayer = nullptr;
            aiAmbientVehicleAudio* impactSound = nullptr;
            if (Aud3DObjectManager::isAlive()) {
                impactSound = self->m_ambInst->getImpactSound();

                if (impactSound && impactSound->field_0x20 != -1) {
                    impactPlayer = impactSound->getPlayer();
                }
            }

            if (impactSound && impactPlayer) {
                impactPlayer->play(unkSum, 500.0f, unk2);
                impactSound->FUN_0024afb0(unkSum);
            }
        }

        if (collider0->getICS() && self->m_sleep.isActive()) {
            self->m_sleep.reset();
        }

        if (!aData) return;

        undefined2 sVar1 = collider0->getUnk2();
        if (sVar1 >= 1000) {
            missManager::get()->getCurrentMission()->onImpact(
                self->m_currentDamage,
                aData->contraThreshold,
                // ? Decompilation says sVar1 + 0x18, but disassembly says (sVar1 - 1000) & 0xff
                (sVar1 - 1000) & 0xff,
                self->m_ambInst->getTransform()
            );
        }
    }
};