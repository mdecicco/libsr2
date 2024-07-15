#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>

#include <utils/Array.h>

namespace sr2 {
    class gfxTexture;

    class ui2Image : public ui2Visual {
        public:
            ui2Image(const char* name, const char* filename, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master);
            ui2Image(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master);
            virtual ~ui2Image();

            void init(const char* filename, i32 p2);
            void loadImage(const char* filename);
            void FUN_001f5db0(i32 p1);

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void draw();

            void setSize(i32 w, i32 h);
        
        protected:
            vec2i m_displaySize;
            utils::Array<gfxTexture*> m_textures;
    };
};