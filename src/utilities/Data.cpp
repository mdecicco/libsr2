#include <libsr2/utilities/Data.h>
#include <libsr2/utilities/utils.h>
#include <exception>

namespace sr2 {
    Data::Data(u8* ptr, u64 size, bool copyData) {
        m_data = nullptr;
        m_ownsData = copyData;
        m_position = 0;
        m_size = size;

        if (copyData) {
            m_data = new u8[size];
            memcpy(m_data, ptr, size);
        } else {
            m_data = ptr;
        }
    }

    Data::Data(const char* srcFilePath) {
        m_data = nullptr;
        m_ownsData = true;
        m_position = 0;
        FILE* fp = fopen(srcFilePath, "rb");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            m_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            m_data = new u8[m_size];
            if (fread(m_data, m_size, 1, fp) != 1) {
                throw std::exception(format("Data::Data Failed to read %llu bytes from '%s'.", m_size, srcFilePath).c_str());
            }
            fclose(fp);
        } else {
            throw std::exception(format("Data::Data Failed open '%s'.", srcFilePath).c_str());
        }
    }

    Data::~Data() {
        if (m_data && m_ownsData) delete [] m_data;
        m_data = nullptr;
    }

    void Data::read(void* dest, u64 sz) {
        if ((m_position + sz) > m_size) {
            u64 remain = m_size - m_position;
            throw std::exception(format("Data::read Cannot read %llu bytes from Data. %s%llu byte%s remain%s between the current offset and the end of the data.", sz, remain, remain == 1 || remain == 0 ? "" : "Only ", remain == 1 ? "" : "s", remain == 1 ? "s" : "").c_str());
        }

        memcpy(dest, m_data + m_position, sz);
        m_position += sz;
    }

    u16 Data::read_str(char* str, u16 bufSz) {
        u16 len = 0;
        char c = 0;
        
        do {
            read(c);
            *str = c;
            str++;
            len++;
        } while (c != 0 && m_position != m_size);

        if (len < bufSz) *str = 0;
        else {
            throw std::exception(format("Data::read_str Destination string buffer size not large enough for string.").c_str());
        }

        return len;
    }
    
    u64 Data::size() const {
        return m_size;
    }

    void Data::position(u64 pos) {
        if (pos > m_size) {
            throw std::exception(format("Data::position Cannot set Data position to %llu. Size is %llu.", pos, m_size).c_str());
        }

        m_position = pos;
    }

    u64 Data::position() const {
        return m_position;
    }

    Data* Data::isolate(u64 sz, bool copy) {
        if ((m_position + sz) > m_size) {
            u64 remain = m_size - m_position;
            throw std::exception(format("Data::isolate Cannot read %llu bytes from Data. %s%llu byte%s remain%s between the current offset and the end of the data.", sz, remain, remain == 1 || remain == 0 ? "" : "Only ", remain == 1 ? "" : "s", remain == 1 ? "s" : "").c_str());
        }
        return new Data(m_data + m_position, sz, copy);
    }

    void* Data::raw() {
        return m_data + m_position;
    }

    void Data::take_ownership() {
        m_ownsData = true;
    }
};