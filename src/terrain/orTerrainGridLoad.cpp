#include <libsr2/terrain/orTerrainGrid.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/managers/phMaterialMgr.h>
#include <libsr2/sim/phBound.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/stream.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/gfx/gfxTexture.h>

namespace sr2 {
    bool orTerrainGrid::load(const char* map, u32 lod_version, u32 lod_idx, mat3x3f* p_unk0, mat4x4f* p_unk1) {
        material_count = 0;
        // field11_0x68 = new mat4x4f();
        // field13_0x70 = 0;
        initLODs(map, lod_version, lod_idx);

        std::string indexed_map_name;
        if (lod_version) indexed_map_name = format("%s_%d", map, lod_version);
        else indexed_map_name = map;
        Stream* mtl = datAssetManager::open("offroad", indexed_map_name.c_str(), "mtl", 0, true);
        if (!mtl && lod_version) mtl = datAssetManager::open("offroad", map, "mtl", 0, true);
        
        // debug
        mtl = nullptr;
        if (mtl) {
            datAsciiTokenizer tok;
            tok.init(map, mtl);
            while (tok.checkToken("mtl", false)) {
                phMaterialMgr::get()->addMaterial(tok);
            }
            delete mtl;
        }

        TaggedStream tsv;
        Stream* fp = tsv.open(const_cast<char*>(map));
        if (!fp) fp = tsv.open("offroad", const_cast<char*>(indexed_map_name.c_str()), "btx2");
        if (!fp) fp = tsv.open("offroad", const_cast<char*>(map), "btx2");
        if (!fp) return false;

        char* tex_names[1024] = { 0 };

        u16 tag;
        u32 size;
        u32 unused;
        while (true) {
            if (!tsv.readTag(&tag, &size)) break;

            switch (tag) {
                case 8192: { fp->read(&unused, 4); break; }
                case 8193: { process_info(fp); break; }
                case 8194: { process_heightmap_1(fp); break; }
                case 8203: { process_heightmap(fp); break; }
                case 8195: { process_texids(fp); break; }
                case 8196: { process_palette(fp); break; }
                case 8197: { process_materials(fp); break; }
                case 8198: { process_textures(fp, tex_names); break; }
                case 8199: { process_8199(fp, p_unk0, p_unk1); break; }
                case 8201: { process_paths(fp); break; }
                case 8202: { process_8202(fp); break; }
                case 8204: { process_8204(fp); break; }
                case 8205: { process_cullflags(fp); break; }
                case 8206: { process_unk_map(fp); break; }
                default: break;
            }
        }

        tsv.close();

        if (!m_did_load_cullflags) {
            generate_cullflags();
        }

        initialize_cells();
        initialize_ugrid_0();
        initialize_textures(const_cast<const char**>(tex_names));
        load_cpv(indexed_map_name.c_str(), lod_version);
        load_low_lod_textures(map, lod_version);
        init_cell_tex_indices();
        load_imap(map, lod_version);
        load_fog_values(map, lod_version);

        if (!m_did_load_8199) {
            if (p_unk1) {
                p_unk1->x = { 1.0f, 1.0f, 1.0f, 1.0f };
                p_unk1->y = { 1.0f, 1.0f, 1.0f, 1.0f };
                p_unk1->z = { 1.0f, 1.0f, 1.0f, 1.0f };
            }

            if (p_unk0) {
                p_unk0->x = { 0.0f, -0.7071f, 0.7071f };
                p_unk0->y = { 0.0f, -0.7071f, 0.7071f };
                p_unk0->z = { 0.0f, -0.7071f, 0.7071f };
            }
        }

        // GFX::vgl::SelectVertexTypeMaybe(VertexColorTexture);
        shadow_texture = nullptr;
        calculateBoundingBox();

        // Game::lvlProgress::updateTask(0, 100.0f);

        return true;
    }
    
