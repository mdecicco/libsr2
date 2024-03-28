#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2String.h>

namespace sr2 {
    class UnkWidgetBinTree0 {
        public:
            struct Node {
                undefined4 field_0x0;
                Node* parent;
                Node* left;
                Node* right;
                i32 someSortingValue;
                undefined8 field_0x14;
                ui2String text;
            };

            UnkWidgetBinTree0();
            ~UnkWidgetBinTree0();

            void reset();
            void destroyNode(Node* node);
            Node* FUN_0020b748(i32 value);
            Node* getRoot() const;
            void setRoot(Node* root);
            void setRoot(Node* root, u32 someCount);
        
        protected:
            Node* m_root;
            u32 m_someCount;
    };
};