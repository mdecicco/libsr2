#include <libsr2/utilities/txtFontTex.h>
#include <libsr2/io/stream.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/gfx/gfxImage.h>
#include <libsr2/globals.h>

#include <render/utils/ImGui.h>

#include <string.h>

namespace sr2 {
    txtFontTexGlyph::txtFontTexGlyph() {
        field_0x0 = 0;
        field_0x4 = 0;
        field_0x5 = 0;
        field_0x6 = 0;
        field_0x7 = 0;
        field_0x8 = 0;
        field_0x9 = 0;
    }

    txtFontTexGlyph::~txtFontTexGlyph() {
    }

    bool txtFontTexGlyph::load(Stream* fp) {
        fp->read(&field_0x0, 4);
        fp->read(&field_0x4, 1);
        fp->read(&field_0x5, 1);
        fp->read(&field_0x6, 1);
        fp->read(&field_0x7, 1);
        fp->read(&field_0x8, 1);
        return fp->read(&field_0x9, 1) != 0;
    }


    txtFont* txtFont::first = nullptr;
    txtFont::txtFont(u64 p1) {
        if (!first) {
            // todo
            // datCallback stuff
        }

        m_last = first;
        first = this;
        name = nullptr;
        field_0x8 = 1;
    }

    txtFont::~txtFont() {
        if (name) delete [] name;
        name = nullptr;
    }



    std::unordered_map<size_t, txtFontTex*> txtFontTex::loadedFonts = {};

    txtFontTex::txtFontTex(u64 p1) : txtFont(p1) {
        field_0x194 = 0;
        m_someArrCount = 0;
        m_glyphs = nullptr;
        m_name = nullptr;
        m_glyphCount = 0;
        m_glyphIndices[0] = 0;

        for (u32 i = 0;i < 24;i++) {
            m_someTexArr[i] = nullptr;
            m_someOtherTexArr[i] = nullptr;
            field_0x70[i] = nullptr;
            field_0x130[i] = nullptr;
        }
                                 
        field_0x20c = 0;
        field_0x208 = 0;
        field_0x21c = 0;
        field_0x218 = 0;
        field_0x214 = 0;
        field_0x210 = 0;
        field_0x22c = 0;
        field_0x228 = 0;
        field_0x224 = 0;
        field_0x220 = 0;
        field_0x230 = 0;
        field_0x1fc = 1;
        field_0x204 = 0x20;
        field_0x1f8 = 0x20;
        field_0x200 = 0x20;
        field_0x33c = 0;
    }

    txtFontTex::~txtFontTex() {
        destroy();
    }

    void txtFontTex::draw(
        f32 param_1,
        f32 param_2,
        i32 param_3,
        i32 param_4,
        wchar_t *param_5,
        const TextDisplayData& param_6,
        i32 param_7,
        i32 **param_8
    ) {
        char text[512] = { 0 };
        wcstombs(text, param_5, 512);

        auto dl = ImGui::GetWindowDrawList();
        dl->AddText(
            ImGui::GetFont(),
            debug_ui_font_size * debug_ui_scale,
            ImVec2((param_1 + param_6.pos.x) * debug_ui_scale, (param_2 + param_6.pos.y) * debug_ui_scale),
            param_6.color,
            text
        );
    }

    void txtFontTex::destroy() {
        // todo
        // FUN_002ef868(0x70000900, gfxTexture::None);

        for (u32 i = 0;i < 24;i++) {
            if (m_someTexArr[i]) gfxTexture::release(m_someTexArr[i]);
            m_someTexArr[i] = nullptr;
            
            if (m_someOtherTexArr[i]) gfxTexture::release(m_someOtherTexArr[i]);
            m_someOtherTexArr[i] = nullptr;

            if (field_0x70[i]) delete field_0x70[i];
            field_0x70[i] = nullptr;

            if (field_0x130[i]) delete field_0x130[i];
            field_0x130[i] = nullptr;
        }

        if (m_name) delete [] m_name;    
        m_name = nullptr;

        if (m_glyphs) delete [] m_glyphs;
        m_glyphs = nullptr;

        m_glyphCount = 0;
        m_someArrCount = 0;
        field_0x194 = 0;
    }

    void txtFontTex::reset(u32 glyphCount, const char* fontName) {
        destroy();

        u32 nameLen = strlen(fontName);
        m_name = new char[nameLen + 1];
        m_name[nameLen] = 0;
        strcpy(m_name, fontName);

        m_glyphCount = glyphCount;
        if (m_glyphCount > 0) {
            m_glyphs = new txtFontTexGlyph[m_glyphCount];
        }

        memset(m_glyphIndices, 0, 256);
        field_0x33c = 0;
    }

