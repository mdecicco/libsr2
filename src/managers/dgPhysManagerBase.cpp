#include <libsr2/managers/dgPhysManagerBase.h>
#include <libsr2/managers/phContactManager.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/sim/phContact.h>
#include <libsr2/sim/phImpact.h>
#include <libsr2/sim/SpatialPartitioner.h>
#include <libsr2/sim/PhysEntity.h>
#include <libsr2/sim/phCollision.h>

namespace sr2 {
    dgPhysManagerBase::dgPhysManagerBase() {
        m_contactManager = new phContactManager();
        phContact::disableContacts();

        m_partitioner = nullptr;

        m_tempOversampleAmount = 1;
        m_invSeconds = 1.0f;
        m_seconds = 0.0f;

        m_liveCount = 0;
        m_otherLiveCount = 0;
        m_liveIndices = nullptr;
        m_otherLiveIndices = nullptr;

        m_field4_0x10 = 1;
        m_field5_0x14 = 1.0f;
        m_field8_0x20 = 0;
    }

    dgPhysManagerBase::~dgPhysManagerBase() {
        delete m_contactManager;
    }

    void dgPhysManagerBase::setSpatialPartitioner(SpatialPartitioner* spatialPartitioner) {
        m_partitioner = spatialPartitioner;
        m_field2_0x8 = 1;
        m_liveCount = 0;
        m_otherLiveCount = 0;
        m_liveIndices = new u32[128];
        m_otherLiveIndices = new u32[128];
    }

    void dgPhysManagerBase::method_0x10() {
        phImpact impact;
        m_seconds = datTimeManager::Seconds;

        if (phContact::ContactsEnabled) {
            if (m_field4_0x10 >= 2) {
                m_contactManager->setTiming(m_seconds * m_field5_0x14);
                m_field8_0x20 = m_field4_0x10;
            } else {
                m_contactManager->setTiming(m_seconds);
                m_field8_0x20 = 1;
            }
        }

        u16 someIdx = m_partitioner->getSomeColliderIndexFrom0();
        while (someIdx != 0xffff) {
            phColliderBase* bodyA = m_partitioner->getColliderAt((i32)someIdx);
            phSleep* sleepA = bodyA->getSleep();

            if (sleepA && sleepA->getState() != phSleepState::SLEEP_STATE_ACTIVE) {
                someIdx = m_partitioner->getSomeColliderIndex();
                continue;
            }
        
            vec3f sphereCenter;
            f32 sphereRadius;
            bodyA->getSphereCenterAndRadius(sphereCenter, sphereRadius);
            
            bool foundResults = m_partitioner->searchForNearbyEntities(
                sphereRadius,
                sphereCenter,
                bodyA->isActive(),
                bodyA->getInstance()->unk->otherFlags
            );

            if (!foundResults) {
                someIdx = m_partitioner->getSomeColliderIndex();
                continue;
            }
            
            u32 resultCount;
            
            resultCount = m_partitioner->getSearch0ResultCount();
            for (i32 i = resultCount - 1; i >= 0; i--) {
                phColliderBase* bodyB = m_partitioner->getSearch0ResultCollider(i);
                phSleep* sleepB = bodyB->getSleep();

                if (bodyB >= bodyA && (!sleepB || sleepB->getState() == phSleepState::SLEEP_STATE_ACTIVE)) {
                    continue;
                }

                if (phContact::ContactsEnabled && bodyA->barelyMoved() && bodyB->barelyMoved()) {
                    continue;
                }

                u32 unk;
                if (m_contactManager->testHeldContact(bodyA, bodyB, &unk)) {
                    m_contactManager->calcHeldContacts(bodyA, bodyB, &unk);
                    continue;
                }

                impact.reset(bodyA, bodyB);
                impact.field2_0x8 = (!bodyA->barelyMoved() || !bodyB->barelyMoved()) ? 1 : 0;

                if (!phCollision::testBoundGeneric(&impact)) continue;

                if (phContact::ContactsEnabled && bodyA->barelyMoved() && bodyB->barelyMoved()) {
                    m_contactManager->addHeldContact(impact.colliderA, impact.colliderB);
                }

                method_0x18(&impact);
            }
        
            resultCount = m_partitioner->getSearch1ResultCount();
            for (i32 i = resultCount - 1; i >= 0; i--) {
                phInst* inst = m_partitioner->getSearch1ResultPhys(i);
                impact.reset(bodyA, inst, inst->unk->bound);

                u16 idx = m_partitioner->getSearch1ResultIndex(i);
                if (!phCollision::testBoundGeneric(&impact)) continue;

                phColliderBase* bodyB = m_partitioner->getNewColliderFromUnk2(idx);
                impact.setCollider(bodyB);
            }

            resultCount = m_partitioner->getSearch2ResultCount();
            for (i32 i = resultCount - 1; i >= 0; i--) {
                phInst* inst = m_partitioner->getSearch2ResultPhys(i);
                impact.reset(bodyA, inst, inst->unk->bound);

                if (phCollision::testBoundGeneric(&impact)) {
                    u16 idx = m_partitioner->getSearch2ResultIndex(i);
                    FUN_00268800(&impact, idx, -1);
                }
            }

            impact.reset();
            impact.setColliderA(bodyA);
            resultCount = m_partitioner->getSearch3ResultCount();
            for (i32 i = resultCount - 1; i >= 0 && !impact.hitImpactLimit(); i--) {
                phInst* inst = m_partitioner->getSearch3ResultPhys(i);
                impact.field2_0x8 = !bodyA->barelyMoved();
                impact.setInstanceB(inst, inst->unk->bound);

                bool boundTest = phCollision::testBoundGeneric(&impact);
                
                if (impact.correctBoundOrder()) {
                    impact.swapBoundOrder();
                }

                impact.resetStartIndex();
                if (boundTest && phContact::ContactsEnabled && bodyA->barelyMoved()) {
                    m_contactManager->addHeldContact(impact.colliderA, impact.colliderB);
                }
            }

            if (impact.impactCount > 0) {
                // FUN_00281ec0();
                method_0x18(&impact);
            }
        
            someIdx = m_partitioner->getSomeColliderIndex();
        }

        if (m_liveCount > 0) {
            // FUN_00267f50(&impact);
        }

        if (m_otherLiveCount > 0) {
            // FUN_00268260(&impact);
        }
    }

    void dgPhysManagerBase::method_0x18(phImpact* impact) {
    }

    void dgPhysManagerBase::method_0x20() {
    }

    void dgPhysManagerBase::FUN_00268800(phImpact* impact, u16 p2, u32 p3) {
        if (m_field3_0xc == 0) {
            // FUN_00268dd0(impact, p2, p3);
            method_0x20();
            impact->applyImpetus();
            return;
        }
        
        if (impact->colliderA || impact->colliderB) {
            impact->applyImpetus();
            return;
        }
        
        phInst* inst = m_partitioner->getPhInstFromUnk0(p2);
        vec3f totalImpulse = impact->getTotalImpulse(inst);
        f32 impulseMagSq = totalImpulse.lengthSq();
        if (!FUN_00269280(impulseMagSq, p2)) return;

        FUN_002695d8(impulseMagSq, 1.0f, inst, impact, 0);
    }

    bool dgPhysManagerBase::FUN_00269280(f32 squaredImpulseMag, u16 p2) {
        // todo
        return false;
    }

    void dgPhysManagerBase::FUN_002695d8(f32 squaredImpulseMag, f32 p2, phInst* inst, phImpact* impact, undefined4 p4) {
        // todo
    }
};