#include <libsr2/io/stream.h>
#include <stdio.h>
#include <filesystem>

namespace sr2 {
    Stream::Stream(FILE* fp) {
        m_fp = fp;
        fseek(m_fp, 0, SEEK_END);
        m_size = (u32)ftell(m_fp);
        fseek(m_fp, 0, SEEK_SET);
    }

    Stream::~Stream() {
        if (m_fp) fclose(m_fp);
        m_fp = nullptr;
    }

    void Stream::close() {
        if (m_fp) fclose(m_fp);
        m_fp = nullptr;
    }

    i32 Stream::tell() {
        return ftell(m_fp);
    }

    i32 Stream::seek(i32 pos) {
        return fseek(m_fp, pos, SEEK_SET);
    }

    i32 Stream::read(void* dst, u32 sz) {
        return fread(dst, sz, 1, m_fp) * sz;
    }

    i32 Stream::write(void* src, u32 sz) {
        return fwrite(src, sz, 1, m_fp) * sz;
    }

    u32 Stream::getSize() {
        return m_size;
    }

    char Stream::getCh() {
        char out;
        if (!read(&out, 1)) return 0xFF;
        return out;
    }

    bool Stream::putCh(char ch) {
        return write(&ch, 1) == 1;
    }

    FILE* Stream::getRaw() {
        return m_fp;
    }

    Stream* Stream::open(const char* path, bool unk0) {
        FILE* fp = nullptr;
        errno_t err = fopen_s(&fp, path, "rb");
        if (fp) return new Stream(fp);

        return nullptr;
    }
};