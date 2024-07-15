#include <libsr2/ui/ui2Image.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/globals.h>

#include <render/utils/ImGui.h>
#include <utils/Array.hpp>

namespace sr2 {
    ui2Image::ui2Image(const char* name, const char* filename, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master) : ui2Visual(name, x, y, master) {
        m_displaySize = { -1, -1 };

        init(filename, p5);
    }

    ui2Image::ui2Image(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master) : ui2Visual(name, x, y, master) {
    }

    ui2Image::~ui2Image() {
        for (gfxTexture* t : m_textures) gfxTexture::release(t);
    }

    void ui2Image::init(const char* filename, i32 p2) {
        if (filename) {
            loadImage(filename);
        }

        addToMasterUnk0(p2, 0x800090000);
    }

    void ui2Image::loadImage(const char* filename) {
        gfxTexture* tex = gfxTexture::get(filename, 0, 1);

        if (tex) {
            tex->addRef();
            m_textures.push(tex);
        }
    }
    
    void ui2Image::FUN_001f5db0(i32 p1) {
    }

    void ui2Image::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive || event != WidgetEventType::UNK8) {
            ui2Visual::onEvent(source, event, data);
            return;
        }

        // FUN_001f5df0(-10);
    }

    void ui2Image::draw() {
        ui2Widget::draw();

        if (!m_isActive) return;

        if (field_0x78) {
            if (m_textures.size() == 0) return;

            i32 w = m_displaySize.x;
            i32 h = m_displaySize.y;
            gfxTexture* tex = m_textures[0];

            if (w == -1) w = tex->getDimensions().x;
            if (h == -1) h = tex->getDimensions().y;

            auto* dl = ImGui::GetWindowDrawList();
            dl->AddRect(
                ImVec2(m_pos.x * debug_ui_scale, m_pos.y * debug_ui_scale),
                ImVec2((m_pos.x + w) * debug_ui_scale, (m_pos.y + h) * debug_ui_scale),
                m_color ? m_color : 0xFFFFFFFF
            );
            
            /*
            dl->AddText(
                ImGui::GetFont(),
                debug_ui_font_size * debug_ui_scale,
                ImVec2(m_pos.x * debug_ui_scale, m_pos.y * debug_ui_scale),
                m_color ? m_color : 0xFFFFFFFF,
                m_widgetName->get()
            );
            */
        }
    }

    void ui2Image::setSize(i32 w, i32 h) {
        m_displaySize = { w, h };
    }
};