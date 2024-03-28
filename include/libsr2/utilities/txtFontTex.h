#pragma once
#include <libsr2/types.h>
#include <unordered_map>

namespace sr2 {
    class Stream;
    class gfxTexture;
    class gfxImage;

    class txtFontTexGlyph {
        public:
            txtFontTexGlyph();
            ~txtFontTexGlyph();

            bool load(Stream* fp);
        
            undefined4 field_0x0;
            u8 field_0x4;
            u8 field_0x5;
            u8 field_0x6;
            u8 field_0x7;
            u8 field_0x8;
            u8 field_0x9;
    };

    class txtFont {
        public:
            txtFont(u64 p1);
            ~txtFont();

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

            void destroy();
            void reset(u32 glyphCount, const char* fontName);
            bool load(const char* fontName);
            void setName(const char* fontName);

            static txtFontTex* get(const char* fontName);
        
        protected:
            static std::unordered_map<size_t, txtFontTex*> loadedFonts;

            char* m_name;
            undefined4 m_glyphCount;
            txtFontTexGlyph* m_glyphs;
            u32 m_someArrCount;
            undefined4 m_someArr[24];
            gfxTexture* m_someTexArr[24];
            gfxTexture* m_someOtherTexArr[24];
            u8 m_glyphIndices[256];
            
            gfxImage* field_0x70[24];
            gfxImage* field_0x130[24];            
            undefined4 field_0x194;
            undefined4 field_0x198;
            undefined4 field_0x1f8;
            undefined4 field_0x1fc;
            undefined4 field_0x200;
            undefined4 field_0x204;
            undefined4 field_0x208;
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