    void orTerrainGrid::process_info(Stream* fp) {
        fp->read(&scale, 4);
        fp->read(&width, 4);
        fp->read(&height, 4);
        fp->read(&cell_size, 4);
        inv_scale = 1.0f / f32(scale);
        width_in_cells = width / cell_size;
        height_in_cells = height / cell_size;

        unk_width_0 = width / 64;
        unk_height_0 = height / 64;
        unk_grid_width = width + 32;
        unk_grid_height = height + 3;
        unk_grid_0 = new f32[unk_grid_width * unk_grid_height];
        height_data = new f32[unk_grid_width * unk_grid_height];
        cpv_map = new i8[unk_grid_width * unk_grid_height];
        texture_ids = new u8[unk_grid_width * unk_grid_height];
        cells = new orTerrainGridCell[width_in_cells * height_in_cells];
        m_did_read_info = true;
        m_tmp_geom_1 = new vec2f[width_in_cells * height_in_cells];
    }
    
    void orTerrainGrid::process_heightmap_1(Stream* fp) {
        process_heightmap(fp);

        for (u32 i = unk_grid_width * unk_grid_height;i != 0;i--) {
            u32 *u = (u32*)&height_data[i];
            *u = *u & 0xFFFFFF000 | (*u & 0xFF) << 4;
        }
    }

    void orTerrainGrid::process_heightmap(Stream* fp) {
        if (!m_did_read_info) throw "Encountered btx2 block before info block";
        // Game::lvlProgress::pushSubTask(25.0f, nullptr);

        f32* read_pos = height_data;
        i32 h = height + 3;
        if (h < 32) h = height;
        else {
            u32 read_sz = (width + 32) * 128;
            u32 stride = (width + 32) * 32;
            for (i32 i = 0;i + 64 <= height + 3;i += 32) {
                fp->read(read_pos, read_sz);
                read_pos += stride;
                /* every 0.03 seconds...
                {
                    Game::lvlProgress::UpdateTask(
                        gfx::Pipeline::Fade != gfx::Pipeline::FadeEnd,
                        (i * 100) / h
                    )
                }
                */
            }
        }

        i32 a = height + 34;
        if ((height + 3) >= 0) a = height + 3;
        i32 b = ((height + 3) + (a / 32)) * -32;
        if (b) fp->read(read_pos, b * 4);
        
        // Game::lvlProgress::popSubTask();
    }

    void orTerrainGrid::process_texids(Stream* fp) {
        if (!m_did_read_info) throw "Encountered btx2 block before info block";
        fp->read(texture_ids, unk_grid_width * unk_grid_height);
    }

    void orTerrainGrid::process_palette(Stream* fp) {
        if (!m_did_read_info) throw "Encountered btx2 block before info block";
        m_did_load_palette = true;

        // Game::lvlProgress::pushSubTask(5.0, nullptr);
        fp->read(some_palette, 512);
        fp->read(cpv_map, unk_grid_width * unk_grid_height);
        // Game::lvlProgress::popSubTask();
    }

    void orTerrainGrid::process_materials(Stream* fp) {
        fp->read(&material_count, 4);
        materials = new phMaterial*[material_count];
        for (u32 i = 0;i < material_count;i++) {
            char name[32] = { 0 };
            fp->read(name, 32);
            materials[i] = phMaterialMgr::get()->find(name);
        }
    }

    void orTerrainGrid::process_textures(Stream* fp, char** tex_names) {
        fp->read(&texture_count, 4);
        if (texture_count == 0) return;

        u16 val;
        texture_material_ids = new u32[texture_count];
        for (u32 i = 0;i < texture_count;i++) {
            tex_names[i] = new char[32];
            fp->read(tex_names[i], 32);
            fp->read(&val, 2);
            texture_material_ids[i] = val;
        }
    }

