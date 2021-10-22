#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/stream.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace sr2 {
    i32 datAsciiTokenizer::read_int32() {
        char _buf[32];
        i32 len = get_token(_buf, 32);
        if ((len != 0 && _buf[0] == '-') || _buf[0] - '0' < 10) {
            return atoi(_buf);
        }

        return 0;
    }

    f32 datAsciiTokenizer::read_float() {
        char _buf[32];
        i32 len = get_token(_buf, 32);
        if ((len == 0 || _buf[0] != '-') && _buf[0] != '.' && 9 < _buf[0] - '0') {
            return 0.0f;
        } else {
            return atof(_buf);
        }
    }

    i32 datAsciiTokenizer::read_delimiter(char* token) {
        return match_token(token);
    }

    i32 datAsciiTokenizer::match_int(char* token) {
        match_token(token);
        return read_int32();
    }

    f32 datAsciiTokenizer::match_float(char* token) {
        match_token(token);
        return read_float();
    }

    vec2f datAsciiTokenizer::match_vec2(char* token) {
        match_token(token);
        return read_vec2();
    }

    vec3f datAsciiTokenizer::match_vec3(char* token) {
        match_token(token);
        return read_vec3();
    }

    vec4f datAsciiTokenizer::match_vec4(char* token) {
        match_token(token);
        return read_vec4();
    }

    bool datAsciiTokenizer::write_string(const char* str, i32 postfix_tab_count) {
        u32 len = strlen(str);
        u32 written = strm->write((void*)str, len);

        for (u32 i = 0;i < postfix_tab_count;i++) {
            strm->put_ch('\t');
            written++;
        }

        return written == len + postfix_tab_count;
    }

    bool datAsciiTokenizer::write_int(i32 i) {
        char _buf[64] = { 0 };
        u32 len = snprintf(_buf, 64, "%d ", i);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::write_float(f32 f) {
        char _buf[64] = { 0 };
        u32 len = snprintf(_buf, 64, "%f ", f);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::write_vec2(const vec2f& v) {
        char _buf[128] = { 0 };
        u32 len = snprintf(_buf, 64, "%f\t%f ", v.x, v.y);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::write_vec3(const vec3f& v) {
        char _buf[128] = { 0 };
        u32 len = snprintf(_buf, 64, "%f\t%f\t%f ", v.x, v.y, v.z);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }

    bool datAsciiTokenizer::write_vec4(const vec4f& v) {
        char _buf[128] = { 0 };
        u32 len = snprintf(_buf, 64, "%f\t%f\t%f\t%f ", v.x, v.y, v.z, v.w);
        u32 written = strm->write((void*)_buf, len);
        return written == len;
    }
};