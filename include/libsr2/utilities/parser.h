#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Data;

    class Parser {
        public:
            enum PARSE_TYPE {
                STRING = 0,
                unk1,
                unk2,
                unk3,
                INT,
                FLOAT,
                VEC2F,
                VEC3F,
                VEC4F
            };

            struct node {
                PARSE_TYPE tp;
                s16 unk0;
                u16 unk1;
                char name[64];
                void* destination;
                i32 unk2;
                node* next;
            };

            Parser();
            ~Parser();

            node* add(PARSE_TYPE tp, const char* name, void* dest, i32 unk0, i32 unk1);

            virtual bool deserialize(Data* src) = 0;
            virtual bool serialize(Data* dst) = 0;

        protected:
            void fulfill(node* n, void* src, size_t sz);
            node* m_targets;
            u32 m_count;
    };
};