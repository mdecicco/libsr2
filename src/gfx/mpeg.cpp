#include <libsr2/gfx/mpeg.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/audio/audPlayer.h>
#include <libsr2/io/stream.h>
#include <libsr2/libsr2.h>

extern "C" {
    #include <mpeg2convert.h>
};

#include <render/vulkan/Texture.h>
#include <render/vulkan/Buffer.h>
#include <render/vulkan/VertexBuffer.h>
#include <render/vulkan/DescriptorSet.h>
#include <render/vulkan/CommandPool.h>
#include <render/vulkan/CommandBuffer.h>
#include <render/vulkan/LogicalDevice.h>
#include <render/vulkan/Queue.h>
#include <render/core/FrameManager.h>
#include <utils/Window.h>
#include <utils/Array.hpp>
#include <utils/Timer.h>

#include <string.h>


namespace sr2 {
    mpeg::mpeg() {
        m_dimensions = { -1, -1 };
        m_texture = nullptr;
        m_frameRate = 0.0f;
        m_frameIdx = 0;
        m_videoBufferOffset = 0;
        m_mpegDecoder = nullptr;
        m_mpegInfo = nullptr;
        m_videoData = nullptr;
        m_audioData = nullptr;
        m_audioSrc = nullptr;
    }

    mpeg::~mpeg() {
        if (m_texture) destroyTexture();
        if (m_mpegDecoder) {
            mpeg2_close(m_mpegDecoder);
            m_mpegDecoder = nullptr;
            m_mpegInfo = nullptr;
        }

        if (m_videoData) delete [] m_videoData;
        if (m_audioData) delete [] m_audioData;
        if (m_audioSrc) delete m_audioSrc;
    }

    bool mpeg::load(const char* file, i32 p2) {
        bool isM2V = strstr(file, ".m2v") != nullptr;
        bool isIMF = !isM2V && strstr(file, ".imf") != nullptr;
        if (!isM2V && !isIMF) return false;

        if (isM2V && !loadM2V(file, p2)) return false;
        else if (isIMF && !loadIMF(file, p2)) return false;

        if (m_audioData) {
            m_audioSrc = new audPlayer();
            m_audioSrc->buffer(asf_stereo_16bit, m_audioData, m_audioDataSize, 48000);
        }

        createTexture();
        getNextFrame(false);

        return true;
    }

    bool mpeg::loadM2V(const char* file, i32 p2) {
        Stream* fp = datAssetManager::open("vid/ntsc", file, "", 0, true);
        if (!fp) return false;
        
        m_videoDataSize = fp->getSize();
        m_videoData = new u8[m_videoDataSize];

        if (!fp->read(m_videoData, m_videoDataSize)) {
            delete [] m_videoData;
            delete fp;
            fp = nullptr;
            m_videoData = nullptr;
            return false;
        }
        
        delete fp;
        fp = nullptr;

        getInfo();

        return true;
    }

