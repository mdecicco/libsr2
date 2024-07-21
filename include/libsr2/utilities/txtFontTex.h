#pragma once
#include <libsr2/types.h>
#include <libsr2/gfx/ui.h>
#include <unordered_map>

#include <utils/Array.h>

namespace sr2 {
    class Stream;
    class gfxTexture;
    class gfxImage;
    
    struct TextDisplayData {
        u32 flags;
        vec2i pos;
        i32 top;
        i32 bottom;
        i32 left;
        i32 right;
        u32 color;

        u32 field_0x1c;
        i32 field_0x20;
        undefined4 advanceX;
        undefined4 field_0x28;
        undefined4 field_0x2c;
    };

    class txtFontTexGlyph {
        public:
            txtFontTexGlyph();
            ~txtFontTexGlyph();

            bool load(Stream* fp);
        
            u32 charCode;
            vec2ub position;
            vec2ub size;
            i8 offsetY;
            i8 offsetX;
    };

    class txtFont {
        public:
            txtFont(u64 p1);
            ~txtFont();

            virtual void draw(
                f32 posX,
                f32 posY,
                i32 param_3,
                i32 param_4,
                wchar_t *param_5,
                TextDisplayData* data,
                i32 param_7,
                i32 **param_8
            ) = 0;

        protected:
            static txtFont* first;

            txtFont* m_last;

            char* name;
            undefined4 field_0x8;
    };

    class txtFontTex : public txtFont {
        public:
            txtFontTex(u64 p1);
            ~txtFontTex();

            struct rendered_glyph {
                ::utils::vec2f vMin, vMax;
                ::utils::vec2f uvMin, uvMax;
                gfxTexture* texture;
                u32 color;
            };

            virtual void draw(
                f32 posX,
                f32 posY,
                i32 param_3,
                i32 param_4,
                wchar_t *param_5,
                TextDisplayData* data,
                i32 param_7,
                i32 **param_8
            );

            void destroy();
            void reset(u32 glyphCount, const char* fontName);
            bool load(const char* fontName);
            void setName(const char* fontName);
            void render();

            static txtFontTex* get(const char* fontName);
            static void renderAll();
        
        protected:
            static std::unordered_map<size_t, txtFontTex*> loadedFonts;
            utils::Array<rendered_glyph> m_renderedGlyphs;

            char* m_name;
            undefined4 m_glyphCount;
            txtFontTexGlyph* m_glyphs;
            u32 m_textureCount;
            i32 m_textureGlyphCounts[24];
            gfxTexture* m_shadowTextures[24];
            gfxTexture* m_textures[24];
            uiRenderer::element* m_elements[24];
            uiRenderer::element* m_shadowElements[24];
            u32 m_elementGlyphCounts[24];
            u8 m_glyphIndices[256];
            u32 m_advanceX;
            u32 m_advanceY;
            u32 m_widestGlyphWidth;
            i32 m_glyphOffsetY;
            
            gfxImage* field_0x70[24];
            gfxImage* field_0x130[24];            
            undefined4 field_0x194;
            undefined4 field_0x198;
            i32 field_0x1fc;
            undefined4 field_0x20c;
            undefined4 field_0x210;
            undefined4 field_0x22c;
            undefined4 field_0x228;
            undefined4 field_0x224;
            undefined4 field_0x220;
            undefined4 field_0x230;
            undefined4 field_0x214;
            undefined4 field_0x218;
            undefined4 field_0x21c;
            undefined2 field_0x33c;
    };
};