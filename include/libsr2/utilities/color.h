#pragma once
#include <libsr2/types.h>
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class giColor : public parFileIO {
        public:
            giColor();

            virtual bool load();
            virtual void afterLoad();
            virtual const char* getDirectory();
            virtual const char* getFileType();
            virtual void loadAndUpdate();
            virtual void updateU32() = 0;

            u32 colorVal;
    };

    class giColorRGB : public giColor {
        public:
            giColorRGB();

            virtual datParserNode* prepParser(datParser* p);
            virtual const char* getFileType();
            virtual void updateU32();

            f32 r, g, b;
    };

    class giColorRGBA : public giColor {
        public:
            giColorRGBA();
            
            virtual datParserNode* prepParser(datParser* p);
            virtual const char* getFileType();
            virtual void updateU32();

            f32 r, g, b, a;
    };
};