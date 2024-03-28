#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class modModel;
    class crSkeletonData;
    class ragUnk1;
    class shdwInstance;
    class PhysEntity;
    class srCreatureGroup;
    class crAnimation;
    class dgRagdoll;

    class srType {
        public:
        srType();
        ~srType();

        void load(srCreatureGroup* group);
        void initData();
        void FUN_001ad528(const char* name);
        dgRagdoll* FUN_001ae018();

        undefined4 field0_0x0;
        f32 field1_0x4;
        f32 field2_0x8;
        f32 field3_0xc;
        f32 field4_0x10;
        vec3f field5_0x14;
        f32 field6_0x20;
        i32 field9_0x2c;
        f32 maxForce;
        u32 animationCount;
        crAnimation** animations;
        modModel* lods[3];
        crSkeletonData* skel;
        PhysEntity* entity;
        ragUnk1* lastEle;
        shdwInstance* shadow;
    };
};