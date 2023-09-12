#include <libsr2/utilities/datRefCount.h>

namespace sr2 {
    datRefCount::datRefCount() {
        count = 1;
    }

    datRefCount::~datRefCount() {
    }

    u32 datRefCount::incRef() {
        return ++count;
    }

    u32 datRefCount::decRef() {
        u32 result = --count;
        if (count == 0) {
            // I know, I know. I didn't choose to do it this way...
            delete this;
        }

        return result;
    }

    u32 datRefCount::refCount() const {
        return count;
    }
};