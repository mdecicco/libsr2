#include <libsr2/io/datMultiTokenizer.h>
#include <libsr2/io/stream.h>
#include <string.h>

namespace sr2 {
    datBaseTokenizer* datMultiTokenizer::GetReadTokenizer(const char* filename, Stream* file, const char* ascii_type_id, const char* bin_type_id) {
        char type_str[32] = { 0 };
        u32 tlen = strlen(ascii_type_id);

        file->read(type_str, tlen);
        type_str[tlen] = 0;

        if (strcmp(type_str, ascii_type_id) != 0) {
            if (strcmp(type_str, bin_type_id) == 0) {
                m_binTok.init(filename, file);
                return &m_binTok;
            }
        }

        m_asciiTok.init(filename, file);
        return &m_asciiTok;
    }
};