#include <libsr2/utilities/color.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/datParser.h>

namespace sr2 {
    u32 colorToU32(f32 r, f32 g, f32 b, f32 a) {
        u32 rB = (  u32(r * 255.0f + 0.5f) & 0xFFU);
        u32 gB = (  u32(g * 255.0f + 0.5f) & 0xFFU) << 8;
        u32 bB = (  u32(b * 255.0f + 0.5f) & 0xFFU) << 16;
        u32 aB = (((u32(a * 255.0f + 0.5f) & 0xFFU) + 1) >> 1) << 24;
        return rB | gB | bB | aB;
    }

    giColor::giColor() {
        colorVal = 0;
    }

    bool giColor::load() {
        u32 weatherVariant = 0;
        // u32 weatherVariant = Game::MissionWeatherVariant;

        if (weatherVariant) {
            std::string fn = format(
                "%s_%d",
                m_name ? m_name : "default",
                weatherVariant
            );
            if (datAssetManager::exists(directory(), fn.c_str(), file_type())) {
                set_name(fn.c_str());
            }
        }

        return parFileIO::load();
    }

    void giColor::after_load() {
        updateU32();
    }

    const char* giColor::directory() {
        return "tune/gameinfo";
    }

    const char* giColor::file_type() {
        return "giColor";
    }

    void giColor::loadAndUpdate() {
        load();
        updateU32();
    }



    giColorRGB::giColorRGB() {
        r = g = b = 0.0f;
    }

    datParserNode* giColorRGB::prep_parser(datParser* p) {
        return p->add(PARSE_TYPE::VEC3F, "v3NormalizedColor", &r, 1, nullptr);
    }

    const char* giColorRGB::file_type() {
        return "giColorRGB";
    }

    void giColorRGB::updateU32() {
        colorVal = colorToU32(r, g, b, 1.0f);
    }


    giColorRGBA::giColorRGBA() {
        r = g = b = a = 0.0f;
    }

    datParserNode* giColorRGBA::prep_parser(datParser* p) {
        return p->add(PARSE_TYPE::VEC4F, "v4NormalizedColor", &r, 1, nullptr);
    }

    const char* giColorRGBA::file_type() {
        return "giColorRGBA";
    }

    void giColorRGBA::updateU32() {
        colorVal = colorToU32(r, g, b, a);
    }
};