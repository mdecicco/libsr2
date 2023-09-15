#include <libsr2/managers/RagdollManager.h>
#include <libsr2/creature/srType.h>
#include <libsr2/creature/ragdoll.h>
#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/sim/dgRagdoll.h>

#include <string.h>

namespace sr2 {
    RagdollManager* RagdollManager::instance = nullptr;

    RagdollManager::RagdollManager() {
        elements = nullptr;
        ragdolls = nullptr;
        elementCount = 0;
        capacity = 0;
        ragdollCount = 0;
    }

    RagdollManager::~RagdollManager() {
        if (elements) {
            delete [] elements;
            elements = nullptr;
        }

        if (ragdolls) {
            delete [] ragdolls;
            ragdolls = nullptr;
        }
    }

    void RagdollManager::init(u32 count) {
        capacity = count;
        elementCount = 0;
        elements = new ragUnk1[count];
    }
    
    void RagdollManager::initRagdolls(u32 count) {
        ragdollCount = count;
        ragdolls = new dgRagdoll[count];

        u32 maxBoundCount = 0;
        u32 maxLinkCount = 0;
        for (u32 i = 0;i < elementCount;i++) {
            phBoundComposite* bound = elements[i].getBound();
            if (bound->boundCount > maxBoundCount) maxBoundCount = bound->boundCount;
            if (elements[i].unk1.count > maxLinkCount) maxLinkCount = elements[i].unk1.count;
        }

        for (u32 i = 0;i < count;i++) {
            ragdolls[i].init(maxBoundCount, maxLinkCount);
        }
    }

    void RagdollManager::load(srType* type, const char* name) {
        if (elements[elementCount].load(type->skel, name)) {
            type->lastEle = &elements[elementCount];
            elementCount++;
            return;
        }

        type->lastEle = nullptr;
    }
    
    ragUnk1* RagdollManager::getOrLoad(crSkeletonData* skel, const char* name) {
        for (u32 i = 0;i < elementCount;i++) {
            if (strcmp(name, elements[i].filename) == 0) {
                return &elements[i];
            }
        }

        if (elements[elementCount].load(skel, name)) {
            return &elements[elementCount++];
        }

        return nullptr;
    }
    
    dgRagdoll* RagdollManager::maybeGetFreeRagdoll(u32 idx) {
        return maybeGetFreeRagdoll(&elements[idx]);
    }
    
    dgRagdoll* RagdollManager::maybeGetFreeRagdoll(ragUnk1* unk) {
        for (u32 i = 0;i < ragdollCount;i++) {
            if (!ragdolls[i].field28_0x190) {
                ragdolls[i].setBoundsFromUnk1(unk);
                return &ragdolls[i];
            }
        }

        return nullptr;
    }

    RagdollManager* RagdollManager::get() {
        if (!instance) instance = new RagdollManager();
        return instance;
    }

    void RagdollManager::destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }
};