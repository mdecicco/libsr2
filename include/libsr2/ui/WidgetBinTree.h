#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2String.h>
#include <libsr2/ui/ui2WidgetBase.h>
#include <libsr2/ui/WidgetRef.h>

namespace sr2 {
    class WidgetBinTree {
        public:
            struct Node {
                undefined4 field_0x0;
                Node* parent;
                Node* left;
                Node* right;
                i32 someSortingValue;
                undefined8 field_0x14;
                ui2String text;
                ui::BaseRef widget;
            };

            WidgetBinTree();
            ~WidgetBinTree();

            void reset();
            void destroyNode(Node* node);
            WidgetBinTree FUN_00204b68(const char* name);
            u32 FUN_00205458(const char* name);
            Node* FUN_0020b748(i32 value);
            WidgetBinTree findByName(const char* name);
            Node* findWidgetByName(const char* name);
            Node* getRoot() const;
            void setRoot(Node* root);
            void setRoot(Node* root, u32 someCount);
        
        protected:
            Node* m_root;
            u32 m_someCount;
            u8 field_0x8;
    };
};