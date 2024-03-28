#pragma once
#include <libsr2/types.h>

#include <unordered_map>

namespace sr2 {
    class txtFontTex;
    class Stream;

    class txtStringTable {
        public:
            struct txtStringData {
                i32 hash;
                txtFontTex* font;
                wchar_t* text;
                vec2f field_0xc;
                u8 field_0x14;
                u8 field_0x15;
                u16 flags;

                bool load(Stream* fp, u16 strFlags);
            };

            txtStringTable();
            ~txtStringTable();

            void kill();
            void init();
            bool isInitialized();
            bool load(const char* p1, LANGUAGE lang, u16 strFlags, bool loadKeys);
        
        protected:
            std::unordered_map<size_t, txtStringData*> m_stringMap;
            bool m_isInitialized;
            u32 m_keyCount;
            char** m_keys;
    };
};