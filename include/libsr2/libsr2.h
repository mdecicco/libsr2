#include <libsr2/types.h>
#include <libsr2/utilities/msgMsgSink.h>

#include <render/IWithRendering.h>
#include <render/core/FrameContext.h>

namespace render {
    namespace vulkan {
        class GraphicsPipeline;
    };
};

namespace sr2 {
    class gameFSM;
    enum class GAME_STATE : u32;

    extern u64 g_HostTimer;
    extern u64 g_FrameTimer;
    extern bool g_showFrameTime;

    class GameEngine : public msgMsgSink, public render::IWithRendering {
        public:
            // 0x001007c8
            static GameEngine* Create(int argc, char** args);

            // 0x001004a8
            static void Terminate();

            // 0x00100928
            static void Execute();

            // 0x001008d8
            static GameEngine* Instance();

            // 0x002fe220
            void BeginFrame(bool doClear);

            // 0x002fe280
            void EndFrame();

            // 0x00100e78
            bool Update();

            // 0x00100c70
            void ChangeState(GAME_STATE state);
    
            virtual void onMessage(msgMessage* msg);

            // From IWithRendering
            virtual bool setupInstance(render::vulkan::Instance* instance);
            virtual void onLogMessage(utils::LOG_LEVEL level, const utils::String& scope, const utils::String& message);

            render::core::FrameContext* currentFrame();


            gameFSM* fsm;
            bool just_update;
            bool should_pause;
            bool should_exit;
            i32 frame_count;

        protected:
            GameEngine(int argc, char** args);
            ~GameEngine();
            static GameEngine* instance;

            utils::Window* m_window;
            render::core::FrameContext* m_currentFrame;
    };
};