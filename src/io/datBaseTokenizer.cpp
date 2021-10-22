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

    i32 datBaseTokenizer::get_token(char* buf, u32 len) {
        bool bVar1;

        do {
            bool is_whitespace = current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r' || !current_char;
            if (!is_whitespace) break;

            if (current_char == '\n') line_idx++;

            current_char = get_token_ch();
        } while (true);

        u32 out_len = 0;

        if (current_char == '"') {
            current_char = get_token_ch();

            if (current_char != -1 && current_char != '"') {
                for (;out_len < len - 1;out_len++) {
                    current_char = get_token_ch();
                    if (current_char == -1 || current_char != '\"') break;
                    
                    buf[out_len] = current_char;
                }
            }

            current_char = get_token_ch();
        }
        else {
            for (;current_char != -1;out_len++) {
                if (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r' || !current_char) break;

                while (current_char == ';') {
                    skip_comment();

                    if (out_len == 0) {
                        do {
                            if (current_char != ' ' && current_char != '\t' && current_char != '\n' && current_char != '\r' && current_char) {
                                continue;
                            }
                            current_char = get_token_ch();
                        } while (true);
                    }
                }

                if (current_char != -1 && out_len < len + -1) {
                    buf[out_len] = current_char;
                }

                current_char = get_token_ch();
            }
        }

        buf[out_len] = '\0';
        return out_len;
    }

    char datBaseTokenizer::get_token_ch() {
        if (buf_len == 0) {
            return strm->get_ch();
        } else {
            return buf[--buf_len];
        }
    }

    void datBaseTokenizer::skip_comment() { 
        char cVar1;
        i32 cchar;

        cchar = current_char;
        if (cchar == ';') {
            do {
                if (cchar != -1 && cchar != '\n' && cchar != '\r') {
                    do {
                        current_char = get_token_ch();
                    } while (current_char != '\r' && current_char != '\n' && current_char != -1);
                }
            } while (cchar == ';');
        }
    }

    void datBaseTokenizer::skip_to_end_of_line() {
        char ch = 0;
        do {
            ch = strm->get_ch();
            if (ch == -1) break;
        } while (ch != '\n');
    }

    void datBaseTokenizer::push_back(char* data, u32 len) {
        buf_len = len + 1;
        char a = *data;
        while (a != 0) {
            data++;
            buf[len--] = a;
            a = *data;
        }
        buf[0] = 0;
    }

    i32 datBaseTokenizer::match_token(char* tok) {
        char _buf[64] = { 0 };
        i32 result = get_token(_buf, 64);
        if (result) return strcmp(tok, _buf);
        return result;
    }

    i32 datBaseTokenizer::match_i_token(char* tok) {
        char _buf[64] = { 0 };
        i32 result = get_token(_buf, 64);
        if (result) return strcmp(tok, _buf);
        return result;
    }

    i32 datBaseTokenizer::check_token(char* tok, bool dont_backup_on_match) {
        char _buf[64] = { 0 };
        i32 len = get_token(_buf, 64);

        if (len == 0 || strcmp(tok, _buf) != 0) {
            push_back(_buf, len);
            return false;
        } else if (!dont_backup_on_match) {
            push_back(_buf, len);
        }

        return true;
    }

    i32 datBaseTokenizer::check_i_token(char* tok, bool dont_backup_on_match) {
        char _buf[64] = { 0 };
        i32 len = get_token(_buf, 64);

        if (len == 0 || strcmp(tok, _buf) != 0) {
            push_back(_buf, len);
            return false;
        } else if (!dont_backup_on_match) {
            push_back(_buf, len);
        }

        return true;
    }

    i8 datBaseTokenizer::read_int8() {
        return read_int32();
    }

    i16 datBaseTokenizer::read_int16() {
        return read_int32();
    }

    vec2f datBaseTokenizer::read_vec2() {
        return vec2f({
            read_float(),
            read_float()
        });
    }

    vec3f datBaseTokenizer::read_vec3() {
        return vec3f({
            read_float(),
            read_float(),
            read_float()
        });
    }

    vec4f datBaseTokenizer::read_vec4() {
        return vec4f({
            read_float(),
            read_float(),
            read_float(),
            read_float()
        });
    }
    
    bool datBaseTokenizer::put_delimiter(char* c) {
        return write_string(c, 0);
    }

    bool datBaseTokenizer::put_ch(char c) {
        return strm->put_ch(c);
    }
};