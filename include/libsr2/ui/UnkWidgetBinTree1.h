#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/WidgetRef.h>

namespace sr2 {
    class ui2Widget;
    enum SOME_WIDGET_ENUM : u32;

    class UnkWidgetBinTree1 {
        public:
            struct Node {
                bool field_0x0;
                Node* parent;
                Node* left;
                Node* right;
                SOME_WIDGET_ENUM someSortingValue;
                SOME_WIDGET_ENUM field_0x14;
                ui::BaseRef widget;
            };

            struct UnkData {
                SOME_WIDGET_ENUM a;
                SOME_WIDGET_ENUM b;
                ui2WidgetBase* widget;
            };

            UnkWidgetBinTree1();
            ~UnkWidgetBinTree1();

            void reset();
            void destroyNode(Node* node);
            Node* FUN_0020b7b0(const UnkData& unk);
            Node* FUN_0020b818(Node* p1, Node* p2, const UnkData& unk);
            Node* FUN_0020bc00(const UnkData& unk);
            void maybeRemove(SOME_WIDGET_ENUM p1, const UnkData& unk);
            Node* getRoot() const;
        
        protected:
            Node* m_root;
            u32 m_someCount;
    };
};