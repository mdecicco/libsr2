#pragma once
#include <libsr2/types.h>
#include <libsr2/io/datBaseTokenizer.h>

namespace sr2 {
    class datAsciiTokenizer : public datBaseTokenizer {
        public:
            virtual i32 read_int32();
            virtual f32 read_float();
            virtual i32 read_delimiter(char* token);
            virtual i32 match_int(char* token);
            virtual f32 match_float(char* token);
            virtual vec2f match_vec2(char* token);
            virtual vec3f match_vec3(char* token);
            virtual vec4f match_vec4(char* token);
            virtual bool write_string(const char* str, i32 postfix_tab_count);
            virtual bool write_int(i32 i);
            virtual bool write_float(f32 f);
            virtual bool write_vec2(const vec2f& v);
            virtual bool write_vec3(const vec3f& v);
            virtual bool write_vec4(const vec4f& v);
    };
};