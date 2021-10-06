#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Data;

    class GameArchive {
        public:
            GameArchive(const char* path);
            ~GameArchive();

            Data* open(const char* path);
            Data* open(const char* directory, const char* filename, const char* extension);

            GameArchive* next;

        protected:
            struct entry {
                u64 name_offset;
                u64 offset;
                u32 compressed_sz;
                u32 decompressed_sz;
            };
            entry* m_files;
            u32 m_fileCount;
            Data* m_archive;
            void* m_fileMap;
    };

    extern GameArchive* g_Archives;

    void LoadArchives();
};

