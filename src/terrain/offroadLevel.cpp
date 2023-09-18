#include <libsr2/terrain/offroadLevel.h>
#include <libsr2/terrain/orTerrainGrid.h>
#include <libsr2/managers/lvlMaterialMgr.h>
#include <libsr2/gfx/gfxLight.h>
#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/sim/SpatialPartitioner.h>

namespace sr2 {
    offroadLevel* offroadLevel::instance = nullptr;
    offroadLevel* offroadLevel::create() {
        if (instance) return instance;
        instance = new offroadLevel();
        lvlMaterialMgr::create(100);
        return instance;
    }

    void offroadLevel::destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }

    offroadLevel* offroadLevel::get() {
        return instance;
    }

    void offroadLevel::setMaxViewports(u32 val, i32 unk) {
        max_viewports = val;
        if (unk < 1) unk = val;
        m_unk2 = unk;
    }

    bool offroadLevel::load(const char* map, const char* mission, WEATHER_TYPE weather) {
        // iVar3 = FUN_002ed360(5, 0);
        gfxTexture* shadow_fader = gfxTexture::get("shadowfader", 1, 0);
        if (shadow_fader != gfxTexture::None) shadow_fader->setTexEnv(shadow_fader->tex_env | 0x10001);
        // FUN_002ed360(iVar3, 0);

        initLODs(map, weather);
        loadWorldClamp(map);

        m_terrain_name = map;
        lvlMaterialMgr::create(100);

        mat3x3f a;
        mat4x4f b;
        m_terrain->load(map, weather, m_unk2, &a, &b);
        loadPointLights(map, weather, nullptr, nullptr);

        return true;
    }

    void offroadLevel::initLODs(const char* map, WEATHER_TYPE weather) {
    }

    bool offroadLevel::loadWorldClamp(const char* map) {
        return true;
    }
    
    bool offroadLevel::loadPointLights(const char* map, WEATHER_TYPE weather, undefined* unk0, undefined* unk1) {
        return true;
    }


    void offroadLevel::FUN_00218f20(undefined4 unk) {

    }

    f32 offroadLevel::getShadowAlpha() {
        f32 alpha = m_terrain->shadow_alpha;
        alpha *= 2.0f;
        return alpha < 1.0f ? 1.0f : alpha;
    }

    offroadLevel::offroadLevel() {
        // m_prop_mgr = new orPropMgr();
        m_terrain = new orTerrainGrid();
        // m_unk0 = new orCullSpans();
        // m_unk1 = new orCullSpans();
        m_unk2 = 1;
        max_viewports = 1;
        m_unk3 = 2.0f;
        m_unk4 = 10;
        // FLOAT_00362f68 = 1.0f;
        // DAT_0087e798 = 0;
        // FLOAT_00362f6c = 1.0f;

        m_default = new gfxLight();
        m_alt = new gfxLight();

        m_fill_1 = new gfxLight();
        m_alt_fill_1 = new gfxLight();

        m_fill_2 = new gfxLight();
        m_alt_fill_2 = new gfxLight();

        m_default->reset();
        m_alt->reset();

        m_fill_1->reset();
        m_alt_fill_1->reset();

        m_fill_2->reset();
        m_alt_fill_2->reset();

        m_ambient_color = m_alt_ambient_color = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_ambient_intensity = m_alt_ambient_intensity = 0.1f;
        m_key_color = { 1.0f, 0.9f, 0.8f, 1.0f };
        m_key_color_mult = m_alt_key_color_mult = 0.8f;
        m_fill_1_color = m_alt_fill_1_color = { 0.7f, 0.85f, 1.0f, 1.0f };
        m_fill_1_mult = m_alt_fill_1_mult = 0.5f;
        m_fill_2_color = m_alt_fill_2_color = { 0.85f, 0.7f, 1.0f, 1.0f };
        m_fill_2_mult = m_alt_fill_2_mult = 0.3f;
        m_tunnel_intensity = 0.4f;
        m_shadow_attenuation = m_alt_shadow_attenuation = 1.0f;
        
        m_default->direction = { 1.0f, 0.0f, 0.0f };
        m_fill_1->direction = { 1.0f, 0.0f, 0.0f };
        m_fill_2->direction = { 1.0f, 0.0f, 0.0f };

        m_alt->direction = { 1.0f, 0.0f, 0.0f };
        m_alt_fill_1->direction = { 1.0f, 0.0f, 0.0f };
        m_alt_fill_2->direction = { 1.0f, 0.0f, 0.0f };

        m_key_dynamic = m_alt_key_dynamic = false;
        m_fill_1_dynamic = m_alt_fill_1_dynamic = false;
        m_fill_2_dynamic = m_alt_fill_2_dynamic = false;
        m_has_alt = false;

        // Utils::SparkArray::Create(100);
        // Unknown::UnkArr1::Create(100);
        // Unknown::UnkArr2::Create(256);
        // Unknown::UnkArr3::Create(100);
    }

    offroadLevel::~offroadLevel() {
        // dgLightSet::Shutdown();
        delete m_fill_1;
        delete m_fill_2;
        delete m_alt;
        delete m_alt_fill_1;
        delete m_alt_fill_2;
        
        // if (m_prop_mgr) delete m_prop_mgr;
        if (m_terrain) delete m_terrain;
        // delete m_unk0;
        // delete m_unk1;

        if (m_shadow_fader /* && m_shadow_fader != gfxTexture::None*/) {
            delete m_shadow_fader;
        }

        // Utils::SparkArray::Destroy();
        // Unknown::UnkArr1::Destroy();
        // Unknown::UnkArr2::Destroy();
        // Unknown::UnkArr3::Destroy();
    }
};