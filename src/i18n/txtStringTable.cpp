#include <libsr2/i18n/txtStringTable.h>
#include <libsr2/io/stream.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/utilities/txtFontTex.h>
#include <string.h>

namespace sr2 {
    bool txtStringTable::txtStringData::load(Stream* fp, u16 strFlags) {
        hash = 0;
        font = nullptr;
        text = nullptr;
        field_0xc = { 0.0f, 0.0f };
        field_0x14 = 0;
        field_0x15 = 0;
        flags = 0;

        char fontName[32] = { 0 };
        wchar_t textBuf[512] = { 0 };

        fp->read(&hash, 4);
        fp->read(&flags, 2);

        u32 fontNameLen;
        fp->read(&fontNameLen, 4);

        if (fontNameLen) {
            fp->read(fontName, fontNameLen);
            fontName[fontNameLen] = 0;
        }

        u32 textLen;
        fp->read(&textLen, 4);
        fp->read(textBuf, textLen * 2);
        textBuf[textLen] = 0;

        fp->read(&field_0xc, 8);
        fp->read(&field_0x14, 1);
        fp->read(&field_0x15, 1);

        if (flags & strFlags) {
            font = txtFontTex::get(fontName);
            text = new wchar_t[textLen + 1];
            
            wchar_t* src = textBuf;
            wchar_t* dst = text;
            while (*src) {
                *dst = *src;
                src++;
                dst++;
            }

            *dst = 0;

            return true;
        }

        return false;
    }



    txtStringTable::txtStringTable() {
        m_isInitialized = false;
        m_keyCount = 0;
        m_keys = nullptr;
    }

    txtStringTable::~txtStringTable() {
        kill();
    }

    void txtStringTable::kill() {
        m_stringMap.clear();

        if (m_keys) {
            for (u32 i = 0;i < m_keyCount;i++) delete [] m_keys[i];
            delete [] m_keys;
        }

        m_keys = nullptr;
        m_keyCount = 0;

        m_isInitialized = false;
    }

    void txtStringTable::init() {
        kill();
        m_isInitialized = true;
    }

    bool txtStringTable::isInitialized() {
        return m_isInitialized;
    }

    bool txtStringTable::load(const char* p1, LANGUAGE lang, u16 strFlags, bool loadKeys) {
        kill();
        Stream* fp = Stream::open(p1, true);
        if (!fp) {
            char buf[100];
            datAssetManager::full_path(buf, 100, "fonts", p1, "strtbl");
            fp = Stream::open(buf, true);
            if (!fp) return false;
        }

        u32 count;
        fp->read(&count, 4);

        u32 selectedLangOffset = 0;

        for (u32 i = 0;i < count;i++) {
            u32 offset;
            fp->read(&offset, 4);
            if (i == lang) selectedLangOffset = offset;
        }

        u32 unused;
        fp->read(&unused, 4);

        if (loadKeys) {
            fp->read(&m_keyCount, 4);
            m_keys = new char*[m_keyCount];

            for (u32 i = 0;i < m_keyCount;i++) {
                u32 len;
                fp->read(&len, 4);

                m_keys[i] = new char[len] + 1;
                fp->read(m_keys[i], len + 1);
                m_keys[i][len] = 0;
            }
        }

        if (selectedLangOffset == 0) {
            fp->close();
            return false;
        }

        fp->seek(selectedLangOffset);

        u32 stringCount = 0;
        fp->read(&stringCount, 4);
        for (u32 i = 0;i < stringCount;i++) {
            txtStringData* data = new txtStringData();
            if (data->load(fp, strFlags)) {
                m_stringMap.insert(std::pair<size_t, txtStringData*>(data->hash, data));
            }
        }

        fp->close();
        return stringCount != 0;
    }
};