    bool txtFontTex::load(const char* fontName) {
        reset(0, "default");

        if (m_name) delete [] m_name;
        u32 nameLen = strlen(fontName);
        m_name = new char[nameLen + 1];
        m_name[nameLen] = 0;
        strcpy(m_name, fontName);

        char buf[256] = { 0 };
        snprintf(buf, 256, "%s.fonttex", fontName);
        Stream* fp = Stream::open(buf, true);
        if (!fp) {
            char buf1[256] = { 0 };
            datAssetManager::full_path(buf1, 256, "fonts", fontName, "fonttex");
            fp = Stream::open(buf1, true);

            if (!fp) {
                char buf2[512] = { 0 };
                strncpy(buf2, datAssetManager::get_path(), 512);
                strncat(buf2, buf1, 512);

                fp = Stream::open(buf2, true);
                if (!fp) return false;
            }
        }

        auto epsilonCheck = [](f32 val, f32 cmp) {
            return fabsf(val - cmp) < 0.000001f;
        };

        f32 maybeVersion = 0.0f;
        fp->read(&maybeVersion, 4);
        fp->read(&field_0x1f8, 4);
        fp->read(&field_0x1fc, 4);
        fp->read(&field_0x200, 4);

        if (maybeVersion > 1.01 || epsilonCheck(maybeVersion, 1.01)) fp->read(&field_0x204, 4);
        field_0x204 = field_0x200 * 2;

        if (maybeVersion > 1.02 || epsilonCheck(maybeVersion, 1.02)) {
            fp->read(&field_0x208, 4);
            fp->read(&field_0x20c, 4);
            fp->read(&field_0x210, 4);
            fp->read(&field_0x214, 4);
            fp->read(&field_0x218, 4);
            fp->read(&field_0x21c, 4);
        }

        fp->read(&m_glyphCount, 4);

        m_glyphs = new txtFontTexGlyph[m_glyphCount];
        for (u32 i = 0;i < m_glyphCount;i++) {
            m_glyphs[i].load(fp);
        }

        fp->read(&m_someArrCount, 4);
        fp->read(&m_someArr, m_someArrCount * 4);

        bool prevIgnorePrefix = datAssetManager::doIgnorePrefix();
        datAssetManager::setIgnorePrefix(true);

        for (u32 i = 0;i < m_someArrCount;i++) {
            char texNameBuf[256] = { 0 };
            if (prevIgnorePrefix) {
                snprintf(texNameBuf, 256, "%sfonts/%s_%02d", datAssetManager::get_path(), fontName, i);
            } else {
                snprintf(texNameBuf, 256, "%s_%02d", fontName, i);
            }

            m_someOtherTexArr[i] = gfxTexture::get(texNameBuf, 1, 0);;
            if (!m_someOtherTexArr[i]) {
                char texNameBuf1[512] = { 0 };
                strncpy(texNameBuf1, datAssetManager::get_path(), 512);
                strncat(texNameBuf1, texNameBuf, 512);
                
                m_someOtherTexArr[i] = gfxTexture::get(texNameBuf1, 1, 0);;

                if (field_0x214 || field_0x210 || field_0x218 || field_0x21c) {
                    strncat(texNameBuf1, "s", 512);
                    m_someTexArr[i] = gfxTexture::get(texNameBuf1, 1, 0);
                }
            } else {
                if (field_0x214 || field_0x210 || field_0x218 || field_0x21c) {
                    strncat(texNameBuf, "s", 256);
                    m_someTexArr[i] = gfxTexture::get(texNameBuf, 1, 0);
                }
            }

            if (m_someOtherTexArr[i]) m_someOtherTexArr[i]->setTexEnv(m_someOtherTexArr[i]->getTexEnv() & 0xfffbffff);
            if (m_someTexArr[i]) m_someTexArr[i]->setTexEnv(m_someTexArr[i]->getTexEnv() & 0xfffbffff);
        }

        datAssetManager::setIgnorePrefix(prevIgnorePrefix);

        field_0x204 = 0;
        for (u32 i = 0;i < m_glyphCount;i++) {
            i32 v = m_glyphs[i].field_0x6 + m_glyphs[i].field_0x9;
            if (field_0x204 < v) field_0x204 = v;
        }

        field_0x33c = 0;
        if (m_glyphCount == 0 || m_glyphs[0].field_0x0 > 0xff) return true;

        for (u32 i = 0;i < m_glyphCount;i++) {
            field_0x33c = u16(i);
            m_glyphIndices[m_glyphs[i].field_0x0] = i;

            if (i + 1 >= m_glyphCount || m_glyphs[i + 1].field_0x0 > 0xff) break;
        }

        return true;
    }

    void txtFontTex::setName(const char* fontName) {
    }

    txtFontTex* txtFontTex::get(const char* fontName) {
        char buf[80] = { 0 };

        u64 unk1 = 0;

        if (!fontName || !*fontName) {
            fontName = "arial112";
            unk1 = 0x50;
        } else {
            for (u32 i = 0;i < 80;i++, unk1++) {
                char ch = fontName[i];
                if (ch - 0x41 < 0x1a) ch += 0x20;
                buf[i] = ch;
                if (fontName[i + 1] == 0) break;
            }
        }

        size_t hash = std::hash<const char*>()(buf);

        auto it = loadedFonts.find(hash);
        if (it != loadedFonts.end()) return it->second;

        txtFontTex* font = new txtFontTex(unk1);
        if (!font->load(buf)) {
            if (!font->load("arial112")) {
                delete font;
                return nullptr;
            }

            font->setName(buf);
        }

        loadedFonts.insert(std::pair<size_t, txtFontTex*>(hash, font));
        return font;
    }
};