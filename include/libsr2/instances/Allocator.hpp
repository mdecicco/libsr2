#pragma once
#include <libsr2/instances/Allocator.h>

namespace sr2 {
    template <typename T>
    T* InstanceAllocator::Alloc() {
        return new T();
    }
}