    void orTerrainGrid::process_8199(Stream* fp, mat3x3f* p_unk0, mat4x4f* p_unk1) {
        m_did_load_8199 = true;
        vec4f u0;
        vec3f u1;
        fp->read(&u0.x, 16);
        p_unk1->x = u0;

        fp->read(&u1.x, 12);
        p_unk0->x = u1;

        fp->read(&u0.x, 16);
        p_unk1->w = { 1.0f, 1.0f, 1.0f, 1.0f };

        fp->read(&u0.x, 16);
        p_unk1->y = u0;

        fp->read(&u1.x, 12);
        p_unk0->y = u1;

        fp->read(&u0.x, 16);
        p_unk1->z = u0;

        fp->read(&u1.x, 12);
        p_unk0->z = u1;
    }
    
    void orTerrainGrid::process_paths(Stream* fp) {
        // Game::lvlProgress::pushSubTask(7.0f, nullptr);

        #pragma pack(1)
        struct path_data_in {
            u8 cell_idx;
            u8 texture_idx;
            u8 tex_coord_offsets[4];
        };
        #pragma pack(pop)
        
        u32 path_cell_count;
        fp->read(&path_cell_count, 4);
        path_data_in* input = new path_data_in[path_cell_count];
        fp->read(input, path_cell_count * 6);
        // Game::lvlProgress::updateTask(0.0f, 40.0f);

        u32 coord_count;
        fp->read(&coord_count, 4);
        vec2s16* coords = new vec2s16[coord_count];
        fp->read(coords, coord_count * 4);

        fp->read(&m_path_tex_count, 4);
        path_texture_ids = new u32[m_path_tex_count];
        fp->read(path_texture_ids, m_path_tex_count * 4);
        // Game::lvlProgress::updateTask(0.0f, 40.0f);

        m_tmp_path_map = new orTerrainGridPathMapPixel[width_in_cells * height_in_cells];
        fp->read(m_tmp_path_map, width_in_cells * height_in_cells * 8);

        path_data = new orTerrainGridPathTile[path_cell_count + 1];
        for (u32 i = 0;i < width_in_cells * height_in_cells;i++) {
            auto& pixel = m_tmp_path_map[i];
            if (pixel.coord_idx == -1) continue;

            auto* in = &input[pixel.path_data_idx];
            auto* out = &path_data[pixel.path_data_idx];
            do {
                out->cell_idx = in->cell_idx;
                out->texture_idx = in->texture_idx;
                out->tex_coords[0] = coords[pixel.coord_idx + in->tex_coord_offsets[0]];
                out->tex_coords[1] = coords[pixel.coord_idx + in->tex_coord_offsets[1]];
                out->tex_coords[2] = coords[pixel.coord_idx + in->tex_coord_offsets[2]];
                out->tex_coords[3] = coords[pixel.coord_idx + in->tex_coord_offsets[3]];

                f32 minx = 100.0f;
                for (u8 i = 0;i < 4;i++) {
                    f32 y = f32(out->tex_coords[i].x) * 0.0009765625f;
                    if (y < minx) minx = y;
                }

                minx = floorf(minx);

                for (u8 i = 0;i < 4;i++) {
                    f32 x = out->tex_coords[i].x;
                    f32 y = out->tex_coords[i].y;
                    out->tex_coords[i].x = (x / 1024.0f - minx) * 1024.0f;
                    out->tex_coords[i].y = (y / 1024.0f       ) * 1024.0f; // Don't ask me...
                }

                in++;
                out++;
            } while (!(out->texture_idx & 0x80));
        }

        delete [] input;
        delete [] coords;

        // Game::lvlProgress::popSubTask();
    }

    void orTerrainGrid::process_8202(Stream* fp) {
        if (!m_did_read_info) throw "Encountered btx2 block before info block";

        f32* map = new f32[width_in_cells * height_in_cells];
        fp->read(map, width_in_cells * height_in_cells * 4);
        
        for (u32 i = 0;i < width_in_cells * height_in_cells;i++) {
            f32 unk = map[i] * map[i] + f32(cell_size) * 0.5f * f32(cell_size) * f32(scale) * f32(scale);

            m_tmp_geom_1[i].x = map[i];
            m_tmp_geom_1[i].y = sqrtf(unk);
        }

        delete [] map;
    }

