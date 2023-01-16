#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>
#include <libsr2/managers/datAssetManager.h>

namespace sr2 {
    TaggedStream::TaggedStream() {
        m_strm = nullptr;
        m_field2_0x8 = 1;
        m_readPos = 0;
        m_field1_0x4 = nullptr;
    }

    TaggedStream::~TaggedStream() {
        if (m_strm) m_strm->close();
        m_strm = nullptr;
    }

    Stream* TaggedStream::open(char* filename) {
        Stream* s = Stream::open(filename, true);
        return open(s);
    }

    Stream* TaggedStream::open(char* dir, char* filename, char* ext) {
        Stream* s = datAssetManager::open(dir, filename, ext, 0, true);
        return open(s);
    }

    Stream* TaggedStream::open(Stream* strm) {
        m_field1_0x4 = nullptr;
        m_strm = strm;
        if (!strm) return nullptr;

        // Should be 'TSV1' (aka. 0x31565354)
        u32 magic;
        if ((strm->read(&magic, 4) >> 2 == 1) && magic == 0x31565354) {
            m_readPos = 4;
        } else {
            strm->close();
            m_strm = nullptr;
            return nullptr;
        }

        return strm;
    }
    
    Stream* TaggedStream::getStream() {
        return m_strm;
    }

    void TaggedStream::close() {
        if (!m_strm) return;
        if (m_field1_0x4) {
            u16 unk = 0;
            m_strm->write(&unk, 2);
        }
        m_strm->close();
        m_strm = nullptr;
        m_readPos = 0;
    }

    bool TaggedStream::readTag(u16* tag, u32* size) {
        if (!m_strm) return false;
        if (m_field1_0x4) return false;

        u32 pos = m_strm->tell();
        if (pos < m_readPos) m_strm->seek(m_readPos);
        else if (m_readPos < pos) {
            if (m_field2_0x8) close();
            return false;
        }

        *tag = 0;
        if ((m_strm->read(tag, 2) >> 1 == 1) && *tag) {
            *size = 0;
            if (m_strm->read(size, 4) >> 2 == 1) {
                m_readPos += 6 + *size;
                return true;
            }
        }

        if (m_field2_0x8) close();
        return false;
    }
};