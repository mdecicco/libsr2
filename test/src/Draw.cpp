#include <Draw.h>

#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/creature/crBoneData.h>
#include <libsr2/creature/ragdoll.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/mat4x4f.h>
#include <libsr2/math/math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sr2 {
    Draw* Draw::instance = nullptr;

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
        m_btnDown = false;
        m_dt = 1.0f;
        m_lastTime = 0.0f;
        m_moveSpeed = 10.0f;
        m_moveDamping = 0.93f;
        m_moveAccel = 20.5f;
        m_moveVelocity = { 0.0f, 0.0f, 0.0f };
        for (u32 i = 0;i < 256;i++) m_keyDown[i] = false;

        m_shaderProgram = 0;
        m_vao = 0;
        m_vbo = 0;

        m_view = math::lookAt(
            { -3.0f, 3.0f, -3.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f }
        );
    }

    Draw::~Draw() {
        if (window) {
            glDeleteBuffers(1, &m_vbo);
            glDeleteVertexArrays(1, &m_vao);
            glDeleteProgram(m_shaderProgram);

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

        const char *vs = 
            "#version 330 core\n"
            "layout (location = 0) in vec3 v_position;\n"
            "layout (location = 1) in vec4 v_color;\n"
            "out vec4 o_color;\n"
            "uniform mat4 u_mvp;\n"
            "void main() {\n"
            "   gl_Position = u_mvp * vec4(v_position.x, v_position.y, v_position.z, 1.0);\n"
            "   o_color = v_color;\n"
            "}"
        ;
        const char *fs =
            "#version 330 core\n"
            "in vec4 o_color;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = o_color;\n"
            "}"
        ;
        
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vs, NULL);
        glCompileShader(vertexShader);
        
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fs, NULL);
        glCompileShader(fragmentShader);

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, vertexShader);
        glAttachShader(m_shaderProgram, fragmentShader);
        glLinkProgram(m_shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        return true;
    }

    void Draw::begin() {
        m_dt = glfwGetTime() - m_lastTime;
        m_lastTime = glfwGetTime();

        mat3x3f b = m_view.transposed().basis();

        if (m_keyDown[GLFW_KEY_W]) m_moveVelocity.z += m_moveAccel * m_moveSpeed * m_dt;
        if (m_keyDown[GLFW_KEY_S]) m_moveVelocity.z -= m_moveAccel * m_moveSpeed * m_dt;
        if (m_keyDown[GLFW_KEY_A]) m_moveVelocity.x += m_moveAccel * m_moveSpeed * m_dt;
        if (m_keyDown[GLFW_KEY_D]) m_moveVelocity.x -= m_moveAccel * m_moveSpeed * m_dt;

        m_moveVelocity.x = math::clamp(m_moveVelocity.x, -m_moveSpeed, m_moveSpeed);
        m_moveVelocity.y = math::clamp(m_moveVelocity.y, -m_moveSpeed, m_moveSpeed);
        m_moveVelocity.z = math::clamp(m_moveVelocity.z, -m_moveSpeed, m_moveSpeed);

        m_view.w += m_moveVelocity * m_dt;
        m_moveVelocity *= m_moveDamping;

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_vertices.clear();
    }

    void Draw::end() {
        if (m_vertices.size() > 0) {
            i32 w, h;
            glfwGetWindowSize(window, &w, &h);
            m_projection = math::perspective(d2r(70.0f), f32(w) / f32(h), 0.1f, 100.0f);
            m_mvp = m_projection * m_view;
            glUseProgram(m_shaderProgram);
            glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_mvp"), 1, GL_FALSE, &m_mvp.x.x);

            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(vertex), (void*)sizeof(vec3f));
            
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

            glDrawArrays(GL_LINES, 0, m_vertices.size());

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void Draw::onCursor(f32 x, f32 y) {
        constexpr f32 rotSpeed = 50.0f;
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
        m_moveSpeed += delta * 0.5f;
        m_moveSpeed = math::max(m_moveSpeed, 1.0f);
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
        m_vertices.push_back({
            a, color
        });

        m_vertices.push_back({
            b, color
        });
    }

    void Draw::drawSkeleton(crBoneData* root, ragUnk1* unk, crSkeleton* skel) {
        mat3x4f t = skel->boneTransforms[root->index0];
        t.w -= unk->offset;

        vec3f& pos = t.w;

        crBoneData* b = root->children;
        while (b) {
            line(t.w, skel->boneTransforms[b->index0].w - unk->offset, { 1.0f, 1.0f, 0.0f, 1.0f });
            drawSkeleton(b, unk, skel);
            b = b->next;
        }

        unk->getBound()->debugDraw();
    }
};