#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Stream;

    class datBaseTokenizer {
        public:
            datBaseTokenizer();
            ~datBaseTokenizer();

            void init(const char* filename, Stream* file);

            i32 get_token(char* buf, u32 bufSz);
            char get_token_ch();
            void skip_comment();
            void skip_to_end_of_line();
            void push_back(char* data, u32 len);
            i32 match_token(char* tok);
            i32 match_i_token(char* tok);
            i32 check_token(char* tok, bool dont_backup_on_match);
            i32 check_i_token(char* tok, bool dont_backup_on_match);

            virtual i8 read_int8();
            virtual i16 read_int16();
            virtual i32 read_int32() = 0;
            virtual f32 read_float() = 0;
            virtual vec2f read_vec2();
            virtual vec3f read_vec3();
            virtual vec4f read_vec4();
            virtual i32 read_delimiter(char* token) = 0;
            virtual i32 match_int(char* token) = 0;
            virtual f32 match_float(char* token) = 0;
            virtual vec2f match_vec2(char* token) = 0;
            virtual vec3f match_vec3(char* token) = 0;
            virtual vec4f match_vec4(char* token) = 0;
            virtual bool write_string(const char* str, i32 postfix_tab_count) = 0;
            virtual bool put_delimiter(char* c);
            virtual bool put_ch(char c);
            virtual bool write_int(i32 i) = 0;
            virtual bool write_float(f32 f) = 0;
            virtual bool write_vec2(const vec2f& v) = 0;
            virtual bool write_vec3(const vec3f& v) = 0;
            virtual bool write_vec4(const vec4f& v) = 0;

            const char* filename;
            Stream* strm;
            u32 line_idx;
            i32 current_char;
            u32 buf_len;
            u8 buf[64];
    };
};