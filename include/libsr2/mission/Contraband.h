#pragma once
#include <libsr2/types.h>
#include <libsr2/instances/unknown.h>

namespace sr2 {
    class TaggedStream;
    class ContrabandInfo {
        public:
            ContrabandInfo();
            ~ContrabandInfo();

            bool load(TaggedStream* tsv);
            void SomeGeomFunc();

            Cls_00394e58 unk0;
            Cls_00394e58 unk1;
            char name[32];
            char unk2[32];
            char geom_file_0[32];
            char geom_file_1[32];
            i32 cash_value;
            bool is_bomb;
            undefined4 unk4;
            undefined4 unk5;
            undefined2 time_to_detonate;
            undefined2 unk7;
    };
};