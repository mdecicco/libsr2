#pragma once
#include <libsr2/types.h>

namespace render {
    namespace vulkan {
        class Texture;
    };
};

namespace sr2 {
    class gfxImage;

    class gfxTexture {
        public:
            static gfxTexture* create(u16 width, u16 height, ImageFormat fmt);
            static gfxTexture* create(u16 width, u16 height, ImageFormat fmt, u32 mipCount);
            static gfxTexture* create(gfxImage* img, bool p2);
            static gfxTexture* get(const char* name, i32 unk0, i32 unk1);
            static void getMovie(gfxTexture** out, const char* name, i32 unk0);
            static void release(gfxTexture* tex);
            static gfxTexture* None;

            void addRef();
            void setTexEnv(u32 env);
            u32 getTexEnv();
            void load(gfxImage* src);
            vec2i getDimensions();
            void setTex(render::vulkan::Texture* tex);
            render::vulkan::Texture* getTex();

        protected:
            gfxTexture();
            ~gfxTexture();

            u16 m_width;
            u16 m_height;
            u32 m_texEnv;
            u32 m_refCount;
            render::vulkan::Texture* m_texture;
    };
};