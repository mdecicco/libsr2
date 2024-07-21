#pragma once
#include <libsr2/types.h>

#include <utils/Math.hpp>
#include <utils/Array.h>
#include <utils/robin_hood.h>
#include <render/core/DataFormat.h>

namespace render {
    namespace vulkan {
        class GraphicsPipeline;
        class Vertices;
        class UniformObject;
        class DescriptorSet;
    };

    namespace core {
        class FrameContext;
    }
};

namespace sr2 {
    class GameEngine;
    
    class uiRenderer {
        public:
            struct global_uniforms {
                ::utils::mat4f projection;
                ::utils::mat4f view;
                ::utils::mat4f viewProj;
            };

            struct vertex {
                ::utils::vec3f position;
                ::utils::vec2f uv;
                ::utils::vec4f color;
            };

            struct element {
                render::vulkan::Vertices* vertices;
                render::vulkan::DescriptorSet* descriptor;
                u32 vertexCount;
                bool doRender;
            };

            uiRenderer();
            ~uiRenderer();

            bool init(GameEngine* eng);
            bool shutdown();

            static element* createElement(u32 vertexCount);
            static void destroyElement(element* ele);
            static void draw(render::core::FrameContext* frame);

        protected:
            render::vulkan::GraphicsPipeline* m_pipeline;
            render::vulkan::UniformObject* m_globalUniforms;
            render::core::DataFormat m_vfmt;
            render::core::DataFormat m_ufmt;
            ::utils::Array<element*> m_elements;
    };
};