#include <libsr2/io/datBaseTokenizer.h>
#include <libsr2/io/stream.h>
#include <string.h>

namespace sr2 {
    datBaseTokenizer::datBaseTokenizer() {
        strm = nullptr;
    }

    datBaseTokenizer::~datBaseTokenizer() {
    }

    void datBaseTokenizer::init(const char* _filename, Stream* file) {
        line_idx = 1;
        filename = _filename;
        strm = file;
        current_char = ' ';
        buf[0] = 0;
        buf_len = 0;
    }

    i32 datBaseTokenizer::getToken(char* buf, u32 len) {
        bool bVar1;

        do {
            bool is_whitespace = current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r' || !current_char;
            if (!is_whitespace) break;

            if (current_char == '\n') line_idx++;

            current_char = getTokenCh();
        } while (true);

        u32 out_len = 0;

        if (current_char == '"') {
            current_char = getTokenCh();

            if (current_char != -1 && current_char != '"') {
                for (;out_len < len - 1;out_len++) {
                    current_char = getTokenCh();
                    if (current_char == -1 || current_char != '\"') break;
                    
                    buf[out_len] = current_char;
                }
            }

            current_char = getTokenCh();
        }
        else {
            for (;current_char != -1;out_len++) {
                if (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r' || !current_char) break;

                while (current_char == ';') {
                    skipComment();

                    if (out_len == 0) {
                        do {
                            if (current_char != ' ' && current_char != '\t' && current_char != '\n' && current_char != '\r' && current_char) {
                                continue;
                            }
                            current_char = getTokenCh();
                        } while (true);
                    }
                }

                if (current_char != -1 && out_len < len + -1) {
                    buf[out_len] = current_char;
                }

                current_char = getTokenCh();
            }
        }

        buf[out_len] = '\0';
        return out_len;
    }

    char datBaseTokenizer::getTokenCh() {
        if (buf_len == 0) {
            return strm->getCh();
        } else {
            return buf[--buf_len];
        }
    }

    void datBaseTokenizer::skipComment() { 
        char cVar1;
        i32 cchar;

        cchar = current_char;
        if (cchar == ';') {
            do {
                if (cchar != -1 && cchar != '\n' && cchar != '\r') {
                    do {
                        current_char = getTokenCh();
                    } while (current_char != '\r' && current_char != '\n' && current_char != -1);
                }
            } while (cchar == ';');
        }
    }

    void datBaseTokenizer::skipToEndOfLine() {
        char ch = 0;
        do {
            ch = strm->getCh();
            if (ch == -1) break;
        } while (ch != '\n');
    }

    void datBaseTokenizer::pushBack(const char* data, u32 len) {
        buf_len = len + 1;
        char a = *data;
        while (a != 0) {
            data++;
            buf[len--] = a;
            a = *data;
        }
        buf[0] = 0;
    }

    i32 datBaseTokenizer::matchToken(const char* tok) {
        char _buf[64] = { 0 };
        i32 result = getToken(_buf, 64);
        if (result) return strcmp(tok, _buf);
        return result;
    }

    i32 datBaseTokenizer::matchIntToken(const char* tok) {
        char _buf[64] = { 0 };
        i32 result = getToken(_buf, 64);
        if (result) return strcmp(tok, _buf);
        return result;
    }

    i32 datBaseTokenizer::checkToken(const char* tok, bool dont_backup_on_match) {
        char _buf[64] = { 0 };
        i32 len = getToken(_buf, 64);

        if (len == 0 || strcmp(tok, _buf) != 0) {
            pushBack(_buf, len);
            return false;
        } else if (!dont_backup_on_match) {
            pushBack(_buf, len);
        }

        return true;
    }

    i32 datBaseTokenizer::checkIntToken(const char* tok, bool dont_backup_on_match) {
        char _buf[64] = { 0 };
        i32 len = getToken(_buf, 64);

        if (len == 0 || strcmp(tok, _buf) != 0) {
            pushBack(_buf, len);
            return false;
        } else if (!dont_backup_on_match) {
            pushBack(_buf, len);
        }

        return true;
    }

    i8 datBaseTokenizer::readInt8() {
        return readInt32();
    }

    i16 datBaseTokenizer::readInt16() {
        return readInt32();
    }

    vec2f datBaseTokenizer::readVec2() {
        return vec2f({
            readFloat(),
            readFloat()
        });
    }

    vec3f datBaseTokenizer::readVec3() {
        return vec3f({
            readFloat(),
            readFloat(),
            readFloat()
        });
    }

    vec4f datBaseTokenizer::readVec4() {
        return vec4f({
            readFloat(),
            readFloat(),
            readFloat(),
            readFloat()
        });
    }
    
    bool datBaseTokenizer::putDelimiter(const char* c) {
        return writeString(c, 0);
    }

    bool datBaseTokenizer::putCh(char c) {
        return strm->putCh(c);
    }
};