#include <libsr2/io/stream.h>
#include <stdio.h>

namespace sr2 {
    Stream::Stream(FILE* fp) {
        m_fp = fp;
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
        /*
            Stream *pSVar1;
            long lVar2;
            code **func;
    
            func = (code **)PTR_PTR_FUN_00372e6c;
            if (param_2 != false) {
                func = (code **)DefaultOpenMethods;
            }
            lVar2 = (**func)(filename);
            if (lVar2 < 0) {
                pSVar1 = NULL;
            }
            else {
                pSVar1 = AllocStream(filename,(int)lVar2,(int)func);
            }
            return pSVar1;
        */

        FILE* fp = nullptr;
        fopen_s(&fp, path, "rb");
        if (fp) return new Stream(fp);

        return nullptr;
    }
};