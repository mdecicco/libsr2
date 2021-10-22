#pragma once
#include <libsr2/types.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/datBinTokenizer.h>

namespace sr2 {
    class datMultiTokenizer {
        public:

            datBaseTokenizer* GetReadTokenizer(const char* filename, Stream* file, const char* ascii_type_id, const char* bin_type_id);
        
        protected:
            datAsciiTokenizer m_asciiTok;
            datBinTokenizer m_binTok;
    };
};