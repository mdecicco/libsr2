#pragma once
#include <libsr2/types.h>
#include <libsr2/states/GameState.h>

namespace sr2 {
    class ui2Widget;

    struct StoredWidget {
        ui2Widget* widget;
        void* some_func_table;
    };

    struct WidgetStorage {
        StoredWidget widgets[83];
    };

    class InMenuState : public GameState {
        public:
            InMenuState();
            ~InMenuState();

            virtual void Enter(GameEngine* eng);
            virtual void method_0x80(GameEngine* eng);

            WidgetStorage* field_0x8;

        protected:
            void set_widget(u16 idx, ui2Widget* widget, StoredWidget* unk);
    };
};