#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class orCullSpans;
    class gfxTexture;
    class phMaterial;
    class Stream;

    #pragma pack(1)
    struct orTerrainGridPathTile {
        u8 cell_idx;
        u8 texture_idx;
        i16 u0;
        vec2s16 tex_coords[4];
    };
    struct orTerrainGridCell {
        orTerrainGridPathTile* path_data;
        f32 unk0[2];
        u16 cull_flags;
        u8* cell_tex_indices;
    };
    struct orTerrainGridPathMapPixel {
        u32 coord_idx;
        u32 path_data_idx;
    };
    #pragma pack(pop)

    class orTerrainGrid : public phBound {
        public:
            orTerrainGrid();
            ~orTerrainGrid();

            bool load(const char* map, u32 lod_version, u32 lod_idx, mat3x3f& unk0, mat4x4f& unk1);
            void initLODs(const char* map, u32 lod_version, u32 lod_idx);

            u32 material_count;
            orCullSpans* unk0;
            orCullSpans* unk1;

            u32 width, height, cell_size;
            u32 width_in_cells, height_in_cells;
            u32 unk_width_0, unk_height_0;
            u32 scale;
            f32 inv_scale;
            gfxTexture** textures;
            phMaterial** materials;
            gfxTexture* shadow_texture;
            i8* height_map;

            u32 width_plus_4;
            vec4f unk2[1024];
            f32 clip_near;
            f32 fog_start, fog_end, fog_clamp;
            u32 fog_color;
            f32 shadow_alpha;

            u32 unk_grid_width, unk_grid_height;
            f32* unk_grid_0;
            f32* unk_grid_1;
            i8* cpv_map;
            u8* texture_ids;
            orTerrainGridCell* cells;
            orTerrainGridPathTile* path_data;
            u32* path_texture_ids;
            u16 some_palette[256];

            u32 texture_count;
            u32* texture_unks;

        protected:
            bool m_did_read_info;
            bool m_did_load_cullflags;
            bool m_did_load_palette;
            u32 m_path_tex_count;
            vec2f* m_tmp_geom_1;
            u16* m_tmp_cullflags;
            orTerrainGridPathMapPixel* m_tmp_path_map;

            void process_info(Stream* fp);
            void process_0x8194(u16 tag, Stream* fp);
            void process_0x8203(u16 tag, Stream* fp);
            void process_texids(Stream* fp);
            void process_palette(Stream* fp);
            void process_materials(Stream* fp);
            void process_textures(Stream* fp, char** tex_names);
            void process_0x8199(Stream* fp, mat3x3f& p_unk0, mat4x4f& p_unk1);
            void process_paths(Stream* fp);
            void process_0x8202(Stream* fp);
            void process_0x8204(Stream* fp);
            void process_cullflags(Stream* fp);
            void process_heightmap(Stream* fp);

            void generate_cullflags();
            void initialize_cells();
            void initialize_ugrid_0();
            void initialize_textures(const char** texNames);
            void load_cpv(const char* map, u32 lod_version);
            void load_low_lod_textures();
            void init_cell_tex_indices();
            void load_imap();
            void generate_imap();
            void load_fog_values();
    };
};