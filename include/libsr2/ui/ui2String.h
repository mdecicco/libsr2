#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class ui2String {
        public:
            ui2String(u32 length = 0);
            ui2String(const char* text, bool isConst);
            ui2String(const ui2String& other, u32 length);
            ~ui2String();

            void setConst(const char* text);
            void set(const char* text);

            const char* get() const;
        
        protected:
            char* m_text;
            u32 m_length;
            bool m_ownsPtr;
    };
};