#include <libsr2/instances/Allocator.h>

namespace sr2 {
    InstanceAllocator* InstanceAllocator::instance = nullptr;
    InstanceAllocator::InstanceAllocator() {
    }

    InstanceAllocator::~InstanceAllocator() {
    }

    InstanceAllocator* InstanceAllocator::Get() {
        return instance;
    }

    void InstanceAllocator::Create() {
        if (instance) return;
        instance = new InstanceAllocator();
    }

    void InstanceAllocator::Destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }
};