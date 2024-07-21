#include <libsr2/gfx/gfxImage.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/stream.h>

#include <assert.h>

namespace sr2 {
    static u8 bppByType[] = { 0, 0x20, 0x18, 0x10, 0x10, 0x8, 0x4, 0x20, 0x18, 0x10, 0x4 };

    gfxImage::gfxImage() {
    }

    gfxImage::~gfxImage() {
    }

    void gfxImage::FUN_002f0440(u8 p1, bool p2) {
        field_0x7 = p1;
        if (p2 && m_nextMipMap) m_nextMipMap->FUN_002f0440(p1, p2);
    }
    
    gfxImage* gfxImage::getNextMipMap() {
        return m_nextMipMap;
    }
    
    vec2i gfxImage::getDimensions() {
        return { m_width, m_height };
    }
    
    ImageFormat gfxImage::getFormat() {
        return m_type;
    }
    
    u32 gfxImage::getTexEnv() {
        return m_texEnv;
    }
    
    gfxImage::Pixel* gfxImage::getPixels() {
        return m_pixels;
    }

    gfxImage* gfxImage::Create(u16 width, u16 height, ImageFormat fmt, u32 mipCount) {
        gfxImage* img = new gfxImage();

        img->field_0x7 = 1;
        img->field_0x14 = 1;
        img->m_nextMipMap = nullptr;
        img->m_width = width;
        img->m_height = height;
        // Just force rgba8888...
        // img->m_type = fmt;
        // img->m_bytesPerRow = (width * bppByType[u32(fmt)]) >> 3;
        // img->m_pixels = new u8[img->m_bytesPerRow * img->m_height];
        img->m_type = ImageFormat::RGBA8888;
        img->m_bytesPerRow = width * 4;
        img->m_pixels = new Pixel[width * height];
        img->m_texEnv = 0;

        if (u32(fmt) - 5 < 2) {
            u32 paletteSz = 0x100;
            if (fmt == ImageFormat::RGBA_8888_P4) paletteSz = 0x10;

            // img->m_palette = new u8[paletteSz << 2];
            img->m_palette = new Pixel[0x100];
        } else {
            img->m_palette = nullptr;
        }

        if (mipCount && width > 8 && height > 8) {
            img->m_nextMipMap = gfxImage::Create(width >> 1, height >> 1, fmt, mipCount - 1);
            if (img->m_nextMipMap && img->m_palette) {
                delete [] img->m_palette;
                img->m_palette = img->m_nextMipMap->m_palette;
            }
        }

        return img;
    }
    
    gfxImage* gfxImage::LoadTEX(const char* filename, bool p2) {
        enum TexPixelFmt : u16 {
            P8 = 1,
            PA8 = 0xe,
            P4_MC = 0xf,
            PA4_MC = 0x10,
            RGB888 = 0x11,
            RGBA8888 = 0x12
        };

        struct TexHeader {
            u16 width, height;
            TexPixelFmt format;
            u16 mipCount;
            u16 unk;
            union {
                unsigned unknown : 31;
                unsigned is_flipped : 1;
            } flags;
        };

        Stream* fp = datAssetManager::open("texture", filename, "tex", 1, true);
        if (!fp) return nullptr;

        TexHeader hdr;
        fp->read(&hdr, 0xe);
        u32 mipCount = 1;
        if (p2) mipCount = hdr.mipCount;

        gfxImage* ret = Create(hdr.width, hdr.height, ImageFormat::RGBA8888, mipCount - 1);
        switch (hdr.format) {
            case P8: {
                Pixel* palette = new Pixel[0x100];
                fp->read(palette, 0x400);

                u8* indices = new u8[hdr.width * hdr.height];

                gfxImage* dst = ret;
                for (u32 i = 0;i < mipCount && dst;i++) {
                    u32 count = dst->m_width * dst->m_height;
                    fp->read(indices, count);

                    for (u32 p = 0;p < count;p++) {
                        Pixel& src = palette[indices[p]];
                        dst->m_pixels[p] = { src.b, src.g, src.r, src.a };
                    }

                    dst = dst->m_nextMipMap;
                }

                delete [] indices;
                delete [] palette;

                break;
            }
            case PA8: {
                Pixel* palette = new Pixel[0x100];
                fp->read(palette, 0x400);

                u8* indices = new u8[hdr.width * hdr.height];

                gfxImage* dst = ret;
                for (u32 i = 0;i < mipCount && dst;i++) {
                    u32 count = dst->m_width * dst->m_height;
                    fp->read(indices, count);

                    for (u32 p = 0;p < count;p++) {
                        Pixel& src = palette[indices[p]];
                        dst->m_pixels[p] = { src.b, src.g, src.r, src.a };
                    }

                    dst = dst->m_nextMipMap;
                }

                delete [] indices;
                delete [] palette;
                break;
            }
            case RGB888: {
                u8* pixels = new u8[hdr.width * hdr.height];

                gfxImage* dst = ret;
                for (u32 i = 0;i < mipCount && dst;i++) {
                    u32 count = dst->m_width * dst->m_height;
                    fp->read(pixels, count * 3);

                    for (u32 p = 0;p < count;p++) {
                        u32 off = p * 3;
                        dst->m_pixels[p] = {
                            pixels[off + 0],
                            pixels[off + 1],
                            pixels[off + 2],
                            0xFF
                        };
                    }

                    dst = dst->m_nextMipMap;
                }

                delete [] pixels;
                break;
            }
            case RGBA8888: {
                gfxImage* dst = ret;
                for (u32 i = 0;i < mipCount && dst;i++) {
                    fp->read(dst->m_pixels, dst->m_width * dst->m_height * 4);                    
                    dst = dst->m_nextMipMap;
                }
                break;
            }
            case PA4_MC: {
                Pixel palette[16];
                fp->read(palette, 64);

                u8* indices = new u8[(hdr.width * hdr.height) / 2];

                gfxImage* dst = ret;
                    
                u32 x = 0;
                u32 y = 0;
                auto px = [&x, &y, dst, hdr](Pixel& p) {
                    u32 idx = (y * dst->m_width) + x;
                    if (hdr.flags.is_flipped) {
                        idx = (((dst->m_height - y) - 1) * dst->m_width) + x;
                    }

                    dst->m_pixels[idx] = {
                        p.b,
                        p.g,
                        p.r,
                        p.a
                    };

                    x++;
                    if (x >= dst->m_width) {
                        x = 0;
                        y++;
                    }
                };

                for (u32 i = 0;i < mipCount && dst;i++) {
                    u32 count = (dst->m_width * dst->m_height) / 2;
                    fp->read(indices, count);

                    for (u32 p = 0;p < count;p++) {
                        u8 b = indices[p];
                        px(palette[b & 0xf]);
                        px(palette[(b & 0xff) >> 0x4]);
                    }

                    dst = dst->m_nextMipMap;
                }
                
                break;
            }
            default: {
                // must implement whatever format this is
                assert(false);
            }
        }

        if (fp) delete fp;

        return ret;
    }
};