    void orTerrainGrid::process_8204(Stream* fp) {
        if (!m_did_read_info) throw "Encountered btx2 block before info block";

        fp->read(m_tmp_geom_1, width_in_cells * height_in_cells * 8);
    }

    void orTerrainGrid::process_cullflags(Stream* fp) {
        m_did_load_cullflags = true;
        m_tmp_cullflags = new u16[width_in_cells * height_in_cells];
        fp->read(m_tmp_cullflags, width_in_cells * height_in_cells * 2);
    }

    void orTerrainGrid::process_unk_map(Stream* fp) {
        height_map = new i8[width * height];
        fp->read(height_map, width * height);
    }

    void orTerrainGrid::generate_cullflags() {
        m_tmp_cullflags = new u16[width_in_cells * height_in_cells];

        for (u32 y = 0;y + 1 < height_in_cells;y++) {
            for (u32 x = 0;x < width_in_cells;x++) {
                u16& flags = m_tmp_cullflags[y * width_in_cells + x];
                flags = 0b0000000000010100;

                for (u32 i = 0;i < 8;i++) {
                    u32* unk = (u32*)&height_data[(i + y * 8) * (width + 32) + (x * 8)];
                    for (u32 j = 0;j < 8;j++) {
                        if (unk[j] & 0x200) {
                            flags |= 0b1000000000000000;
                            break;
                        }
                    }

                    if (flags & 0b1000000000000000) break;
                }
            }
        }
    }

    void orTerrainGrid::initialize_cells() {
        for (u32 y = 0;y + 1 < height_in_cells;y++) {
            for (u32 x = 0;x < width_in_cells;x++) {
                u16 cell_idx = y * width_in_cells + x;
                auto& cell = cells[cell_idx];
                cell.cull_flags = m_tmp_cullflags[cell_idx];

                auto& path = m_tmp_path_map[cell_idx];
                if (path.path_data_idx == 0xffffffff) cell.path_data = nullptr;
                else cell.path_data = &path_data[path.path_data_idx];

                cell.unk0[0] = m_tmp_geom_1[cell_idx].x;
                cell.unk0[1] = m_tmp_geom_1[cell_idx].y;
            }
        }

        delete [] m_tmp_cullflags;
        delete [] m_tmp_path_map;
        delete [] m_tmp_geom_1;
        m_tmp_cullflags = nullptr;
        m_tmp_path_map = nullptr;
        m_tmp_geom_1 = nullptr;
    }

    void orTerrainGrid::initialize_ugrid_0() {
        for (u32 y = 0;y + 1 < height / 8;y++) {
            for (u32 x = 0;x < width / 8;x++) {
                f32 u = height_data[(y * 8) * (width + 32) + (x * 8)];
                unk_grid_0[y * ((width / 8) + 4) + x] = u;
            }
        }
    }

    void orTerrainGrid::initialize_textures(const char** texNames) {
        textures = new gfxTexture*[texture_count + (unk_width_0 * unk_height_0)];

        // Game::lvlProgress::pushSubTask(13.0f, nullptr);
        // FUN_002ed360(DAT_003633bc, 0);

        for (u32 i = 0;i < texture_count;i++) {
            gfxTexture::getMovie(&textures[i], texNames[i], 1);
            if (textures[i] && textures[i] != gfxTexture::None) {
                // textures[i]->setMipmapBias(-3.25f, 0);
            }

            // Game::lvlProgress::updateTask(0, f32((i * 100) / texture_count));

            delete [] texNames[i];
        }

        // Game:: lvlProgress::popSubTask();

        for (u32 i = 0;i < m_path_tex_count;i++) {
            // textures[path_texture_ids[i]]->setMipmapBias(-8.0f, 1);
            u32 env = textures[path_texture_ids[i]]->getTexEnv();
            textures[path_texture_ids[i]]->setTexEnv(env & 0xfffefffe | 1);
        }
    }

