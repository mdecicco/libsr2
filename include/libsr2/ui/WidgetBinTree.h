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

            void init();
            void deinit();

            void reset();
            void destroyNode(Node* node);
            Node* FUN_00204760(bool p1, Node* node, const ui::NamedRef& widget);
            WidgetBinTree FUN_00204b68(const ui::NamedRef& widget);
            void FUN_00204de8(WidgetBinTree* p1, WidgetBinTree* p2);
            u32 FUN_00205458(const char* name);
            Node* FUN_0020b748(i32 value);
            WidgetBinTree findByName(const char* name);
            Node* findWidgetByName(const char* name);
            void findNearestNode0(WidgetBinTree* p1, const char* search);
            void findNearestNode1(WidgetBinTree* p1, const char* search);
            Node* getRoot() const;
        
        protected:
            Node* m_root;
            u32 m_someCount;
            u8 field_0x8;
    };
};