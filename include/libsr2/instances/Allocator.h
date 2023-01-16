#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class InstanceAllocator {
        public:
            template <typename T>
            static T* Alloc();
            static InstanceAllocator* Get();
            static void Create();
            static void Destroy();

        private:
            static InstanceAllocator* instance;
            InstanceAllocator();
            ~InstanceAllocator();
    };
};