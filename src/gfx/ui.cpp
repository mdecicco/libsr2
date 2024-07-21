#include <libsr2/gfx/ui.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/libsr2.h>
#include <libsr2/globals.h>

#include <render/vulkan/GraphicsPipeline.h>
#include <render/vulkan/Texture.h>
#include <render/vulkan/CommandBuffer.h>
#include <render/vulkan/CommandPool.h>
#include <render/vulkan/LogicalDevice.h>
#include <render/vulkan/Queue.h>
#include <render/vulkan/VertexBuffer.h>
#include <render/vulkan/DescriptorSet.h>
#include <render/vulkan/UniformBuffer.h>
#include <render/core/FrameManager.h>
#include <render/core/FrameContext.h>

#include <utils/Singleton.hpp>
#include <utils/Array.hpp>

using namespace render;
using namespace vulkan;

namespace sr2 {
    uiRenderer::uiRenderer() {
        m_pipeline = nullptr;
        m_globalUniforms = nullptr;
    }

    uiRenderer::~uiRenderer() {
        shutdown();
    }

    bool uiRenderer::init(GameEngine* eng) {
        m_pipeline = new GraphicsPipeline(
            eng->getShaderCompiler(),
            eng->getLogicalDevice(),
            eng->getSwapChain(),
            eng->getRenderPass()
        );

        const char* vsh =
            "layout (location = 0) in vec3 v_pos;\n"
            "layout (location = 1) in vec2 v_tex;\n"
            "layout (location = 2) in vec4 v_color;\n"
            "layout (binding = 0) uniform _ubo {\n"
            "    mat4 projection;\n"
            "    mat4 view;\n"
            "    mat4 viewProj;\n"
            "} ubo;\n"
            "\n"
            "layout (location = 0) out vec2 a_tex;\n"
            "layout (location = 1) out vec4 a_color;\n"
            "\n"
            "void main() {\n"
            "  gl_Position = ubo.viewProj * vec4(v_pos, 1.0);\n"
            "  a_tex = v_tex;\n"
            "  a_color = v_color;\n"
            "}\n"
        ;
        const char* fsh =
            "layout (location = 0) in vec2 a_tex;\n"
            "layout (location = 1) in vec4 a_color;\n"
            "layout (binding = 1) uniform sampler2D s_tex;\n"
            "\n"
            "layout (location = 0) out vec4 o_color;\n"
            "\n"
            "void main() {\n"
            "    o_color = texture(s_tex, a_tex) * a_color;\n"
            "}\n"
        ;
            
        m_vfmt.addAttr(&vertex::position);
        m_vfmt.addAttr(&vertex::uv);
        m_vfmt.addAttr(&vertex::color);
        m_pipeline->setVertexFormat(&m_vfmt);

        m_ufmt.addAttr(&global_uniforms::projection);
        m_ufmt.addAttr(&global_uniforms::view);
        m_ufmt.addAttr(&global_uniforms::viewProj);
        m_pipeline->addUniformBlock(0, &m_ufmt, VK_SHADER_STAGE_VERTEX_BIT);
        m_pipeline->addSampler(1, VK_SHADER_STAGE_FRAGMENT_BIT);
        
        m_pipeline->addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
        m_pipeline->addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
        m_pipeline->setPrimitiveType(PT_TRIANGLES);
        m_pipeline->setDepthTestEnabled(true);
        m_pipeline->setDepthCompareOp(COMPARE_OP::CO_LESS_OR_EQUAL);
        m_pipeline->setDepthWriteEnabled(true);
        m_pipeline->setColorBlendEnabled(true);
        m_pipeline->setColorBlendOp(BLEND_OP::BO_ADD);
        m_pipeline->setAlphaBlendOp(BLEND_OP::BO_ADD);
        m_pipeline->setSrcColorBlendFactor(BLEND_FACTOR::BF_SRC_ALPHA);
        m_pipeline->setDstColorBlendFactor(BLEND_FACTOR::BF_ONE_MINUS_SRC_ALPHA);
        m_pipeline->setSrcAlphaBlendFactor(BLEND_FACTOR::BF_ONE);
        m_pipeline->setDstAlphaBlendFactor(BLEND_FACTOR::BF_ZERO);

        if (!m_pipeline->setVertexShader(vsh)) return false;
        if (!m_pipeline->setFragmentShader(fsh)) return false;
        if (!m_pipeline->init()) return false;

        m_globalUniforms = eng->allocateUniformObject(&m_ufmt);
        if (!m_globalUniforms) return false;
        
        f32 screenWidth = 640 * debug_ui_scale;
        f32 screenHeight = 480 * debug_ui_scale;

        render::vulkan::CommandBuffer* cb = eng->getFrameManager()->getCommandPool()->createBuffer(true);
        if (!cb->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)) {
            eng->getFrameManager()->getCommandPool()->freeBuffer(cb);
            return false;
        }

