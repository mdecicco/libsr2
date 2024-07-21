#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class gfxImage {
        public:
            struct Pixel {
                u8 r, g, b, a;
            };

            gfxImage();
            ~gfxImage();

            void FUN_002f0440(u8 p1, bool p2);
            gfxImage* getNextMipMap();
            vec2i getDimensions();
            ImageFormat getFormat();
            u32 getTexEnv();
            Pixel* getPixels();

            static gfxImage* Create(u16 width, u16 height, ImageFormat fmt, u32 mipCount);
            static gfxImage* LoadTEX(const char* filename, bool p2);

        protected:
            u16 m_width;
            u16 m_height;
            i16 m_bytesPerRow;
            ImageFormat m_type;
            Pixel* m_pixels;
            Pixel* m_palette;
            gfxImage* m_nextMipMap;
            u32 m_texEnv;

            u8 field_0x7;
            undefined4 field_0x14;
    };
};