#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Stream;

    class datAssetManager {
        public:
            static Stream* open(const char* dir, const char* filename, const char* ext, i32 unk0, bool unk1);
            static bool exists(const char* dir, const char* filename, const char* ext);
            static void full_path(char* buf, u32 bufSz, const char* dir, const char* filename, const char* ext);

            char* set_path(const char* path);

        protected:
            static char m_path[256];
    };
};