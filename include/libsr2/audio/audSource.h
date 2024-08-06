#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/audBase.h>

namespace sr2 {
    class audSource : public audBase {
        public:
            audSource(const char* name, i32 maxAudible, i32 priority, i32 index);
            ~audSource();

        protected:
            const char* m_name;
            i32 m_maxAudible;
            i32 m_priority;
            i32 m_index;
    };
};