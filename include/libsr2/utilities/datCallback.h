#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class datCallback {
        public:
            datCallback(void* func, void* self);
            ~datCallback();

            template <typename ...Args>
            void call(Args... args) {
            }

        private:
            void* m_func;
            void* m_self;
    };
};