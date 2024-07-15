#pragma once
#include <libsr2/types.h>

#include <unordered_map>

namespace sr2 {
    class txtFontTex;
    class Stream;

    struct txtStringData {
        u32 hash;
        txtFontTex* font;
        wchar_t* text;
        vec2f field_0xc;
        u8 field_0x14;
        u8 field_0x15;
        u16 flags;

        bool load(Stream* fp, u16 strFlags);
    };

    class txtStringTable {
        public:
            txtStringTable();
            ~txtStringTable();

            void kill();
            void init();
            bool isInitialized();
            bool load(const char* p1, LANGUAGE lang, u16 strFlags, bool loadKeys);
            void setSomeFlag(u32 flag);
            txtStringData* findString(const char* str);
        
        protected:
            std::unordered_map<u32, txtStringData*> m_stringMap;
            u32 m_someFlag;
            bool m_isInitialized;
            u32 m_keyCount;
            char** m_keys;
    };
};