    void orTerrainGrid::load_cpv(const char* map, u32 lod_version) {
        Stream* fp = datAssetManager::open("offroad", map, "cpv", 1, true);
        if (!m_did_load_palette && !fp) return;
        if (!fp) return;

        u32 magic = 0;
        fp->read(&magic, 4);
        if (magic != 0x24682468) {
            delete fp;
            return;
        }

        u32 w, h;
        fp->read(&w, 4);
        fp->read(&h, 4);
        if (w != width || h != height) {
            delete fp;
            return;
        }

        // Game::lvlProgress::pushSubTask(5.0f, nullptr);

        fp->read(some_palette, 512);
        for (u32 i = 0;i < h;i++) {
            fp->read(&cpv_map[(i * (width + 32)) + 1], w);
            // Game::lvlProgress::updateTask(0, f32((i * 100) / h));
        }

        delete fp;

        // Again... Don't ask me
        for (u32 i = 0;i < h;i++) {
            cpv_map[0] = cpv_map[1];
            cpv_map[w + 1] = cpv_map[w];
            cpv_map[w + 2] = cpv_map[w];
        }

        // Some context is lost in translation here. The negative offset
        // from cpv_map is because cpv_map is part of a larger allocation
        // which contains multiple potentially unrelated datasets. The 
        // game uses the segments of that allocation separately. However,
        // I STILL have no idea what the intent of this line is... In fact,
        // I have no idea what _ANY_ of these lines mean.....
        //
        // memcpy(cpv_map - ((width + 32) + 1), cpv_map, width + 32);
        // ...
        // memcpy(
        //     (cpv_map - (width + 32) + 1) + ((width + 32) * (height + 1)),
        //     (cpv_map - (width + 32) + 1) + ((width + 32) * height),
        //     width + 32
        // );
        // ...
        // memcpy(
        //     (cpv_map - (width + 32) + 1) + (width + 32) * (height + 2),
        //     (cpv_map - (width + 32) + 1) + ((width + 32) * height),
        //     width + 32
        // );

        // Game::lvlProgress::popSubTask();
    }

    void orTerrainGrid::load_low_lod_textures(const char* map, u32 lod_version) {
        // some orCullSpans nonsense...
        // ...
        low_lod_texture_ids = new u32[unk_width_0 * unk_height_0];
        
        u32 idx = 0;
        for (u32 y = 0;y < unk_height_0;y++) {
            for (u32 x = 0;x < unk_width_0;x++) {
                std::string texPath;
                if (lod_version) {
                    texPath = format(
                        "%stexture/%s_VL_%d/%s_VL_%d_%d",
                        datAssetManager::getPath(),
                        map, lod_version, map, y, x
                    );
                } else {
                    texPath = format(
                        "%stexture/%s_VL/%s_VL_%d_%d",
                        datAssetManager::getPath(),
                        map, map, y, x
                    );
                }

                gfxTexture* tex = gfxTexture::get(texPath.c_str(), 1, 1);
                if (tex == gfxTexture::None && lod_version) {
                    texPath = format(
                        "%stexture/%s_VL/%s_VL_%d_%d",
                        datAssetManager::getPath(),
                        map, map, y, x
                    );
                    tex = gfxTexture::get(texPath.c_str(), 1, 1);
                }

                if (tex) tex->setTexEnv(tex->getTexEnv() | 0x10001);

                u32 tidx = texture_count + idx;
                textures[tidx] = tex;
                low_lod_texture_ids[idx] = tidx;
                idx++;

                // Game::lvlProgress::UpdateTask(0, f32(idx * 100) / f32(unk_width_0 * unk_height_0));
            }
        }

        // There's also some stuff about downloading the textures to "path3" that
        // I am ignoring because it's not applicable to anything other than the PS2
        // hardware
        //
        // Hope I can figure out how to reproduce the intent!
    }
    
