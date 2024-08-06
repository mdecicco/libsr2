#include <libsr2/types.h>

#include <utils/Thread.h>
#include <utils/Array.h>
#include <utils/Timer.h>

extern "C" {
    #include <config.h>
    #include <mpeg2.h>
};

namespace sr2 {
    class gfxTexture;
    class gfxImage;
    class Stream;
    class audPlayer;

    class mpeg {
        public:
            mpeg();
            ~mpeg();

            bool load(const char* file, i32 p2);
            gfxTexture* createTexture();
            void destroyTexture();
            i32 getNextFrame(bool p1);
            void start();
            void shutdown();
            void reset();
            vec2i getDimensions();
            f32 getFrameRate();
        
        protected:
            bool loadM2V(const char* file, i32 p2);
            bool loadIMF(const char* file, i32 p2);

            bool getInfo();
            void updateTexture();

            vec2i m_dimensions;
            f32 m_frameRate;
            u32 m_frameIdx;
            utils::Timer m_frameTimer;
            utils::Timer m_elapsedTime;

            gfxTexture* m_texture;
            i16* m_audioData;
            u32 m_audioDataSize;
            u8* m_videoData;
            u32 m_videoDataSize;

            u32 m_videoBufferOffset;
            mpeg2dec_t* m_mpegDecoder;
            const mpeg2_info_t* m_mpegInfo;

            audPlayer* m_audioSrc;
    };
};