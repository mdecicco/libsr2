#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>

namespace sr2 {
    class ui2Text : public ui2Visual {
        public:
            ui2Text(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master);
            virtual ~ui2Text();

            virtual void setBounds(i32 w, i32 h);
            virtual void setBoundsFromPos(i32 x, i32 y);
            virtual void setAlignment(i32 align);
    };
};