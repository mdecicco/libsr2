#pragma once
#include <libsr2/types.h>
#include <string>

namespace sr2 {
    class Stream;
    class TaggedStream {
        public:
            TaggedStream();
            ~TaggedStream();

            Stream* open(const char* filename);
            Stream* open(const char* dir, const char* filename, const char* ext);
            Stream* open(Stream* strm);
            Stream* getStream();
            void closeOnError(bool value);
            void restart();
            void close();

            bool readTag(u16* tag, u32* size);

        protected:
            Stream* m_strm;
            Stream* m_field1_0x4;
            bool m_closeOnError;
            i32 m_readPos;
    };
};