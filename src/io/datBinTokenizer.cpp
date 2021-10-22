#include <libsr2/io/datBinTokenizer.h>

namespace sr2 {
    i8 datBinTokenizer::read_int8() {
        return 0;
    }

    i16 datBinTokenizer::read_int16() {
        return 0;
    }

    i32 datBinTokenizer::read_int32() {
        return 0;
    }

    f32 datBinTokenizer::read_float() {
        return 0.0f;
    }

    vec2f datBinTokenizer::read_vec2() {
        return vec2f({ 0.0f, 0.0f });
    }

    vec3f datBinTokenizer::read_vec3() {
        return vec3f({ 0.0f, 0.0f, 0.0f });
    }

    vec4f datBinTokenizer::read_vec4() {
        return vec4f({ 0.0f, 0.0f, 0.0f, 0.0f });
    }

    i32 datBinTokenizer::read_delimiter(char* token) {
        return 0;
    }

    i32 datBinTokenizer::match_int(char* token) {
        return 0;
    }

    f32 datBinTokenizer::match_float(char* token) {
        return 0.0f;
    }

    vec2f datBinTokenizer::match_vec2(char* token) {
        return vec2f({ 0.0f, 0.0f });
    }

    vec3f datBinTokenizer::match_vec3(char* token) {
        return vec3f({ 0.0f, 0.0f, 0.0f });
    }

    vec4f datBinTokenizer::match_vec4(char* token) {
        return vec4f({ 0.0f, 0.0f, 0.0f, 0.0f });
    }

    bool datBinTokenizer::write_string(const char* str, i32 postfix_tab_count) {
        return false;
    }

    bool datBinTokenizer::put_delimiter(char* c) {
        return false;
    }

    bool datBinTokenizer::write_int(i32 i) {
        return false;
    }

    bool datBinTokenizer::write_float(f32 f) {
        return false;
    }

    bool datBinTokenizer::write_vec2(const vec2f& v) {
        return false;
    }

    bool datBinTokenizer::write_vec3(const vec3f& v) {
        return false;
    }

    bool datBinTokenizer::write_vec4(const vec4f& v) {
        return false;
    }
};