    bool mpeg::loadIMF(const char* file, i32 p2) {
        Stream* fp = datAssetManager::open("vid/ntsc", file, "", 0, true);
        if (!fp) return false;

        #pragma pack(push, 1)
        struct IMF_HEADER {
            u32 magic;
            u32 unk2Count;
            u32 blockCount;
            u32 unk1;
        } header;
        #pragma pack(pop)

        if (fp->read(&header, sizeof(IMF_HEADER)) != sizeof(IMF_HEADER)) {
            delete fp;
            fp = nullptr;
            return false;
        }

        if (header.magic != 0x31464d49) {
            delete fp;
            fp = nullptr;
            return false;
        }

        u8 unk2[2032] = { 0 };

        if (fp->read(unk2, header.unk2Count) != header.unk2Count) {
            delete fp;
            fp = nullptr;
            return false;
        }

        m_videoDataSize = header.unk1 * header.unk2Count;
        m_audioDataSize = 0;
        for (u32 i = 0;i < header.unk2Count;i++) {
            m_audioDataSize += unk2[i] * header.blockCount;
        }

        m_videoData = new u8[m_videoDataSize];
        m_audioData = new i16[m_audioDataSize / sizeof(i16)];

        i16* audioBuf = new i16[header.blockCount / sizeof(i16)];

        u32 blockOffset = 2048;
        u32 videoReadOffset = 0;
        u32 cByte = 0;
        bool isLeft = true;
        u32 leftSampleIdx = 0;
        u32 rightSampleIdx = 0;
        for (u32 i = 0;i < header.unk2Count;i++) {
            fp->seek(blockOffset);

            for (u32 b = 0;b < unk2[i];b++) {
                fp->read(audioBuf, header.blockCount);

                u32 sampleCount = header.blockCount / sizeof(i16);
                for (u32 r = 0;r < sampleCount;r++) {
                    if (isLeft) {
                        m_audioData[leftSampleIdx * 2] = audioBuf[r];
                        leftSampleIdx++;
                    } else {
                        m_audioData[(rightSampleIdx * 2) + 1] = audioBuf[r];
                        rightSampleIdx++;
                    }

                    cByte += sizeof(i16);
                    if (cByte == 512) {
                        cByte = 0;
                        isLeft = !isLeft;
                    }
                }

                blockOffset += header.blockCount;
            }
            
            fp->read(m_videoData + videoReadOffset, header.unk1);
            videoReadOffset += header.unk1;
            
            blockOffset += header.unk1;
        }

        delete [] audioBuf;

        getInfo();

        return true;
    }

    bool mpeg::getInfo() {
        m_mpegDecoder = mpeg2_init();
        m_mpegInfo = mpeg2_info(m_mpegDecoder);

        u32 bufPos = 0;
        while (true) {
            mpeg2_state_t state = mpeg2_parse(m_mpegDecoder);
            switch (state) {
                case STATE_BUFFER: {
                    mpeg2_buffer(m_mpegDecoder, m_videoData + bufPos, m_videoData + bufPos + 4096);
                    bufPos += 4096;
                    if (bufPos >= m_videoDataSize) break;
                    break;
                }
                case STATE_SEQUENCE: {
                    m_dimensions = { i32(m_mpegInfo->sequence->width), i32(m_mpegInfo->sequence->height) };
                    m_frameRate = f32((uint64_t)1001000000 * 27 / m_mpegInfo->sequence->frame_period) / 1001.0f;
                    mpeg2_reset(m_mpegDecoder, true);
                    return true;
                }
                default: break;
            }
        }

        mpeg2_reset(m_mpegDecoder, true);
        return false;
    }

