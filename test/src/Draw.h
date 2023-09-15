#include <libsr2/utilities/DebugDrawer.h>

struct GLFWwindow;

namespace gla {
    class GLAnnotate;
};

namespace sr2 {
    class crBoneData;
    class ragUnk1;
    class crSkeleton;

    class Draw : public IDebugDrawer {
        public:
            bool init();
            void begin();
            void end();
            void onCursor(f32 x, f32 y);
            void onScroll(f32 delta);
            void onMouseDown();
            void onMouseUp();
            void onKeyDown(i32 key);
            void onKeyUp(i32 key);
            bool shouldClose();
            f32 getDeltaTime();

            virtual void line(const vec3f& a, const vec3f& b, const vec4f& color);
            void drawSkeleton(crBoneData* root, ragUnk1* unk, crSkeleton* skel);

            static Draw* get();
            static void destroy();
        
            GLFWwindow* window;
            gla::GLAnnotate* gfx;
        
        protected:
            mat4x4f m_projection;
            mat4x4f m_view;
            bool m_btnDown;
            bool m_keyDown[256];
            vec2f m_cursor;
            f32 m_dt;
            f32 m_lastTime;
        
        private:
            Draw();
            ~Draw();

            static Draw* instance;
    };
}