    void orTerrainGrid::init_cell_tex_indices() {
        u8 buf[1024] = { 0xFF };
        for (u32 y = 0;y + 1 < height_in_cells;y++) {
            for (u32 x = 0;x < width_in_cells;x++) {
                u32 cell_tex_count = 0;
                for (u32 iy = 0;iy < 8;iy++) {
                    for (u32 ix = 0;ix < 8;ix++) {
                        u32 tx = (y * 8) + ix;
                        u32 ty = (x * 8) + iy;
                        u8 tex_id = texture_ids[tx * (width + 32) + ty];

                        u32 k = 0;
                        while (k < cell_tex_count && buf[k] != tex_id) k++;
                        if (k == cell_tex_count) buf[cell_tex_count++] = tex_id;
                    }
                }

                orTerrainGridCell& cell = cells[y * width_in_cells + x];
                if (cell.path_data) {
                    auto* pdata = cell.path_data;

                    while (!(pdata->texture_idx & 0x80)) {
                        u8 tex_id = u8(path_texture_ids[pdata->texture_idx]);

                        u32 k = 0;
                        while (k < cell_tex_count && buf[k] != tex_id) k++;
                        if (k == cell_tex_count) buf[cell_tex_count++] = tex_id;

                        pdata++;
                    }
                }

                cell.cell_tex_indices = new u8[cell_tex_count + 1];
                memcpy(cell.cell_tex_indices, buf, cell_tex_count);
                
                u8* dst = cell.cell_tex_indices;
                for (u32 i = 0;i < cell_tex_count;i++) dst[i] = buf[i];
                cell.cell_tex_indices[cell_tex_count] = 0xFF;
            }
        }
    }

    void orTerrainGrid::load_imap(const char* map, u32 lod_version) {
        std::string indexed_map_name;
        if (lod_version) indexed_map_name = format("%s_%d", map, lod_version);
        else indexed_map_name = map;

        Stream* fp = datAssetManager::open("offroad", indexed_map_name.c_str(), "imap", 0, true);
        if (!fp && lod_version) {
            fp = datAssetManager::open("offroad", map, "imap", 0, true);
        }

        if (!fp) {
            generate_imap();
            return;
        }
    
        u8 buf0[1024] = { 0 };
        for (u32 i = 0;i < 16;i++) {
            f32 u = powf(f32(i) / 15.0f, 2.0f);
            i32 v = 0;
            for (i32 j = 0;j <= u32(u * 255.0f);j++) {
                buf0[j] = i * 16;
            }
        }

        // Game::lvlProgress::pushSubTask(15.0f, nullptr);

        u8 buf1[4096];
        for (u32 y = 0;y < height;y++) {
            fp->read(buf1, width);
            u32* htd_row = (u32*)&height_data[y * (width + 32)];

            for (u32 x = 0;x < width;x++) {
                *htd_row = *htd_row & 0xfffffff0 | (buf0[buf1[x]] / 16);
                htd_row++;
            }

            // Game::lvlProgress::updateTask(0, (y * 100) / height);
        }

        /*
        for (u32 i = 0;i < 16;i++) {
            TerrainHeight::IntensityGammaTable[i] = powf(powf(f32(i) / 15.0f, 2.0f), 2.0f);
        }
        */

        delete fp;
        // Game::lvlProgress::popSubTask();
    }

    void orTerrainGrid::generate_imap() {
        for (u32 y = 0;y < height;y++) {
            i8* cpv_row = &cpv_map[y * (width + 32)];
            u32* htd_row = (u32*)&height_data[y * (width + 32)];

            for (u32 x = 0;x < width;x++) {
                // Yes I know it's a float
                // Don't ask me what sort of sorcery is going on here
                *htd_row = *htd_row & 0xfffffff0 | (some_palette[*cpv_row] & 0x3e0) / 128;
                htd_row++;
                cpv_row++;
            }
        }

        /*
        for (u32 i = 0;i < 16;i++) {
            TerrainHeight::IntensityGammaTable[i] = f32(i) / 15.0f;
        }
        */
    }

