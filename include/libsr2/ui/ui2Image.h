#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>
#include <libsr2/gfx/ui.h>

#include <utils/Array.h>

namespace sr2 {
    class gfxTexture;

    class ui2Image : public ui2Visual {
        public:
            ui2Image(const char* name, const char* filename, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master);
            virtual ~ui2Image();

            void init(const char* filename, i32 renderPriority);
            void loadImage(const char* filename);

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void draw();

            void setFrameRate(f32 frameRate);
            void FUN_001f5db0(i32 p1);
            void FUN_001f5df0(i32 p1);
            void setSize(i32 w, i32 h);
        
        protected:
            vec2i m_displaySize;
            utils::Array<gfxTexture*> m_textures;
            uiRenderer::element* m_elem;
            u32 m_frameIdx;
            u32 m_animState;
            u32 m_animMode;
            f32 m_animFrameRate;
            f32 m_animStartTime;
            f32 m_animElapsedTime;
    };
};