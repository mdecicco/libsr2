#include <libsr2/creature/srActivePool.h>
#include <libsr2/creature/srActive.h>
#include <libsr2/creature/srCreatureGroup.h>
#include <libsr2/creature/srType.h>

namespace sr2 {
    srActivePool::srActivePool() {
        count = 10;
        pool = new srActive[count];
    }
    
    srActivePool::~srActivePool() {
        if (pool) {
            delete [] pool;
            pool = nullptr;
        }
    }

    void srActivePool::reset() {
        for (u32 i = 0;i < count;i++) pool[i].reset();
    }

    void srActivePool::update() {
        calculateGroupRepulsion();
        for (u32 i = 0;i < count;i++) {
            if (!pool[i].field0_0x0) continue;
            if (!pool[i].ragdollFrames) {
                pool[i].calcGroupCenterForce();
                pool[i].calcPlayerRepulsion();
            }

            pool[i].FUN_001af1d8();
        }

        for (u32 i = 0;i < count;i++) {
            pool[i].resolveForces();
            pool[i].updateAnim();
            pool[i].repulsion = { 0.0f, 0.0f, 0.0f };
        }
    }
    
    void srActivePool::calculateGroupRepulsion() {
        for (u32 i = 0;i < count;i++) {
            if (pool[i].ragdollFrames > 0) continue;
            
            for (u32 j = i + 1;j < count;j++) {
                vec3f dp = pool[j].transform.w - pool[i].transform.w;
                f32 xzMagSq = dp.x * dp.x + dp.z * dp.z;
                f32 unk = (
                    pool[i].group->type->field1_0x4 * 1000.0f +
                    pool[j].group->type->field1_0x4 * 1000.0f
                ) / xzMagSq * xzMagSq;

                pool[i].repulsion.x -= dp.x * unk;
                pool[i].repulsion.z -= dp.z * unk;
                pool[j].repulsion.x += dp.x * unk;
                pool[j].repulsion.z += dp.z * unk;
            }
        }
    }
    
    void srActivePool::addGroup(srCreatureGroup* group) {
        if (group->unk0 == 0) return;

        u32 startIdx = 0;
        u32 addedCount = 0;
        for (u32 g = 0;g < group->unk0;g++) {
            for (u32 i = startIdx;i < count;i++) {
                if (pool[i].field0_0x0) continue;

                startIdx = i + 1;

                if (group->type->field9_0x2c < 1) pool[i].activate(group, -1);
                else pool[i].activate(group, addedCount++);

                break;
            }
        }
    }
    
    void srActivePool::removeGroup(srCreatureGroup* group) {
        for (u32 i = 0;i < count;i++) {
            if (pool[i].group == group) {
                pool[i].deactivate();
            }
        }
    }
};