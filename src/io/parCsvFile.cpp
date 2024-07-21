#include <libsr2/io/parCsvFile.h>
#include <libsr2/io/stream.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/utilities/utils.h>

#include <stdlib.h>

namespace sr2 {
    parCsvFile::parCsvFile(u32 bufSz) {
        m_unk0 = 0x10;
        m_bufSz = bufSz;
        m_cells = nullptr;
        m_colCount = m_rowCount = 0;
        m_colNames = new char*[64];
    }

    parCsvFile::~parCsvFile() {
        kill();
    }

    bool parCsvFile::load(const char* dir, const char* filename, u32 unk0, i32 unk1) {
        Stream *fp = datAssetManager::open(dir, filename, "csv", 0, true);
        if (!fp) return false;

        char* buf = new char[m_bufSz];
        if (unk0 == 0) {
            fgets(buf, m_bufSz, fp->getRaw());
            char* off = strchr(buf, '#');
            if (off) {
                off = strchr(buf, '#');
                *off = '\0';
            }
        } else {
            for (i32 i = unk1;i >= 0;i--) {
                fgets(buf, m_bufSz, fp->getRaw());
                char* off = strchr(buf, '#');
                if (off) {
                    off = strchr(buf, '#');
                    *off = '\0';
                }
            }
        }

        m_colCount = 0;
        if (*buf != '\0' && 0 < m_unk0) {
            char* n = buf;
            char* n1 = nullptr;
            char curCh = 0;
            do {
                curCh = *n;
                bool breakCond = false;
                while (curCh == '\0' || ('\x1f' < curCh)) {
                    n1 = n;
                    if (*n < ' ') curCh = *n1;
                    else {
                        if (*n != ',') {
                            bool someCond = true;
                            do {
                                n1 = n1 + 1;
                                if (*n1 < ' ') {
                                    curCh = *n1;
                                    someCond = false;
                                }
                            } while (*n1 != ',');
                            if (someCond) curCh = *n1;
                        } else curCh = *n;
                    }

                    if (curCh != '\0') {
                        *n1 = '\0';
                        n1 = n1 + 1;
                    }

                    if (n == n1) curCh = *n1;
                    else {
                        n = duplicate_string(n);
                        m_colNames[m_colCount++] = n;
                        curCh = *n1;
                    }

                    if ((curCh == '\0') || (m_unk0 <= m_colCount)) {
                        breakCond = true;
                        break;
                    }
                    n = n1;
                    curCh = *n1;
                }

                if (breakCond) break;
                n++;
            } while (true);
        }

        if (unk0 != 0) {
            fp->seek(0);
            for (i32 i = unk1;i > 0;i--) {
                fgets(buf, m_bufSz, fp->getRaw());
            }
        }

        m_rowCount = 0;
        while (fgets(buf, m_bufSz, fp->getRaw())) m_rowCount++;

        fp->seek(0);
        if (unk0 == 0) fgets(buf, m_bufSz, fp->getRaw());
        else if (0 < unk1) {
            while (true) {
                unk1 += -1;
                fgets(buf, m_bufSz, fp->getRaw());
                if (unk1 == 0) break;
            }
        }

        m_cells = new char*[m_rowCount * m_colCount];
        memset(m_cells, 0, m_rowCount * m_colCount * sizeof(char*));

        m_rowCount = 0;
        do {
            if (!fgets(buf, m_bufSz, fp->getRaw())) {
                delete fp;
                delete [] buf;
                return true;
            }

            char* off = strchr(buf,'#');
            if (off != NULL) {
                off = strchr(buf,'#');
                *off = '\0';
            }

            u32 rowIdx = 0;
            bool bVar1 = false;
            if (*buf != '\0' && 0 < m_colCount) {
                char* n = buf;
                char* n1 = nullptr;
                char curCh = 0;
                do {
                    curCh = *n;
                    bool breakCond = false;
                    while ((curCh == '\0' || ('\x1f' < curCh))) {
                        n1 = n;
                        if (*n < ' ') curCh = *n1;
                        else {
                            if (*n != ',') {
                                bool someCond = true;
                                do {
                                    n1 = n1 + 1;
                                    if (*n1 < ' ') {
                                        curCh = *n1;
                                        someCond = false;
                                        break;
                                    }
                                } while (*n1 != ',');
                                if (someCond) curCh = *n1;
                            } else curCh = *n;
                        }

                        if (curCh != '\0') {
                            *n1 = '\0';
                            n1 = n1 + 1;
                        }

                        if (n == n1) curCh = *n1;
                        else {
                            n = duplicate_string(n);
                            bVar1 = true;
                            m_cells[m_rowCount * m_colCount + rowIdx] = n;
                            rowIdx += 1;
                            curCh = *n1;
                        }

                        if ((curCh == '\0') || (m_colCount <= rowIdx)) {
                            breakCond = true;
                            break;
                        }
                        n = n1;
                        curCh = *n1;
                    }

                    if (breakCond) break;
                    n = n + 1;
                } while (true);
            }

            if (bVar1) {
                m_rowCount++;
                continue;
            }
        } while (true);

        delete fp;
    }

    void parCsvFile::kill() {
        int iVar1;
        int iVar2;
        char **ppcVar3;
        int iVar4;
        
        iVar4 = 0;
        if (m_cells) {
            for (u32 i = 0;i < m_colCount;i++) {
                if (m_colNames[i]) delete [] m_colNames[i];
                m_colNames[i] = nullptr;
            }

            u32 cellCount = m_colCount * m_rowCount;
            for (u32 i = 0;i < cellCount;i++) {
                if (m_cells[i]) delete [] m_cells[i];
                m_cells[i] = nullptr;
            }
            
            delete [] m_cells;
            m_cells = nullptr;
        }

        if (m_colNames) delete [] m_colNames;
        m_colNames = nullptr;
    }

    u32 parCsvFile::rowCount() const {
        return m_rowCount;
    }
    u32 parCsvFile::colCount() const {
        return m_colCount;
    }
    i32 parCsvFile::getInt(u32 row, u32 col) const {
        return atoi(m_cells[row * m_colCount + col]);
    }
    char* parCsvFile::getCell(u32 row, u32 col) const {
        return m_cells[row * m_colCount + col];
    }
};