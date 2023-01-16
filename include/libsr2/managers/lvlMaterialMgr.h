#pragma once
#include <libsr2/types.h>
#include <libsr2/managers/phMaterialMgr.h>

namespace sr2 {
    class lvlMaterialMgr : public phMaterialMgr {
        public:
            static lvlMaterialMgr* create(u32 materialCapacity);
            static lvlMaterialMgr* get();

        private:
            lvlMaterialMgr(u32 materialCapacity);
            ~lvlMaterialMgr();
    };
};