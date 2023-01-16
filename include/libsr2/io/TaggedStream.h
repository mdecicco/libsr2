#pragma once
#include <libsr2/types.h>
#include <string>

namespace sr2 {
    class Stream;
    class TaggedStream {
        public:
            TaggedStream();
            ~TaggedStream();

            Stream* open(char* filename);
            Stream* open(char* dir, char* filename, char* ext);
            Stream* open(Stream* strm);
            Stream* getStream();
            void close();

            bool readTag(u16* tag, u32* size);

        protected:
            Stream* m_strm;
            Stream* m_field1_0x4;
            i32 m_field2_0x8;
            i32 m_readPos;
    };
};