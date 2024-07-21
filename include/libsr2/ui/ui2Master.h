#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

#include <utils/Array.h>

namespace sr2 {
    class ui2Master : public ui2Widget {
        public:

            ui2Master(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster);
            virtual ~ui2Master();

            virtual void reset();
            virtual void draw();
            virtual void method_0x48();
            virtual bool loadWidget();
            virtual void method_0x58();
            virtual void method_0xc8(u32 p1);
            virtual void configureParser(datParser* parser);
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;
            virtual void method_0xe8();
            virtual void method_0xf0();

            void addRenderable(const ui::NamedRef& p1, i32 priority);
            void removeRenderable(const ui::NamedRef& p1);
            undefined4 insertToUnk1(const ui::NamedRef& p1);
            i32 deleteFromUnk1(const ui::NamedRef& p1);
        
        protected:
            struct Renderable {
                i32 priority;
                ui::NamedRef element;
            };

            utils::Array<Renderable> m_renderables;
            utils::Array<ui::NamedRef> m_testUnk1;
    };
};