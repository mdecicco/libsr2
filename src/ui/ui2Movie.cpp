#include <libsr2/ui/ui2Movie.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/gfx/gfx.h>
#include <libsr2/gfx/mpeg.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/utilities/datArgParser.h>
#include <libsr2/globals.h>
#include <libsr2/managers/datTimeManager.h>

#include <render/vulkan/VertexBuffer.h>
#include <render/vulkan/DescriptorSet.h>

namespace sr2 {
    undefined4 ui2Movie::DAT_00362d14 = 1;
    u32 ui2Movie::DAT_00362d10 = 0;

    ui2Movie::ui2Movie(
        const char* name,
        const char* filename,
        undefined4 p3,
        i32 x,
        i32 y,
        i32 renderPriority,
        i32* p7,
        const WidgetRef<ui2Master>& master
    ) : ui2Visual(name, x, y, master), m_filename(160) {
        m_elem = nullptr;
        m_mpeg = nullptr;
        m_displaySize = { -1, -1 };
        m_frameSize = { -1, -1 };
        m_texture = nullptr;

        field_0xac = 1;
        field_0xb0 = 0;
        field_0xb4 = p7;
        field_0xb8 = 0;
        field_0xbc = 1;
        field_0xc0 = 1;
        field_0xc4 = 0;
        field_0xd8 = 0xffffffff;

        init(filename, p3, renderPriority);
    }

    ui2Movie::~ui2Movie() {
        FUN_001f3658();
        ui2Movie::DAT_00362d10--;
        if (m_mpeg) delete m_mpeg;
        if (m_elem) uiRenderer::destroyElement(m_elem);
    }

    void ui2Movie::init(const char* filename, undefined4 p2, i32 renderPriority) {
        if (ui2Movie::DAT_00362d14) {
            ui2Movie::DAT_00362d10++;
            makeRenderable(renderPriority);
            FUN_001f3340(p2);
            setFilename(filename);
            m_mpeg = new mpeg();
        }
    }

