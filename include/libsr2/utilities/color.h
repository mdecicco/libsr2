#pragma once
#include <libsr2/types.h>
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class giColor : public parFileIO {
        public:
            giColor();

            virtual bool load();
            virtual void after_load();
            virtual const char* directory();
            virtual const char* file_type();
            virtual void loadAndUpdate();
            virtual void updateU32() = 0;

            u32 colorVal;
    };

    class giColorRGB : public giColor {
        public:
            giColorRGB();

            virtual datParserNode* prep_parser(datParser* p);
            virtual const char* file_type();
            virtual void updateU32();

            f32 r, g, b;
    };

    class giColorRGBA : public giColor {
        public:
            giColorRGBA();
            
            virtual datParserNode* prep_parser(datParser* p);
            virtual const char* file_type();
            virtual void updateU32();

            f32 r, g, b, a;
    };
};