    void orTerrainGrid::load_fog_values(const char* map, u32 lod_version) {
        std::string indexed_map_name;
        if (lod_version) indexed_map_name = format("%s_%d", map, lod_version);
        else indexed_map_name = map;

        fog_end = LODDist[7] * 1.4f;

        Stream* fp = datAssetManager::open("tune/fog", indexed_map_name.c_str(), "fog", 0, true);
        if (!fp && lod_version) {
            fp = datAssetManager::open("tune/fog", map, "fog", 0, true);
        }
        if (!fp) return;

        datAsciiTokenizer tok;
        tok.init(map, fp);
        char buf[64] = { 0 };
        while (tok.getToken(buf, 64)) {
            if (strncmp(buf, "fogend", 64) == 0) fog_end = tok.readFloat();
            else if (strncmp(buf, "fogstart", 64) == 0) fog_start = tok.readFloat();
            else if (strncmp(buf, "fogclamp", 64) == 0) fog_clamp = tok.readFloat();
            else if (strncmp(buf, "fogcolor", 64) == 0) {
                u8 r = tok.readInt32();
                u8 g = tok.readInt32();
                u8 b = tok.readInt32();
                u8 a = 128;
                fog_color = r | g << 8 | b << 16 | a << 24;
            }
        }

        delete fp;
    }

    void orTerrainGrid::initLODs(const char* map, u32 lod_version, u32 lod_idx) {
        LODDistRelated = -1.0f;
        LODDist[0] = -1.0f;
        LODDist[1] = -1.0f;
        LODDist[2] = 80.0f;
        LODDist[3] = 150.0f;
        LODDist[4] = 500.0f;
        LODDist[5] = 600.0f;
        LODDist[6] = 700.0f;
        LODDist[7] = 3000.0f;

        bool whoknows = true;

        std::string filename = format("%s_v%d_p%d", map, lod_version, lod_idx);
        Stream* fp = datAssetManager::open("tune/lod", filename.c_str(), "terrainlod", 1, true);
        if (!fp) {
            filename = format("%s_v%d", map, lod_version);
            fp = datAssetManager::open("tune/lod", filename.c_str(), "terrainlod", 1, true);
        }
        if (!fp) {
            whoknows = false;
            filename = format("%s_p%d", map, lod_version);
            fp = datAssetManager::open("tune/lod", filename.c_str(), "terrainlod", 1, true);
        }
        if (!fp) fp = datAssetManager::open("tune/lod", map, "terrainlod", 1, true);
        if (!fp) return;

        datAsciiTokenizer tok;
        tok.init(filename.c_str(), fp);
        char* buf = (char*)tok.buf;
        
        while (tok.getToken(buf, 64)) {
            if (strncmp(buf, "hirez8", 64) == 0) LODDist[0] = tok.readFloat();
            else if (strncmp(buf, "hirez4", 64) == 0) LODDist[1] = tok.readFloat();
            else if (strncmp(buf, "hirez2", 64) == 0) LODDist[2] = tok.readFloat();
            else if (strncmp(buf, "hirez1", 64) == 0) LODDist[3] = tok.readFloat();
            else if (strncmp(buf, "lorez1", 64) == 0) LODDist[4] = tok.readFloat();
            else if (strncmp(buf, "lorez2", 64) == 0) LODDist[5] = tok.readFloat();
            else if (strncmp(buf, "lorez4", 64) == 0) LODDist[6] = tok.readFloat();
            else if (strncmp(buf, "lorez8", 64) == 0) LODDist[7] = tok.readFloat();
        }

        delete fp;

        // ...
        // Omitting ineffectual code that just sets 'filename' based on 'whoknows' and lod_idx
        // and then doesn't use 'filename' again
        // ...

        squareLODDists();
    }
};