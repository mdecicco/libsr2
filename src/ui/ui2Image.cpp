#include <libsr2/ui/ui2Image.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/globals.h>

#include <render/utils/ImGui.h>
#include <render/vulkan/VertexBuffer.h>
#include <render/vulkan/DescriptorSet.h>
#include <utils/Array.hpp>

namespace sr2 {
    ui2Image::ui2Image(const char* name, const char* filename, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master) : ui2Visual(name, x, y, master) {
        m_displaySize = { -1, -1 };
        m_elem = nullptr;
        m_animFrameRate = 30.0f;
        m_animStartTime = 0.0f;
        m_animState = 0;
        m_animMode = 0;
        m_frameIdx = 0;
        m_animElapsedTime = 0.0f;

        init(filename, p5);
    }

    ui2Image::~ui2Image() {
        for (gfxTexture* t : m_textures) gfxTexture::release(t);
        if (m_elem) uiRenderer::destroyElement(m_elem);
    }

    void ui2Image::init(const char* filename, i32 priority) {
        if (filename) {
            loadImage(filename);
        }

        makeRenderable(priority);

        if (m_textures.size() > 0) {
            m_elem = uiRenderer::createElement(6);
            m_elem->descriptor->add(m_textures[0]->getTex(), 1);
            m_elem->descriptor->update();
        }
    }

    void ui2Image::loadImage(const char* filename) {
        gfxTexture* tex = gfxTexture::get(filename, 0, 1);

        if (tex == gfxTexture::None) {
            u32 texIdx = 0;
            while (true) {
                char fn[256] = { 0 };
                snprintf(fn, 256, "%s-%04d", filename, texIdx++);
                tex = gfxTexture::get(fn, 0, 1);
                if (tex == gfxTexture::None) break;

                tex->addRef();
                m_textures.push(tex);
            }
        } else if (tex) {
            tex->addRef();
            m_textures.push(tex);
        }
    }

    void ui2Image::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive || event != WidgetEventType::UNK8) {
            ui2Visual::onEvent(source, event, data);
            return;
        }

        FUN_001f5df0(-10);
    }

    void ui2Image::draw() {
        ui2Widget::draw();

        if (!m_isActive) return;

        i32 numTextures = m_textures.size();
        
        switch (m_animState) {
            case 0: {
                m_animState = 2;
                m_animStartTime = datTimeManager::ElapsedTime;
                m_animElapsedTime = 0.0f;
                m_frameIdx = 0;
                dispatchEvent(WidgetEventType::AnimationStarted);
                break;
            }
            case 1: {
                m_animState = 2;
                m_frameIdx = numTextures - 1;
                m_animStartTime = datTimeManager::ElapsedTime - (f32(numTextures) / utils::max(1e-05f, m_animFrameRate) - 1.175494e-38f);
                m_animElapsedTime = 0.0f;
                dispatchEvent(WidgetEventType::AnimationStarted);
                break;
            }
            case 2: {
                f32 elapsed = datTimeManager::ElapsedTime - m_animStartTime;

                switch (m_animMode) {
                    case 0:
                    case 1: {
                        m_frameIdx = 0;
                        if (numTextures > 0) {
                            if (elapsed >= 0.0f) m_frameIdx = i32(elapsed * m_animFrameRate) % numTextures;
                            else m_frameIdx = numTextures - i32(-elapsed * m_animFrameRate) % (numTextures + 1);

                            i32 t1 = i32((elapsed * m_animFrameRate) / f32(numTextures));
                            i32 t2 = i32((m_animElapsedTime * m_animFrameRate) / f32(numTextures));
                            if (t1 != t2) dispatchEvent(WidgetEventType::AnimationFrame);
                        }
                        break;
                    }
                    case 2:
                    case 3: {
                        i32 unk = i32(elapsed * utils::max(m_animFrameRate, 1e-05f));
                        m_frameIdx = utils::max(utils::min(unk, numTextures - 1), 0);

                        if (m_frameIdx != unk) {
                            m_animState = 3;
                            dispatchEvent(WidgetEventType::AnimationEnded);
                        }
                        break;
                    }
                    default: {
                        m_frameIdx = 0;
                        break;
                    }
                }

                m_animElapsedTime = elapsed;
                break;
            }
            default: break;
        }

        if (!m_isVisible) return;

        i32 w = m_displaySize.x;
        i32 h = m_displaySize.y;
        gfxTexture* tex = m_textures[m_frameIdx];

        if (w == -1) w = tex->getDimensions().x;
        if (h == -1) h = tex->getDimensions().y;

        utils::vec2f pMin = utils::vec2f(m_pos.x * debug_ui_scale, m_pos.y * debug_ui_scale);
        utils::vec2f pMax = utils::vec2f((m_pos.x + w) * debug_ui_scale, (m_pos.y + h) * debug_ui_scale);
        utils::vec4f color = utils::vec4f::FromU32(_byteswap_ulong(m_color));

        if (m_elem && m_elem->vertices->beginUpdate()) {
            m_elem->descriptor->set(tex->getTex(), 1);
            m_elem->descriptor->update();

            u32 v = 0;
            m_elem->vertices->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMin.x, pMin.y, -0.5f), utils::vec2f(0.0f, 0.0f), color };
            m_elem->vertices->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMax.x, pMin.y, -0.5f), utils::vec2f(1.0f, 0.0f), color };
            m_elem->vertices->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMax.x, pMax.y, -0.5f), utils::vec2f(1.0f, 1.0f), color };
            
            m_elem->vertices->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMin.x, pMin.y, -0.5f), utils::vec2f(0.0f, 0.0f), color };
            m_elem->vertices->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMax.x, pMax.y, -0.5f), utils::vec2f(1.0f, 1.0f), color };
            m_elem->vertices->at<uiRenderer::vertex>(v++) = { utils::vec3f(pMin.x, pMax.y, -0.5f), utils::vec2f(0.0f, 1.0f), color };
            m_elem->vertices->commitUpdate();
            m_elem->doRender = true;
        }
    }

    void ui2Image::setFrameRate(f32 frameRate) {
        m_animFrameRate = frameRate;
    }

    void ui2Image::FUN_001f5db0(i32 p1) {
        if (p1 == 1) {
            m_animMode = 0;
            m_animState = 3;
            return;
        }

        if (p1 != 3) {
            m_animMode = p1;
            return;
        }

        m_animState = 3;
        m_animMode = 2;
    }

    void ui2Image::FUN_001f5df0(i32 p1) {
        if (p1 == -10) m_animState = 0;
        else if (p1 == -9) m_animState = 1;
    }

    void ui2Image::setSize(i32 w, i32 h) {
        m_displaySize = { w, h };
    }
};