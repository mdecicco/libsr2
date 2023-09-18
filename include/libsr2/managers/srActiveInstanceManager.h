#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class srCreatureMgr;
    class srActiveInstance;

    class srActiveInstanceMgr {
        public:
            static srActiveInstanceMgr* get();
            static void destroy();

            void init(const char* levelName, const char* missionName);
            void reset();
            void update();

            u32 instanceCount;
            srCreatureMgr* creatureMgr;
            srActiveInstance* instances;
        
        protected:
            srActiveInstanceMgr();
            ~srActiveInstanceMgr();

            static srActiveInstanceMgr* instance;
    };
};