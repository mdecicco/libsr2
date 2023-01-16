#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class UnkInstance0;
    struct CullRoomData {
        undefined2 unk0;
        u16 flags;
        UnkInstance0* instances;
        undefined4 unk1;
    };

    class lvlLevel {
        public:

        protected:
            lvlLevel();
            ~lvlLevel();

            undefined2 m_unk;
            CullRoomData** m_cull_rooms;
            u32 m_room_count;
            UnkInstance0* m_first_instance;
            u32 m_instance_count;
    };

    class gfxTexture;
    class gfxLight;
    class orTerrainGrid;
    class orPropMgr;
    class orCullSpans;
    class offroadLevel : public lvlLevel {
        public:
            static offroadLevel* create();
            static void destroy();
            static offroadLevel* get();

            void setMaxViewports(u32 val, i32 unk);
            bool load(const char* map, const char* mission, WEATHER_TYPE weather);
            void initLODs(const char* map, WEATHER_TYPE weather);
            bool loadWorldClamp(const char* map);
            bool loadPointLights(const char* map, WEATHER_TYPE weather, undefined* unk0, undefined* unk1);

            void FUN_00218f20(undefined4 unk);
            f32 getShadowAlpha();

        protected:
            static offroadLevel* instance;
            offroadLevel();
            ~offroadLevel();

            const char* m_terrain_name;
            u32 max_viewports;
            gfxTexture* m_shadow_tex;
            mat4x4f* m_shadow_mtx;
            gfxLight* m_default;
            gfxLight* m_fill_1;
            gfxLight* m_fill_2;
            gfxLight* m_alt;
            gfxLight* m_alt_fill_1;
            gfxLight* m_alt_fill_2;
            vec4f m_key_color;
            vec4f m_fill_1_color;
            vec4f m_fill_2_color;
            vec4f m_ambient_color;
            vec4f m_alt_key_color;
            vec4f m_alt_fill_1_color;
            vec4f m_alt_fill_2_color;
            vec4f m_alt_ambient_color;
            f32 m_key_color_mult;
            f32 m_fill_1_mult;
            f32 m_fill_2_mult;
            f32 m_ambient_intensity;
            f32 m_alt_key_color_mult;
            f32 m_alt_fill_1_mult;
            f32 m_alt_fill_2_mult;
            f32 m_alt_ambient_intensity;
            bool m_key_dynamic;
            bool m_fill_1_dynamic;
            bool m_fill_2_dynamic;
            bool m_alt_key_dynamic;
            bool m_alt_fill_1_dynamic;
            bool m_alt_fill_2_dynamic;
            bool m_has_alt;
            f32 m_shadow_attenuation;
            f32 m_alt_shadow_attenuation;
            f32 m_tunnel_intensity;
            CullRoomData* m_cull_rooms;
            orTerrainGrid* m_terrain;
            orPropMgr* m_prop_mgr;
            orCullSpans* m_unk0;
            orCullSpans* m_unk1;
            i32 m_unk2;
            f32 m_unk3;
            undefined4 m_unk4;
            u32 m_some_width;
            u32 m_some_height;
            vec2f m_camera_xz;
            f32 m_north_clamp, m_east_clamp, m_south_clamp, m_west_clamp;
            gfxTexture* m_shadow_fader;
    };
};