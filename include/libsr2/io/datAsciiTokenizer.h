#pragma once
#include <libsr2/types.h>
#include <libsr2/io/datBaseTokenizer.h>

namespace sr2 {
    class datAsciiTokenizer : public datBaseTokenizer {
        public:
            virtual i32 readInt32();
            virtual f32 readFloat();
            virtual i32 readDelimiter(char* token);
            virtual i32 matchInt(char* token);
            virtual f32 matchFloat(char* token);
            virtual vec2f matchVec2(char* token);
            virtual vec3f matchVec3(char* token);
            virtual vec4f matchVec4(char* token);
            virtual bool writeString(const char* str, i32 postfix_tab_count);
            virtual bool writeInt(i32 i);
            virtual bool writeFloat(f32 f);
            virtual bool writeVec2(const vec2f& v);
            virtual bool writeVec3(const vec3f& v);
            virtual bool writeVec4(const vec4f& v);
    };
};