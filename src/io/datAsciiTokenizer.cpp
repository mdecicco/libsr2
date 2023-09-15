#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/stream.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace sr2 {
    i32 datAsciiTokenizer::readInt32() {
        char _buf[32];
        i32 len = getToken(_buf, 32);
        if ((len != 0 && _buf[0] == '-') || _buf[0] - '0' < 10) {
            return atoi(_buf);
        }

        return 0;
    }

    f32 datAsciiTokenizer::readFloat() {
        char _buf[32];
        i32 len = getToken(_buf, 32);
        if ((len == 0 || _buf[0] != '-') && _buf[0] != '.' && 9 < _buf[0] - '0') {
            return 0.0f;
        } else {
            return atof(_buf);
        }
    }

    i32 datAsciiTokenizer::readDelimiter(char* token) {
        return matchToken(token);
    }

    i32 datAsciiTokenizer::matchInt(char* token) {
        matchToken(token);
        return readInt32();
    }

    f32 datAsciiTokenizer::matchFloat(char* token) {
        matchToken(token);
        return readFloat();
    }

    vec2f datAsciiTokenizer::matchVec2(char* token) {
        matchToken(token);
        return readVec2();
    }

    vec3f datAsciiTokenizer::matchVec3(char* token) {
        matchToken(token);
        return readVec3();
    }

    vec4f datAsciiTokenizer::matchVec4(char* token) {
        matchToken(token);
        return readVec4();
    }

    bool datAsciiTokenizer::writeString(const char* str, i32 postfix_tab_count) {
        u32 len = strlen(str);
        u32 written = strm->write((void*)str, len);

        for (u32 i = 0;i < postfix_tab_count;i++) {
            strm->putCh('\t');
            written++;
        }

        return written == len + postfix_tab_count;
    }

    bool datAsciiTokenizer::writeInt(i32 i) {
        char _buf[64] = { 0 };
        u32 len = snprintf(_buf, 64, "%d ", i);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::writeFloat(f32 f) {
        char _buf[64] = { 0 };
        u32 len = snprintf(_buf, 64, "%f ", f);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::writeVec2(const vec2f& v) {
        char _buf[128] = { 0 };
        u32 len = snprintf(_buf, 64, "%f\t%f ", v.x, v.y);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::writeVec3(const vec3f& v) {
        char _buf[128] = { 0 };
        u32 len = snprintf(_buf, 64, "%f\t%f\t%f ", v.x, v.y, v.z);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::writeVec4(const vec4f& v) {
        char _buf[128] = { 0 };
        u32 len = snprintf(_buf, 64, "%f\t%f\t%f\t%f ", v.x, v.y, v.z, v.w);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }
};