    void mpeg::updateTexture() {
        if (!m_texture) return;

        // todo: don't wait for gpu...? don't recreate texture...?

        GameEngine* eng = GameEngine::Instance();

        render::vulkan::CommandBuffer* cb = eng->getFrameManager()->getCommandPool()->createBuffer(true);
        render::vulkan::Texture* tex = new render::vulkan::Texture(eng->getLogicalDevice());
        tex->init(m_dimensions.x, m_dimensions.y, VkFormat::VK_FORMAT_R8G8B8A8_SRGB);
        tex->initSampler();
        tex->initStagingBuffer();
    
        render::vulkan::Buffer* pb = tex->getStagingBuffer();
        pb->map();
        cb->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        tex->setLayout(cb, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        u32 readPos = 0;
        u8* dest = (u8*)pb->getPointer();

        for (u32 x = 0;x < m_dimensions.x;x++) {
            for (u32 y = 0;y < m_dimensions.y;y++) {
                u32 idx = (y * m_dimensions.x * 3) + (x * 3);
                u8 r = m_mpegInfo->display_fbuf->buf[0][idx + 0];
                u8 g = m_mpegInfo->display_fbuf->buf[0][idx + 1];
                u8 b = m_mpegInfo->display_fbuf->buf[0][idx + 2];
                u8 a = 0xFF;

                idx = (y * m_dimensions.x * 4) + (x * 4);
                dest[idx + 0] = r;
                dest[idx + 1] = g;
                dest[idx + 2] = b;
                dest[idx + 3] = a;
            }
        }

        tex->flushPixels(cb);
        tex->setLayout(cb, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        cb->end();
        eng->getLogicalDevice()->getGraphicsQueue()->submit(cb);
        eng->getLogicalDevice()->waitForIdle();
        pb->unmap();
        eng->getFrameManager()->getCommandPool()->freeBuffer(cb);

        m_texture->setTex(tex);
    }

    gfxTexture* mpeg::createTexture() {
        if (m_texture) return m_texture;

        m_texture = gfxTexture::create(m_dimensions.x, m_dimensions.y, ImageFormat::RGBA8888);

        return m_texture;
    }

    void mpeg::destroyTexture() {
        if (!m_texture) return;
        gfxTexture::release(m_texture);
        m_texture = nullptr;
    }

    i32 mpeg::getNextFrame(bool maybeDoLoop) {
        // todo: return code meaning?
        i32 result = 1;
        u32 desiredFrameIdx = m_elapsedTime.elapsed() * m_frameRate;

        if (m_videoBufferOffset >= m_videoDataSize) {
            if (!maybeDoLoop) return 3;
            result = 2;
            reset();
            start();
        } else if (m_frameIdx > 0 && m_frameIdx == desiredFrameIdx) {
            return 0;
        }

        u32 frameCount = 0;
        if (desiredFrameIdx > m_frameIdx) frameCount = desiredFrameIdx - m_frameIdx;
        if (frameCount == 0 && m_frameIdx == 0) frameCount = 1;
        
        for (u32 f = 0;f < frameCount;f++) {
            bool skipFrame = f < frameCount - 1;
            bool done = false;

            while (!done) {
                if (m_videoBufferOffset >= m_videoDataSize) {
                    if (!maybeDoLoop) return 3;
                    result = 2;
                    reset();
                    start();
                }

                mpeg2_state_t state = mpeg2_parse(m_mpegDecoder);
                switch (state) {
                    case STATE_BUFFER: {
                        u32 endOffset = m_videoBufferOffset + 4096;
                        if (endOffset >= m_videoDataSize) endOffset = m_videoDataSize;

                        mpeg2_buffer(m_mpegDecoder, m_videoData + m_videoBufferOffset, m_videoData + endOffset);
                        m_videoBufferOffset += endOffset - m_videoBufferOffset;
                        break;
                    }
                    case STATE_SEQUENCE: {
                        if (!skipFrame) mpeg2_convert(m_mpegDecoder, mpeg2convert_rgb24, nullptr);
                        break;
                    }
                    case STATE_SLICE:
                    case STATE_END:
                    case STATE_INVALID_END: {
                        if (!m_mpegInfo->display_fbuf) continue;

                        m_frameIdx++;
                        done = true;

                        if (!skipFrame) {
                            updateTexture();
                            return result;
                        }
                    }
                    default: break;
                }
            }
        }

        return 0;
    }

    void mpeg::start() {
        m_frameTimer.start();
        m_elapsedTime.start();
        if (m_audioSrc) m_audioSrc->play();
    }

    void mpeg::shutdown() {
        if (m_texture) destroyTexture();
        if (m_mpegDecoder) {
            mpeg2_close(m_mpegDecoder);
            m_mpegDecoder = nullptr;
            m_mpegInfo = nullptr;
        }

        if (m_videoData) delete [] m_videoData;
        m_videoData = nullptr;
        m_videoDataSize = 0;
        if (m_audioData) delete [] m_audioData;
        m_audioData = nullptr;
        m_audioDataSize = 0;
        if (m_audioSrc) {
            m_audioSrc->stop();
            delete m_audioSrc;
        }
        m_audioSrc = nullptr;
        m_dimensions = { 0, 0 };
        m_frameRate = 0;
        m_frameIdx = 0;
        m_videoBufferOffset = 0;
        m_frameTimer.reset();
        m_elapsedTime.reset();
    }

    void mpeg::reset() {
        mpeg2_reset(m_mpegDecoder, true);
        m_videoBufferOffset = 0;
        m_frameIdx = 0;
        m_frameTimer.reset();
        m_elapsedTime.reset();
        if (m_audioSrc) m_audioSrc->stop();
    }

    vec2i mpeg::getDimensions() {
        return m_dimensions;
    }

    f32 mpeg::getFrameRate() {
        return m_frameRate;
    }
};