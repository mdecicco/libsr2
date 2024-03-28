#include <libsr2/io/datBinTokenizer.h>

namespace sr2 {
    i8 datBinTokenizer::readInt8() {
        return 0;
    }

    i16 datBinTokenizer::readInt16() {
        return 0;
    }

    i32 datBinTokenizer::readInt32() {
        return 0;
    }

    f32 datBinTokenizer::readFloat() {
        return 0.0f;
    }

    vec2f datBinTokenizer::readVec2() {
        return vec2f({ 0.0f, 0.0f });
    }

    vec3f datBinTokenizer::readVec3() {
        return vec3f({ 0.0f, 0.0f, 0.0f });
    }

    vec4f datBinTokenizer::readVec4() {
        return vec4f({ 0.0f, 0.0f, 0.0f, 0.0f });
    }

    i32 datBinTokenizer::readDelimiter(const char* token) {
        return 0;
    }

    i32 datBinTokenizer::matchInt(const char* token) {
        return 0;
    }

    f32 datBinTokenizer::matchFloat(const char* token) {
        return 0.0f;
    }

    vec2f datBinTokenizer::matchVec2(const char* token) {
        return vec2f({ 0.0f, 0.0f });
    }

    vec3f datBinTokenizer::matchVec3(const char* token) {
        return vec3f({ 0.0f, 0.0f, 0.0f });
    }

    vec4f datBinTokenizer::matchVec4(const char* token) {
        return vec4f({ 0.0f, 0.0f, 0.0f, 0.0f });
    }

    bool datBinTokenizer::writeString(const char* str, i32 postfix_tab_count) {
        return false;
    }

    bool datBinTokenizer::putDelimiter(const char* c) {
        return false;
    }

    bool datBinTokenizer::writeInt(i32 i) {
        return false;
    }

    bool datBinTokenizer::writeFloat(f32 f) {
        return false;
    }

    bool datBinTokenizer::writeVec2(const vec2f& v) {
        return false;
    }

    bool datBinTokenizer::writeVec3(const vec3f& v) {
        return false;
    }

    bool datBinTokenizer::writeVec4(const vec4f& v) {
        return false;
    }
};