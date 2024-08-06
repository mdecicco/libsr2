#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/gfx/gfxImage.h>
#include <libsr2/libsr2.h>

#include <render/vulkan/Texture.h>
#include <render/vulkan/CommandBuffer.h>
#include <render/vulkan/CommandPool.h>
#include <render/vulkan/LogicalDevice.h>
#include <render/vulkan/Queue.h>
#include <render/core/FrameManager.h>

namespace sr2 {
    void NormalizeFilename(char *out, const char *in) {
        long tch;
        char ch;
        
        do {
            ch = *in;
            in = in + 1;
            if ((int)ch - 0x41U < 0x1a) {
                tch = (long)(ch + 0x20);
            }
            else {
                tch = (long)ch;
                if ((long)ch == 0x5c) {
                    tch = 0x2f;
                }
            }
            *out = (char)tch;
            out = out + 1;
        } while (tch != 0);
    }

    gfxTexture* gfxTexture::None = nullptr;
    gfxTexture::gfxTexture() {
        m_refCount = 1;
        m_texture = nullptr;
        m_width = m_height = 0;
        m_texEnv = 0;
    }

    gfxTexture::~gfxTexture() {
    }
    
    gfxTexture* gfxTexture::create(u16 width, u16 height, ImageFormat fmt) {
        gfxTexture* ret = new gfxTexture();
        ret->m_width = width;
        ret->m_height = height;
        return ret;
    }

    gfxTexture* gfxTexture::create(u16 width, u16 height, ImageFormat fmt, u32 mipCount) {
        gfxTexture* ret = new gfxTexture();
        ret->m_width = width;
        ret->m_height = height;

        // todo...

        return ret;
    }

    gfxTexture* gfxTexture::create(gfxImage* img, bool p2) {
        u32 mipCount = 0;
        if (p2) {
            gfxImage* mip = img;
            while (mip->getNextMipMap()) {
                mipCount++;
                mip = mip->getNextMipMap();
            }
        }

        vec2i dims = img->getDimensions();
        gfxTexture* tex = create(dims.x, dims.y, img->getFormat(), mipCount);
        if (!tex) return nullptr;

        tex->load(img);
        tex->setTexEnv(tex->m_texEnv | img->getTexEnv());

        return tex;
    }

    gfxTexture* gfxTexture::get(const char* name, i32 unk0, i32 unk1) {
        // todo: this is not at all how this works
        
        char normalizedFilename[128] = { 0 };
        NormalizeFilename(normalizedFilename, name);

        gfxImage* img = gfxImage::LoadTEX(normalizedFilename, unk0);
        if (img) {
            return gfxTexture::create(img, unk0);
        }

        return gfxTexture::None;
    }

    void gfxTexture::getMovie(gfxTexture** out, const char* name, i32 unk0) {
        *out = gfxTexture::None;
    }

    void gfxTexture::release(gfxTexture* tex) {
        if (!tex) return;
        tex->m_refCount--;
        if (tex->m_refCount == 0) delete tex;
    }


    void gfxTexture::addRef() {
        m_refCount++;
    }

    void gfxTexture::setTexEnv(u32 env) {
        m_texEnv = env;
    }

    u32 gfxTexture::getTexEnv() {
        return m_texEnv;
    }
    
    void gfxTexture::load(gfxImage* src) {
        GameEngine* eng = GameEngine::Instance();
        m_texture = new render::vulkan::Texture(eng->getLogicalDevice());

        if (!m_texture->init(m_width, m_height, VK_FORMAT_R8G8B8A8_SRGB)) {
            delete m_texture;
            m_texture = nullptr;
            return;
        }

        if (!m_texture->initStagingBuffer()) {
            delete m_texture;
            m_texture = nullptr;
            return;
        }

        if (!m_texture->initSampler()) {
            delete m_texture;
            m_texture = nullptr;
            return;
        }

        render::vulkan::CommandBuffer* cb = eng->getFrameManager()->getCommandPool()->createBuffer(true);
        if (!cb->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)) {
            eng->getFrameManager()->getCommandPool()->freeBuffer(cb);
            delete m_texture;
            m_texture = nullptr;
            return;
        }

        gfxImage::Pixel* data = (gfxImage::Pixel*)m_texture->getStagingBuffer()->getPointer();
        memset(data, 0, m_width * m_height * sizeof(gfxImage::Pixel));

        gfxImage::Pixel* pixels = src->getPixels();
        u32 pixelCount = m_width * m_height;

        for (u32 x = 0;x < m_width;x++) {
            for (u32 y = 0;y < m_height;y++) {
                data[m_width * ((m_height - y) - 1) + x] = pixels[m_width * y + x];
            }
        }
        
        if (!m_texture->setLayout(cb, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
            eng->getFrameManager()->getCommandPool()->freeBuffer(cb);
            delete m_texture;
            m_texture = nullptr;
        }

        m_texture->flushPixels(cb);

        if (!m_texture->setLayout(cb, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
            eng->getFrameManager()->getCommandPool()->freeBuffer(cb);
            delete m_texture;
            m_texture = nullptr;
        }

        if (!cb->end()) {
            eng->getFrameManager()->getCommandPool()->freeBuffer(cb);
            delete m_texture;
            m_texture = nullptr;
            return;
        }

        if (!eng->getLogicalDevice()->getGraphicsQueue()->submit(cb)) {
            eng->getFrameManager()->getCommandPool()->freeBuffer(cb);
            delete m_texture;
            m_texture = nullptr;
            return;
		}

        eng->getLogicalDevice()->getGraphicsQueue()->waitForIdle();
        m_texture->shutdownStagingBuffer();
        
        eng->getFrameManager()->getCommandPool()->freeBuffer(cb);
    }

    vec2i gfxTexture::getDimensions() {
        return { m_width, m_height };
    }
    
    void gfxTexture::setTex(render::vulkan::Texture* tex) {
        if (m_texture) delete m_texture;
        m_texture = tex;
    }

    render::vulkan::Texture* gfxTexture::getTex() {
        return m_texture;
    }
};