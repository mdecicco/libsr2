#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>
#include <libsr2/gfx/ui.h>

namespace sr2 {
    class gfxTexture;
    class mpeg;

    class ui2Movie : public ui2Visual {
        public:
            ui2Movie(
                const char* name,
                const char* filename,
                undefined4 p3,
                i32 x,
                i32 y,
                i32 renderPriority,
                i32* p7,
                const WidgetRef<ui2Master>& master
            );
            ~ui2Movie();

            void init(const char* filename, undefined4 p2, i32 p3);

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void method_0x48();
            virtual void draw();
            virtual void configureParser(datParser* parser);
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;
            
            void setFilename(const char* filename);
            void FUN_001f3210();
            void FUN_001f31d0();
            void FUN_001f3340(undefined4 p1);
            void FUN_001f3360(i32 p1);
            void FUN_001f35f0();
            void FUN_001f3658();
            void FUN_001f3328(undefined4 p1);
            void FUN_001f3350(undefined4 p1);
            void getFilePath(ui2String& out);
            void stopMovie();

            static bool FUN_001f29f0(u32 width, u32 height);
            static undefined4 FUN_001f2ae0();
            static undefined4 DAT_00362d14;
            static u32 DAT_00362d10;
        
        protected:
            vec2i m_displaySize;
            vec2i m_frameSize;
            mpeg* m_mpeg;
            uiRenderer::element* m_elem;
            gfxTexture* m_texture;
            ui2String m_filename;

            undefined4 field_0xac;
            undefined4 field_0xb0;
            i32* field_0xb4;
            undefined4 field_0xb8;
            undefined4 field_0xbc;
            undefined4 field_0xc0;
            undefined4 field_0xc4;
            i32 field_0xd8;
    };
};