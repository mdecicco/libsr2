#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2String.h>
#include <libsr2/ui/WidgetRef.h>

namespace sr2 {
    enum class WidgetEventType : u32;
    class ui2Widget;
    class ui2EventData;
    typedef void (ui2Widget::*WidgetEventCallback)(const WidgetRef<ui2Widget>&, WidgetEventType, const WidgetRef<ui2EventData>&);
    
    class UnkWidgetBinTree0 {
        public:
            struct UnkData {
                WidgetEventType event;

                // This was incredibly difficult to figure out
                // bask in my reverse engineering prowess
                WidgetEventCallback callback;

                ui2String str;
            };

            struct Node {
                undefined4 field_0x0;
                Node* parent;
                Node* left;
                Node* right;
                UnkData unk;
            };

            UnkWidgetBinTree0();
            ~UnkWidgetBinTree0();

            void reset();
            void destroyNode(Node* node);
            Node* FUN_0020b2e8(Node* p1, Node* p2, const UnkData& unk);
            Node* FUN_0020b6e8(const UnkData& unk);
            Node* FUN_0020b748(WidgetEventType value);
            Node* getRoot() const;
        
        protected:
            Node* m_root;
            u32 m_someCount;
    };
};