        global_uniforms u;
        u.projection = mat4f::Orthographic(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
        u.view = mat4f();
        u.viewProj = u.projection;
        m_globalUniforms->set(u);
        m_globalUniforms->getBuffer()->submitUpdates(cb);
        cb->end();

        eng->getLogicalDevice()->getGraphicsQueue()->submit(cb);
        eng->getLogicalDevice()->waitForIdle();
        eng->getFrameManager()->getCommandPool()->freeBuffer(cb);

        return true;
    }

    bool uiRenderer::shutdown() {
        if (m_globalUniforms) {
            m_globalUniforms->free();
            m_globalUniforms = nullptr;
        }

        if (m_pipeline) {
            m_pipeline->shutdown();
            delete m_pipeline;
            m_pipeline = nullptr;
        }

        return true;
    }

    uiRenderer::element* uiRenderer::createElement(u32 vertexCount) {
        GameEngine* eng = GameEngine::Instance();
        uiRenderer* self = ::utils::Singleton<uiRenderer>::Get();
        
        render::vulkan::Vertices* verts = eng->allocateVertices(&self->m_vfmt, vertexCount);
        if (!verts) return nullptr;

        render::vulkan::DescriptorSet* desc = eng->allocateDescriptor(self->m_pipeline);
        if (!desc) return nullptr;
        
        desc->add(self->m_globalUniforms, 0);

        uiRenderer::element* elem = new uiRenderer::element({ verts, desc, vertexCount, false });
        self->m_elements.push(elem);

        return elem;
    }

    void uiRenderer::destroyElement(uiRenderer::element* ele) {
        if (!ele) return;

        uiRenderer* self = ::utils::Singleton<uiRenderer>::Get();
        for (u32 i = 0;i < self->m_elements.size();i++) {
            if (self->m_elements[i] == ele) {
                self->m_elements.remove(i);
                break;
            }
        }

        ele->vertices->free();
        ele->descriptor->free();
        delete ele;
    }

    void uiRenderer::draw(render::core::FrameContext* frame) {
        f32 screenWidth = 640 * debug_ui_scale;
        f32 screenHeight = 480 * debug_ui_scale;

        GameEngine* eng = GameEngine::Instance();
        uiRenderer* self = ::utils::Singleton<uiRenderer>::Get();
        auto cb = frame->getCommandBuffer();

        cb->bindPipeline(self->m_pipeline, VK_PIPELINE_BIND_POINT_GRAPHICS);
        cb->setViewport(0, screenHeight, screenWidth, -screenHeight, 0, 1);
        cb->setScissor(0, 0, screenWidth, screenHeight);

        for (uiRenderer::element* ele: self->m_elements) {
            if (!ele->doRender) continue;
            cb->bindVertexBuffer(ele->vertices->getBuffer());
            cb->bindDescriptorSet(ele->descriptor, VK_PIPELINE_BIND_POINT_GRAPHICS);
            cb->draw(ele->vertexCount, ele->vertices->getOffset());
            ele->doRender = false;
        }
    }
};