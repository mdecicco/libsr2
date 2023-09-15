#include <Draw.h>

#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/creature/crBoneData.h>
#include <libsr2/creature/ragdoll.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/mat4x4f.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLAnnotate.hpp>

namespace sr2 {
    Draw* Draw::instance = nullptr;
    f32 rotSpeed = 50.0f;

    f32 d2r(f32 degrees) {
        return degrees * 0.0174533f;
    }

    void scroll_cb(GLFWwindow* w, f64 xOff, f64 yOff) {
        Draw::get()->onScroll(yOff);
    }

    void click_cb(GLFWwindow* w, i32 button, i32 action, i32 mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) Draw::get()->onMouseDown();
            else if (action == GLFW_RELEASE) Draw::get()->onMouseUp();
        }
    }

    void key_cb(GLFWwindow* w, i32 key, i32 scancode, i32 action, i32 mods) {
        if (action == GLFW_PRESS) Draw::get()->onKeyDown(key);
        else if (action == GLFW_RELEASE) Draw::get()->onKeyUp(key);
    }

    void move_cb(GLFWwindow* wnd, f64 x, f64 y) {
        Draw::get()->onCursor(x, y);
    }




    Draw::Draw() {
        window = nullptr;
        gfx = nullptr;
        m_btnDown = false;
        m_dt = 1.0f;
        m_lastTime = 0.0f;
        for (u32 i = 0;i < 256;i++) m_keyDown[i] = false;

        m_view = math::lookAt(
            { 3.0f, 3.0f, 3.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f }
        );
    }

    Draw::~Draw() {
        if (window) {
            delete gfx;
            gfx = nullptr;

            glfwDestroyWindow(window);
            glfwTerminate();
            window = nullptr;
        }
    }

    Draw* Draw::get() {
        if (!instance) instance = new Draw();
        return instance;
    }

    void Draw::destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }

    bool Draw::init() {
        if (!glfwInit()) return false;
    
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(800, 600, "libsr2 testing", nullptr, nullptr);
        if (!window) return false;

        glfwMakeContextCurrent(window);

        glfwSetScrollCallback(window, scroll_cb);
        glfwSetMouseButtonCallback(window, click_cb);
        glfwSetCursorPosCallback(window, move_cb);
        glfwSetKeyCallback(window, key_cb);
    
        glfwSwapInterval(1);
    
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(window);
            glfwTerminate();
            window = nullptr;
            return false;
        }

        gfx = new gla::GLAnnotate();
        if (gfx->init()) {
            delete gfx;
            gfx = nullptr;

            glfwDestroyWindow(window);
            glfwTerminate();
            window = nullptr;
            return false;
        }

        return true;
    }

    void Draw::begin() {
        m_dt = glfwGetTime() - m_lastTime;
        m_lastTime = glfwGetTime();

        mat3x3f b = m_view.transposed().basis();

        if (m_keyDown[GLFW_KEY_W]) m_view.w.z += m_dt;
        if (m_keyDown[GLFW_KEY_S]) m_view.w.z += m_dt * -1.0f;
        if (m_keyDown[GLFW_KEY_A]) m_view.w.x += m_dt;
        if (m_keyDown[GLFW_KEY_D]) m_view.w.x += m_dt * -1.0f;

        i32 w, h;
        glfwGetWindowSize(window, &w, &h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        m_projection = math::perspective(d2r(70.0f), f32(w) / f32(h), 0.1f, 100.0f);
        gfx->setProjection(&m_projection.x.x);
        gfx->setTransform(&m_view.x.x);
    }

    void Draw::end() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void Draw::onCursor(f32 x, f32 y) {
        vec2f cur = { f32(x), f32(y) };
        
        if (m_btnDown) {
            i32 w, h;
            glfwGetWindowSize(window, &w, &h);

            vec2f delta = {
                cur.x - m_cursor.x,
                cur.y - m_cursor.y
            };

            vec2f scale = {
                fabsf(delta.x) / f32(w),
                fabsf(delta.y) / f32(h)
            };

            vec3f xAxis = vec3f(1.0f, 0.0f, 0.0f);
            vec3f yAxis = m_view.basis() * vec3f(0.0f, 1.0f, 0.0f);

            if (delta.x < 0.0f) {
                mat3x4f r;
                math::rotation_axis_angle(r, yAxis, d2r(rotSpeed * scale.x));
                m_view *= r;
            } else if (delta.x > 0.0f) {
                mat3x4f r;
                math::rotation_axis_angle(r, yAxis, d2r(-rotSpeed * scale.x));
                m_view *= r;
            }

            if (delta.y < 0.0f) {
                mat3x4f r;
                math::rotation_axis_angle(r, xAxis, d2r(rotSpeed * scale.y));
                m_view *= r;
            } else if (delta.y > 0.0f) {
                mat3x4f r;
                math::rotation_axis_angle(r, xAxis, d2r(-rotSpeed * scale.y));
                m_view *= r;
            }
        }

        m_cursor = cur;
    }

    void Draw::onScroll(f32 delta) {

    }

    void Draw::onMouseDown() {
        m_btnDown = true;
    }

    void Draw::onMouseUp() {
        m_btnDown = false;
    }

    void Draw::onKeyDown(i32 key) {
        m_keyDown[key] = true;
    }

    void Draw::onKeyUp(i32 key) {
        m_keyDown[key] = false;
    }

    bool Draw::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    f32 Draw::getDeltaTime() {
        return m_dt;
    }

    void Draw::line(const vec3f& a, const vec3f& b, const vec4f& color) {
        gfx->setColor(color.x, color.y, color.z, color.w);
        gfx->line(a.x, a.y, a.z, b.x, b.y, b.z);
    }

    void Draw::drawSkeleton(crBoneData* root, ragUnk1* unk, crSkeleton* skel) {
        phBoundComposite* bound = unk->getBound();

        sphere(0.03f, skel->boneTransforms[root->index0]);

        vec3f& pos = skel->boneTransforms[root->index0].w;

        crBoneData* b = root->children;
        while (b) {
            line(pos, skel->boneTransforms[b->index0].w, { 1.0f, 1.0f, 0.0f, 1.0f });
            drawSkeleton(b, unk, skel);
            b = b->next;
        }
    }
};