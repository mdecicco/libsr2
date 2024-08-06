#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class audBase {
        public:
            audBase();
            virtual ~audBase();
        
        protected:
            audBase* m_last;
            audBase* m_next;
    };
};