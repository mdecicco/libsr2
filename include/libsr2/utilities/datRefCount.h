#pragma once
#include <libsr2/types.h>

namespace sr2 {
    /**
     * @brief THIS CLASS DELETES ITSELF WHEN THE REF COUNT REACHES 0!
     * THEREFORE, YOU *MUST* ALLOCATE DERIVATIVES OF THIS CALSS WITH 
     * THE NEW OPERATOR!
     * 
     * @todo
     * FIX THIS ONCE ALL DERIVATIVES ARE DEFINED
     */
    class datRefCount {
        public:
            datRefCount();
            virtual ~datRefCount();

            u32 incRef();
            u32 decRef();
            u32 refCount() const;

            u32 count;
    };
};