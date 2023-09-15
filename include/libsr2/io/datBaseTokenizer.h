#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Stream;

    class datBaseTokenizer {
        public:
            datBaseTokenizer();
            ~datBaseTokenizer();

            void init(const char* filename, Stream* file);

            i32 getToken(char* buf, u32 bufSz);
            char getTokenCh();
            void skipComment();
            void skipToEndOfLine();
            void pushBack(char* data, u32 len);
            i32 matchToken(char* tok);
            i32 matchIntToken(char* tok);
            i32 checkToken(char* tok, bool dont_backup_on_match);
            i32 checkIntToken(char* tok, bool dont_backup_on_match);

            virtual i8 readInt8();
            virtual i16 readInt16();
            virtual i32 readInt32() = 0;
            virtual f32 readFloat() = 0;
            virtual vec2f readVec2();
            virtual vec3f readVec3();
            virtual vec4f readVec4();
            virtual i32 readDelimiter(char* token) = 0;
            virtual i32 matchInt(char* token) = 0;
            virtual f32 matchFloat(char* token) = 0;
            virtual vec2f matchVec2(char* token) = 0;
            virtual vec3f matchVec3(char* token) = 0;
            virtual vec4f matchVec4(char* token) = 0;
            virtual bool writeString(const char* str, i32 postfix_tab_count) = 0;
            virtual bool putDelimiter(char* c);
            virtual bool putCh(char c);
            virtual bool writeInt(i32 i) = 0;
            virtual bool writeFloat(f32 f) = 0;
            virtual bool writeVec2(const vec2f& v) = 0;
            virtual bool writeVec3(const vec3f& v) = 0;
            virtual bool writeVec4(const vec4f& v) = 0;

            const char* filename;
            Stream* strm;
            u32 line_idx;
            i32 current_char;
            u32 buf_len;
            u8 buf[64];
    };
};