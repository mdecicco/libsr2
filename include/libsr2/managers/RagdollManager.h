#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class ragUnk1;
    class dgRagdoll;
    class srType;
    class crSkeletonData;

    class RagdollManager {
        public:
            RagdollManager();
            ~RagdollManager();

            void init(u32 count);
            void initRagdolls(u32 count);
            void load(srType* type, const char* name);
            ragUnk1* getOrLoad(crSkeletonData* skel, const char* name);
            dgRagdoll* maybeGetFreeRagdoll(u32 idx);
            dgRagdoll* maybeGetFreeRagdoll(ragUnk1* unk);

            ragUnk1* elements;
            dgRagdoll* ragdolls;
            u32 elementCount;
            u32 capacity;
            u32 ragdollCount;

            static RagdollManager* get();
            static void destroy();
        
        private:
            static RagdollManager* instance;
    };
};