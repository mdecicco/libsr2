#pragma once
#include <libsr2/types.h>

typedef struct _iobuf;
typedef _iobuf FILE;

namespace sr2 {
    class Stream {
        public:
            Stream(FILE* fp);
            ~Stream();

            void close();
            i32 read(void* dst, u32 sz);
            i32 write(void* src, u32 sz);
            char get_ch();
            bool put_ch(char ch);

            static Stream* open(char* path, bool unk0);

        protected:
            FILE* m_fp;
    };
};