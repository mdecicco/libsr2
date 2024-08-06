#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Stream;

    class datAssetManager {
        public:
            static Stream* open(const char* dir, const char* filename, const char* ext, i32 unk0, bool unk1);
            static Stream* open(const char* filename, const char* ext, i32 unk0, bool unk1);
            static bool exists(const char* dir, const char* filename, const char* ext);
            static void fullPath(char* buf, u32 bufSz, const char* dir, const char* filename, const char* ext);
            static void fullPath(char* buf, u32 bufSz, const char* filename, const char* ext);
            static char* setPath(const char* path);
            static char* getPath();
            static bool doIgnorePrefix();
            static void setIgnorePrefix(bool ignore);

        protected:
            static char m_path[256];
            static bool m_ignorePrefix;
    };
};