#include <libsr2/types.h>

namespace sr2 {
    class gameFSM;
    enum GAME_STATE;

    extern u64 g_HostTimer;
    extern u64 g_FrameTimer;
    extern bool g_showFrameTime;

    class GameEngine {
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
            void BeginFrame();

            // 0x002fe280
            void EndFrame();

            // 0x00100e78
            bool Update();

            // 0x00100c70
            void ChangeState(GAME_STATE state);

            gameFSM* fsm;
            bool just_update;
            bool should_pause;
            bool should_exit;
            i32 frame_count;

        protected:
            GameEngine(int argc, char** args);
            ~GameEngine();
            static GameEngine* instance;
    };
};