#include <libsr2/utilities/DebugDrawer.h>
#include <vector>

struct GLFWwindow;

namespace sr2 {
    class crBoneData;
    class ragUnk1;
    class crSkeleton;

    #pragma pack(push, 1)
    struct vertex {
        vec3f position;
        vec4f color;
    };
    #pragma pack(pop)

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
        
        protected:
            mat4x4f m_projection;
            mat4x4f m_view;
            mat4x4f m_mvp;
            bool m_btnDown;
            bool m_keyDown[256];
            vec2f m_cursor;
            f32 m_dt;
            f32 m_lastTime;
            f32 m_moveSpeed;
            f32 m_moveDamping;
            f32 m_moveAccel;
            vec3f m_moveVelocity;
            i32 m_shaderProgram;
            u32 m_vbo;
            u32 m_vao;
            std::vector<vertex> m_vertices;
        
        private:
            Draw();
            ~Draw();

            static Draw* instance;
    };
}