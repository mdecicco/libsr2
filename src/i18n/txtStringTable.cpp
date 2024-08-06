#include <libsr2/i18n/txtStringTable.h>
#include <libsr2/io/stream.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/utilities/txtFontTex.h>
#include <string.h>

namespace sr2 {
    bool txtStringData::load(Stream* fp, u16 strFlags) {
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

    u32 strTblHash(const char* str) { 
        char cVar1;
        char cVar2;
        u32 uVar3;
        u32 uVar4;
        
        uVar3 = 0;
        cVar2 = *str;
        cVar1 = *str;
        while (cVar1 != '\0') {
            uVar3 = uVar3 * 0x10 + i32(cVar2);
            uVar4 = uVar3 & 0xf0000000;
            if (uVar4 != 0) uVar3 = uVar3 ^ uVar4 >> 0x18 ^ uVar4;

            str++;
            cVar2 = *str;
            cVar1 = *str;
        }
        return uVar3;
    }

    txtStringTable::txtStringTable() {
        m_isInitialized = false;
        m_keyCount = 0;
        m_keys = nullptr;
        m_someFlag = 0;
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
            datAssetManager::fullPath(buf, 100, "fonts", p1, "strtbl");
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
            delete fp;
            return false;
        }

        fp->seek(selectedLangOffset);

        u32 stringCount = 0;
        fp->read(&stringCount, 4);
        for (u32 i = 0;i < stringCount;i++) {
            txtStringData* data = new txtStringData();
            if (data->load(fp, strFlags)) {
                m_stringMap.insert(std::pair(data->hash, data));
            }
        }

        delete fp;
        return stringCount != 0;
    }

    void txtStringTable::setSomeFlag(u32 flag) {
        m_someFlag = flag;
    }

    txtStringData* txtStringTable::findString(const char* str) {
        u32 hash = strTblHash(str);
        auto it = m_stringMap.find(hash);
        if (it != m_stringMap.end()) return it->second;

        wchar_t outBuf[32] = { 0 };

        if (m_someFlag == 2) {
            char buf[32] = "!!";
            strcat(buf, str);
            strcat(buf, "!!");
            i32 len = i32(strlen(buf));
            for (u32 i = 0;i < len;i++) outBuf[i] = buf[i];
        } else {
            i32 len = i32(strlen(str));
            for (u32 i = 0;i < len;i++) outBuf[i] = str[i];
        }

        txtStringData* data = new txtStringData;
        u32 len = wcslen(outBuf);
        data->text = new wchar_t[len + 1];
        memcpy(data->text, outBuf, len * 2);
        data->hash = hash;
        data->font = txtFontTex::get(nullptr);

        m_stringMap.insert(std::pair(hash, data));
        return data;
    }
};