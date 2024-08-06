#pragma once
#include <libsr2/types.h>

struct _iobuf;
typedef _iobuf FILE;

namespace sr2 {
    class Stream {
        public:
            Stream(FILE* fp);
            ~Stream();

            void close();
            i32 tell();
            i32 seek(i32 pos);
            i32 read(void* dst, u32 sz);
            i32 write(void* src, u32 sz);
            u32 getSize();
            char getCh();
            bool putCh(char ch);
            FILE* getRaw();

            static Stream* open(const char* path, bool unk0);

        protected:
            FILE* m_fp;
            u32 m_size;
    };
};