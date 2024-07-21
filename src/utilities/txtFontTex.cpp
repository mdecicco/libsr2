#include <libsr2/utilities/txtFontTex.h>
#include <libsr2/io/stream.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/gfx/gfxImage.h>
#include <libsr2/globals.h>

#include <render/utils/ImGui.h>
#include <render/vulkan/DescriptorSet.h>
#include <render/vulkan/VertexBuffer.h>
#include <render/vulkan/UniformBuffer.h>

#include <utils/Array.hpp>

#include <string.h>

namespace sr2 {
    u32 fontHash(const char* str) { 
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

    txtFontTexGlyph::txtFontTexGlyph() {
        charCode = 0;
        position = { 0, 0 };
        size = { 0, 0 };
        offsetY = 0;
        offsetX = 0;
    }

    txtFontTexGlyph::~txtFontTexGlyph() {
    }

    bool txtFontTexGlyph::load(Stream* fp) {
        fp->read(&charCode, 4);
        fp->read(&position, 2);
        fp->read(&size, 2);
        fp->read(&offsetY, 1);
        return fp->read(&offsetX, 1) != 0;
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
        m_textureCount = 0;
        m_glyphs = nullptr;
        m_name = nullptr;
        m_glyphCount = 0;
        m_glyphIndices[0] = 0;

        for (u32 i = 0;i < 24;i++) {
            m_shadowTextures[i] = nullptr;
            m_textures[i] = nullptr;
            field_0x70[i] = nullptr;
            field_0x130[i] = nullptr;
            m_elements[i] = nullptr;
            m_shadowElements[i] = nullptr;
            m_elementGlyphCounts[i] = 0;
        }
                                 
        field_0x20c = 0;
        m_glyphOffsetY = 0;
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
        m_widestGlyphWidth = 0x20;
        m_advanceY = 0x20;
        m_advanceX = 0x20;
        field_0x33c = 0;
    }

    txtFontTex::~txtFontTex() {
        destroy();
    }

    void txtFontTex::draw(
        f32 posX,
        f32 posY,
        i32 param_3,
        i32 param_4,
        wchar_t *text,
        TextDisplayData* data,
        i32 param_7,
        i32 **param_8
    ) {
        /*
        {
            char ftxt[512] = { 0 };
            wcstombs(ftxt, text, 512);

            auto dl = ImGui::GetWindowDrawList();
            dl->AddText(
                ImGui::GetFont(),
                debug_ui_font_size * debug_ui_scale,
                ImVec2((posX + data->pos.x) * debug_ui_scale, (posY + data->pos.y) * debug_ui_scale),
                data->color,
                ftxt
            );
        }
        */

        u32 len = wcslen(text);
        wchar_t beginChar = L'!';
        bool someFlag = false;

        utils::vec2f cursorPos = {
            0.499f,
            0.499f
        };

        bool useCustomAdvanceX = false;
        f32 advanceX = m_widestGlyphWidth;

        if (data) {
            if ((data->flags & 8) == 0 || (data->left <= data->pos.x && data->pos.x <= data->right)) {
                cursorPos.x += data->pos.x;
            } else {
                cursorPos.x += data->left;
            }

            cursorPos.y += data->pos.y;

            useCustomAdvanceX = (data->flags >> 7) & 1;
            if (data->advanceX != 0) advanceX = data->advanceX;
        }

        u32 lastWordBegin = 0;
        for (u32 i = 0;i < len;i++) {
            wchar_t ch = text[i];

            u32 texIdx = 0;
            for (u32 t = 0;t < m_textureCount;t++) {
                if ((ch - beginChar) < m_textureGlyphCounts[t]) {
                    texIdx = t;
                    break;
                }
            }

            gfxTexture* tex = m_textures[texIdx];
            u32 glyphIdx = m_glyphIndices[ch];

            m_elementGlyphCounts[texIdx]++;
            if (m_elementGlyphCounts[texIdx] >= 1024) continue;

            if (ch == L'\n') {
                if (!data) cursorPos.x = 0.499f;
                else cursorPos.x = data->left + 0.499f;
                cursorPos.y += m_advanceY;
                continue;
            }

            if (ch == L'\b') {
                cursorPos.x -= (useCustomAdvanceX ? advanceX : m_advanceX) * 0.25f;
                continue;
            }

            if (ch == L' ') {
                cursorPos.x += useCustomAdvanceX ? advanceX : m_advanceX;
                lastWordBegin = i + 1;
                continue;
            }

            txtFontTexGlyph& glyph = m_glyphs[glyphIdx];
            if (useCustomAdvanceX) {
                f32 iVar12 = floorf(advanceX - glyph.size.x);
                if (iVar12 < 0) iVar12 = 0;

                f32 fVar28 = cursorPos.y + f32(glyph.offsetY) + f32(m_glyphOffsetY);
                if (someFlag) fVar28 += f32(field_0x20c);

                f32 uVar25 = posX * f32(glyph.size.x);

                utils::vec2f glyphPos = {
                    posX + iVar12 + cursorPos.x,
                    posY * fVar28
                };

                if (data) {
                    if ((data->flags & 8) && data->right <= (glyphPos.x + (posX * f32(glyph.size.x)))) {
                        m_renderedGlyphs.push({
                            cursorPos,
                            cursorPos + utils::vec2f(posX * f32(glyph.size.x), posY * f32(glyph.size.y)),
                            utils::vec2f(glyph.position.x, glyph.position.y),
                            utils::vec2f(glyph.position.x + glyph.size.x, glyph.position.y + glyph.size.y),
                            tex,
                            data ? data->color : 0xFFFFFFFF
                        });

                        cursorPos.y += m_advanceY;
                        cursorPos.x = data->left + 0.499f;
                        continue;
                    }

                    if (
                        (data->flags & 5) == 0 &&
                        (
                            data->right  <= (glyphPos.x + (posX * f32(glyph.size.x))) ||
                            data->bottom <= (glyphPos.y + (posY * f32(glyph.size.y))) ||
                            glyphPos.x < data->left ||
                            glyphPos.y < data->top
                        )
                    ) {
                        // ? ? ?
                    }
                }

                m_renderedGlyphs.push({
                    glyphPos,
                    glyphPos + utils::vec2f(posX * f32(glyph.size.x), posY * f32(glyph.size.y)),
                    utils::vec2f(glyph.position.x, glyph.position.y),
                    utils::vec2f(glyph.position.x + glyph.size.x, glyph.position.y + glyph.size.y),
                    tex,
                    data ? data->color : 0xFFFFFFFF
                });

                cursorPos.x += advanceX;
                continue;
            } else {
                f32 fVar28 = cursorPos.y + f32(glyph.offsetY) + f32(m_glyphOffsetY);
                if (someFlag) fVar28 += f32(field_0x20c);

                f32 uVar25 = posX * f32(glyph.size.x);

                utils::vec2f glyphPos = {
                    posX * cursorPos.x,
                    posY * fVar28
                };

                if (!data) {
                    m_renderedGlyphs.push({
                        glyphPos,
                        glyphPos + utils::vec2f(posX * f32(glyph.size.x), posY * f32(glyph.size.y)),
                        utils::vec2f(glyph.position.x, glyph.position.y),
                        utils::vec2f(glyph.position.x + glyph.size.x, glyph.position.y + glyph.size.y),
                        tex,
                        data ? data->color : 0xFFFFFFFF
                    });

                    cursorPos.x += f32(glyph.size.x) + f32(glyph.offsetX) + f32(field_0x1fc);
                    continue;
                }

                if ((data->flags & 8) == 0 || (glyphPos.x + uVar25) < data->right) {
                    if (
                        (data->flags & 5) == 0 &&
                        (
                            data->right  <= (glyphPos.x + (posX * f32(glyph.size.x))) ||
                            data->bottom <= (glyphPos.y + (posY * f32(glyph.size.y))) ||
                            glyphPos.x < data->left ||
                            glyphPos.y < data->top
                        )
                    ) {
                        // ? ? ?
                    }

                    m_renderedGlyphs.push({
                        glyphPos,
                        glyphPos + utils::vec2f(posX * f32(glyph.size.x), posY * f32(glyph.size.y)),
                        utils::vec2f(glyph.position.x, glyph.position.y),
                        utils::vec2f(glyph.position.x + glyph.size.x, glyph.position.y + glyph.size.y),
                        tex,
                        data ? data->color : 0xFFFFFFFF
                    });
                    cursorPos.x += f32(glyph.size.x) + f32(glyph.offsetX) + f32(field_0x1fc);
                    continue;
                }

                while (i >= lastWordBegin) {
                    if (i > lastWordBegin) m_renderedGlyphs.pop();
                    i--;
                }

                cursorPos.y += m_advanceY;
                cursorPos.x = data->left + 0.499f;
            }
        }

        if (data) {
            if ((data->flags & 0x100) == 0) {
                data->pos.x = cursorPos.x;
                data->pos.y = cursorPos.y;
            } else {
                data->pos.x = 0;
                data->pos.y = cursorPos.y + m_advanceY;
            }
        }
    }

    void txtFontTex::destroy() {
        // todo
        // FUN_002ef868(0x70000900, gfxTexture::None);

        for (u32 i = 0;i < 24;i++) {
            if (m_shadowTextures[i]) gfxTexture::release(m_shadowTextures[i]);
            m_shadowTextures[i] = nullptr;
            
            if (m_textures[i]) gfxTexture::release(m_textures[i]);
            m_textures[i] = nullptr;

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
        m_textureCount = 0;
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
        fp->read(&m_advanceY, 4);
        fp->read(&field_0x1fc, 4);
        fp->read(&m_advanceX, 4);

        if (maybeVersion > 1.01 || epsilonCheck(maybeVersion, 1.01)) fp->read(&m_widestGlyphWidth, 4);
        m_widestGlyphWidth = m_advanceX * 2;

        if (maybeVersion > 1.02 || epsilonCheck(maybeVersion, 1.02)) {
            fp->read(&m_glyphOffsetY, 4);
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

        fp->read(&m_textureCount, 4);
        fp->read(&m_textureGlyphCounts, m_textureCount * 4);

        bool prevIgnorePrefix = datAssetManager::doIgnorePrefix();
        datAssetManager::setIgnorePrefix(true);

        for (u32 i = 0;i < m_textureCount;i++) {
            char texNameBuf[256] = { 0 };
            if (!prevIgnorePrefix) {
                snprintf(texNameBuf, 256, "%sfonts/%s_%02d", datAssetManager::get_path(), fontName, i);
            } else {
                snprintf(texNameBuf, 256, "%s_%02d", fontName, i);
            }

            m_textures[i] = gfxTexture::get(texNameBuf, 1, 0);
            if (!m_textures[i]) {
                char texNameBuf1[512] = { 0 };
                strncpy(texNameBuf1, datAssetManager::get_path(), 512);
                strncat(texNameBuf1, texNameBuf, 512);
                
                m_textures[i] = gfxTexture::get(texNameBuf1, 1, 0);

                if (field_0x214 || field_0x210 || field_0x218 || field_0x21c) {
                    strncat(texNameBuf1, "s", 512);
                    m_shadowTextures[i] = gfxTexture::get(texNameBuf1, 1, 0);
                }
            } else {
                if (field_0x214 || field_0x210 || field_0x218 || field_0x21c) {
                    strncat(texNameBuf, "s", 256);
                    m_shadowTextures[i] = gfxTexture::get(texNameBuf, 1, 0);
                }
            }

            if (m_textures[i]) {
                m_textures[i]->setTexEnv(m_textures[i]->getTexEnv() & 0xfffbffff);
                m_elements[i] = uiRenderer::createElement(1024 * 6);
                m_elements[i]->descriptor->add(m_textures[i]->getTex(), 1);
                m_elements[i]->descriptor->update();
            }
            
            if (m_shadowTextures[i]) {
                m_shadowTextures[i]->setTexEnv(m_shadowTextures[i]->getTexEnv() & 0xfffbffff);
                m_shadowElements[i] = uiRenderer::createElement(1024 * 6);
                m_shadowElements[i]->descriptor->add(m_shadowTextures[i]->getTex(), 1);
                m_shadowElements[i]->descriptor->update();
            }
        }

        datAssetManager::setIgnorePrefix(prevIgnorePrefix);

        m_widestGlyphWidth = 0;
        for (u32 i = 0;i < m_glyphCount;i++) {
            i32 v = m_glyphs[i].size.x + m_glyphs[i].offsetX;
            if (m_widestGlyphWidth < v) m_widestGlyphWidth = v;
        }

        field_0x33c = 0;
        if (m_glyphCount == 0 || m_glyphs[0].charCode > 0xff) return true;

        for (u32 i = 0;i < m_glyphCount;i++) {
            field_0x33c = u16(i);
            m_glyphIndices[m_glyphs[i].charCode] = i;

            if (i + 1 >= m_glyphCount || m_glyphs[i + 1].charCode > 0xff) break;
        }

        delete fp;
        return true;
    }

    void txtFontTex::setName(const char* fontName) {
    }

    void txtFontTex::render() {
        for (u32 i = 0;i < m_textureCount;i++) {
            if (m_elementGlyphCounts[i] == 0) continue;

            render::vulkan::Vertices* verts = m_elements[i]->vertices;
            if (verts->beginUpdate()) {
                u32 v = 0;

                for (u32 g = 0;g < m_renderedGlyphs.size();g++) {
                    rendered_glyph& glyph = m_renderedGlyphs[g];
                    if (glyph.texture != m_textures[i]) continue;

                    utils::vec2f pMin = glyph.vMin * debug_ui_scale;
                    utils::vec2f pMax = glyph.vMax * debug_ui_scale;

                    sr2::vec2i dims = m_textures[i]->getDimensions();
                    utils::vec2f invDims = utils::vec2f(1.0f / f32(dims.x), 1.0f / f32(dims.y));

                    // UVs adjusted inward by 0.5 pixels because otherwise
                    // bits of other glyphs are included
                    utils::vec2f uvMin = (glyph.uvMin + utils::vec2f(0.5f, 0.5f)) * invDims;
                    utils::vec2f uvMax = (glyph.uvMax - utils::vec2f(0.5f, 0.5f)) * invDims;
                    utils::vec4f color = utils::vec4f::FromU32(_byteswap_ulong(glyph.color));

                    verts->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMin.x, pMin.y, -0.5f), utils::vec2f(uvMin.x, uvMin.y), color };
                    verts->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMax.x, pMin.y, -0.5f), utils::vec2f(uvMax.x, uvMin.y), color };
                    verts->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMax.x, pMax.y, -0.5f), utils::vec2f(uvMax.x, uvMax.y), color };
                    verts->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMin.x, pMin.y, -0.5f), utils::vec2f(uvMin.x, uvMin.y), color };
                    verts->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMax.x, pMax.y, -0.5f), utils::vec2f(uvMax.x, uvMax.y), color };
                    verts->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMin.x, pMax.y, -0.5f), utils::vec2f(uvMin.x, uvMax.y), color };
                }

                verts->commitUpdate();
                m_elements[i]->doRender = true;
                m_elements[i]->vertexCount = v;
            }

            m_elementGlyphCounts[i] = 0;
        }

        m_renderedGlyphs.clear();
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

        size_t hash = fontHash(buf);

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

    void txtFontTex::renderAll() {
        for (auto& it : loadedFonts) {
            it.second->render();
        }
    }
};