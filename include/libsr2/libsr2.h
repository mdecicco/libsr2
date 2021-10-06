namespace sr2 {
    class gameFSM;
    class GameEngine {
        public:
            static void Create(int arg_count, const char** args);
            static void Destroy();
            static GameEngine* Instance();

            void BeginFrame();
            void EndFrame();

            void Run();
            bool Update();

            gameFSM* fsm;
            bool just_update;
            bool should_pause;
            bool should_exit;
            i32 frame_count;

        protected:
            GameEngine(int arg_count, const char** args);
            ~GameEngine();
            static GameEngine* instance;
    };
};