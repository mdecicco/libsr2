#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class parCsvFile {
        public:
            parCsvFile(u32 bufSz = 0x100);
            ~parCsvFile();

            bool load(const char* dir, const char* file, u32 unk0, i32 unk1);
            void kill();

            u32 rowCount() const;
            u32 colCount() const;
            i32 getInt(u32 row, u32 col) const;
            char* getCell(u32 row, u32 col) const;
        
        protected:
            i32 m_unk0;
            u32 m_bufSz;
            u32 m_colCount;
            u32 m_rowCount;
            char** m_colNames;
            char** m_cells;
    };
};