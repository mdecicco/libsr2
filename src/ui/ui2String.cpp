#include <libsr2/ui/ui2String.h>
#include <string.h>
#include <stdio.h>

namespace sr2 {
    ui2String::ui2String(u32 len) {
        m_text = const_cast<char*>("");
        m_ownsPtr = len != 0;
        m_length = len;

        if (m_length == 0) return;
        m_text = new char[len + 1];
        m_text[0] = 0;
    }
    
    ui2String::ui2String(const char* text, bool isConst) {
        m_text = const_cast<char*>("");
        m_ownsPtr = false;
        m_length = 0;

        if (isConst) setConst(text);
        else set(text);
    }
    
    ui2String::ui2String(const ui2String& other, u32 len) {
        m_text = const_cast<char*>("");
        m_ownsPtr = len != 0;
        m_length = len;

        if (m_length == 0) return;
        m_text = new char[len + 1];

        set(other.m_text);
    }

    ui2String::~ui2String() {
        if (m_text && m_ownsPtr) delete [] m_text;
        m_text = nullptr;
    }

    void ui2String::setConst(const char* text) {
        if (m_text && m_ownsPtr) delete [] m_text;
        m_length = 0;
        if (!text) {
            m_text = const_cast<char*>("");
            m_ownsPtr = false;
        } else {
            // m_text = text;
            // m_ownsPtr = true;
            
            u32 len = strlen(text);
            m_text = new char[len + 1];
            strncpy(m_text, text, len);
            m_ownsPtr = true;
        }
    }

    void ui2String::set(const char* text) {
        if (m_length == 0) {
            if (m_ownsPtr && m_text) delete [] m_text;
            if (!text || text[0] == 0) {
                m_ownsPtr = false;
                m_text = const_cast<char*>("");
            } else {
                u32 len = strlen(text);
                m_text = new char[len + 1];
                strncpy(m_text, text, len);
                m_ownsPtr = true;
            }
        } else if (!text) {
            m_text[0] = 0;
            return;
        } else {
            strncpy(m_text, text, m_length - 1);
        }
    }

    const char* ui2String::get() const {
        return m_text;
    }
};