#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Stream;
    class datBaseTokenizer;
    class datCallback;

    enum PARSE_TYPE {
        STRING = 0,
        BOOLEAN,
        INT8,
        INT16,
        INT32,
        FLOAT,
        VEC2F,
        VEC3F,
        VEC4F,
        STRUCTURE,
        unk5
    };

    struct datParserNode {
        PARSE_TYPE tp;
        u16 element_count;
        u16 string_len;
        char name[64];
        void* destination;
        void* callback;
        datParserNode* next;
    };

    class datParser {
        public:
            datParser(const char* ext);
            ~datParser();

            datParser* add_parser(const char* nodeName, datCallback* cb);
            datParserNode* add(PARSE_TYPE tp, const char* name, void* dest, u16 element_count, datCallback* cb);

            i32 read_inner(datBaseTokenizer* tokenizer);
            void read(datBaseTokenizer* tokenizer);
            void load(Stream* file, const char* filename);
            bool load(const char* dir, const char* filename, const char* ext);
            bool save(const char* dir, const char* filename, const char* ext);

        protected:
            void fulfill(datParserNode* n, void* src, size_t sz);

            char m_last_token[64];
            datParserNode* m_targets;
            u32 m_count;
            u32 field_0x50;
    };
};