    void ui2Movie::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive || !ui2Movie::DAT_00362d14) {
            ui2Visual::onEvent(source, event, data);
            return;
        }

        if (event == WidgetEventType::UNK65) {
            FUN_001f3360(-10);
            return;
        }

        ui2Visual::onEvent(source, event, data);
    }

    void ui2Movie::method_0x48() {
        const char* unk = m_filename.get();
        if (unk && unk[0]) {
            if (!field_0xc0) {
                FUN_001f35f0();
            }

            FUN_001f31d0();
        }
    }

    void ui2Movie::draw() {
        ui2Widget::draw();
        if (!m_isActive) return;
        if (!ui2Movie::DAT_00362d14) return;
        if (!m_isVisible) return;
        
        const char* unk = m_filename.get();
        if (!unk || !unk[0]) return;
        if (field_0xc4 != 1) return;

        ui2String local_c0(160);
        getFilePath(local_c0);

        ui2String** someStrings = ui2Base::getSomeStrings();
        if (strcmp(someStrings[0]->get(), local_c0.get()) != 0) {
            if (*someStrings[0]->get()) m_mpeg->shutdown();
            if (!m_mpeg->load(local_c0.get(), 0)) {
                someStrings[0]->set("");
                FUN_001f3210();
                dispatchEvent(WidgetEventType::UNK64);
                return;
            }

            someStrings[0]->set(local_c0.get());

            // GameLoadingScreen::FUN_001c54e8(&GameLoadingScreen::InstanceMem,9);
            // GameLoadingScreen::InstanceMem.progress_frac = 1.0;
            // GameLoadingScreen::InstanceMem.progress_frac_again_for_some_reason = 1.0;
            // GameLoadingScreen::FUN_001c54f0(&GameLoadingScreen::InstanceMem,0.25);

            m_frameSize = m_mpeg->getDimensions();
        }

        if (!field_0xb8) {
            m_mpeg->reset();
            m_mpeg->start();
            field_0xb8 = 1;

            dispatchEvent(WidgetEventType::UNK55);
        }

        if (/*field_0xb0 && */ !m_texture) {
            m_texture = m_mpeg->createTexture();
            if (!m_texture) {
                FUN_001f3210();
                dispatchEvent(WidgetEventType::UNK64);
                return;
            }

            m_elem = uiRenderer::createElement(6);
            m_elem->descriptor->add(m_texture->getTex(), 1);
            m_elem->descriptor->update();
        }

        if (field_0xd8 == -1) {
            i32 iVar2 = m_mpeg->getNextFrame(field_0xbc == 0);
            if (iVar2 == 2) {
                dispatchEvent(WidgetEventType::UNK56);
            } else if (iVar2 == 3) {
                dispatchEvent(WidgetEventType::UNK63);
                field_0xb8 = 2;
                return;
            }
        }

        vec2i sz = m_displaySize;
        if (sz.x == -1) sz.x = m_frameSize.x;
        if (sz.y == -1) sz.y = m_frameSize.y;
        
        utils::vec2f pMin = utils::vec2f(m_pos.x * debug_ui_scale, m_pos.y * debug_ui_scale);
        utils::vec2f pMax = utils::vec2f((m_pos.x + sz.x) * debug_ui_scale, (m_pos.y + sz.y) * debug_ui_scale);
        utils::vec4f color = utils::vec4f::FromU32(_byteswap_ulong(m_color));

        if (m_elem && m_elem->vertices->beginUpdate()) {
            m_elem->descriptor->set(m_texture->getTex(), 1);
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
    
    void ui2Movie::configureParser(datParser* parser) {
    }

    const char* ui2Movie::getType() const {
        return "ui2Movie";
    }

    bool ui2Movie::isA(const char* type) const {
        if (strcmp("ui2Movie", type) == 0) return true;
        return ui2Visual::isA(type);
    }

    void ui2Movie::setFilename(const char* filename) {
        m_filename.set(filename);
    }

    void ui2Movie::FUN_001f3210() {
        if (!ui2Movie::DAT_00362d14) return;

        ui2String local_c0(160);
        getFilePath(local_c0);

        ui2String** someStrings = ui2Base::getSomeStrings();
        if (strcmp(someStrings[0]->get(), local_c0.get()) != 0) {
            if (*someStrings[0]->get()) {
                m_mpeg->shutdown();
                someStrings[0]->set("");
            }

            // GameLoadingScreen::FUN_001c54e8(&GameLoadingScreen::InstanceMem,9);
            // GameLoadingScreen::InstanceMem.progress_frac = 1.0;
            // GameLoadingScreen::InstanceMem.progress_frac_again_for_some_reason = 1.0;
            // GameLoadingScreen::FUN_001c54f0(&GameLoadingScreen::InstanceMem,0.25);

            field_0xc4 = 0;
        }
    }
    
    void ui2Movie::FUN_001f31d0() {
        if (!ui2Movie::DAT_00362d14) return;
        FUN_001f3360(-10);
        field_0xc4 = 1;
    }
    
    void ui2Movie::FUN_001f3340(undefined4 p1) {
        field_0xc0 = p1;
    }

    void ui2Movie::FUN_001f3360(i32 p1) {
        if (p1 == -10) field_0xb8 = 0;
    }
    
    void ui2Movie::FUN_001f35f0() {
        // the commented stuff is probably unnecessary
        // ui2String local_c0(160);
        // getFilePath(local_c0);

        FUN_001f3658();

        // Video::CacheFileInHashTable(local_c0.get());
    }

    void ui2Movie::FUN_001f3658() {
        field_0xd8 = 0xFFFFFFFF;
        m_frameSize = { -1, -1 };
        field_0xb8 = 0;
        field_0xc4 = 0;

        if (m_texture) {
            if (m_mpeg) m_mpeg->destroyTexture();
            m_texture = nullptr;
        }
    }

    void ui2Movie::FUN_001f3328(undefined4 p1) {
        field_0xbc = p1;
    }

    void ui2Movie::FUN_001f3350(undefined4 p1) {
        field_0xac = p1;
    }
    
    void ui2Movie::getFilePath(ui2String& out) {
        if (!ui2Movie::DAT_00362d14) return;

        ui2String** someStrings = ui2Base::getSomeStrings();
        const char* str = someStrings[2]->get();
        if (field_0xc0) str = someStrings[1]->get();

        u32 len = strlen(str);
        const char* sep0 = "/";
        if (len == 0 || str[len - 1] == '\\' || str[len - 1] == '/') sep0 = "";

        const char* subdir = "";
        const char* sep1 = "";

        if (field_0xb4) {
            LANGUAGE lang;
            u32 langFlags;
            ui2Base::getGlobalMaster()->getLangInfo(&lang, &langFlags);

            static const char* langAbbevs[] = {
                "en", "es", "fr", "de", "it", "pt", "jp", "ch", "ko", "no"
            };

            subdir = langAbbevs[lang];
            sep1 = "/";
        }

        char path[160] = { 0 };
        snprintf(path, 160, "%s%s%s%s%s", str, sep0, subdir, sep1, m_filename.get());
        out.set(path);
    }
    
    void ui2Movie::stopMovie() {
        m_mpeg->reset();
    }

    bool ui2Movie::FUN_001f29f0(u32 width, u32 height) {
        ui2String** someStrings = ui2Base::getSomeStrings();
        someStrings[0] = new ui2String(160);
        someStrings[1] = new ui2String(160);
        someStrings[2] = new ui2String(160);

        if (datArgParser::GetBooleanArgument("nofmv")) {
            ui2Movie::FUN_001f2ae0();
            return false;
        }

        ui2Movie::DAT_00362d14 = 1;
        return true;
    }

    undefined4 ui2Movie::FUN_001f2ae0() {
        ui2String** someStrings = ui2Base::getSomeStrings();
        if (someStrings[0]) {
            delete someStrings[0];
            someStrings[0] = nullptr;
        }

        if (someStrings[1]) {
            delete someStrings[1];
            someStrings[1] = nullptr;
        }

        if (someStrings[2]) {
            delete someStrings[2];
            someStrings[2] = nullptr;
        }

        undefined4 result = ui2Movie::DAT_00362d14;
        if (ui2Movie::DAT_00362d14) {
            // result = Video::DeinitMpeg();
            result = 1;
            ui2Movie::DAT_00362d14 = 0;
        }

        return result;
    }
};