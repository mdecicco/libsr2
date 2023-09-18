#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class srActivePool;
    class srCreatureGroup;
    class srType;

    class srCreatureMgr {
        struct crmType {
            char name[32];
            srType* type;
        };

        public:
            srCreatureMgr();
            ~srCreatureMgr();

            void reset();
            void update();
            srType* getType(srCreatureGroup* group);
            void FUN_001ac6b0();
            void FUN_001ac750();
            bool load(const char* levelName, const char* missionName);

            u32 groupCount;
            srCreatureGroup* groups;
            srActivePool* pool;
            crmType types[10];
    };
};