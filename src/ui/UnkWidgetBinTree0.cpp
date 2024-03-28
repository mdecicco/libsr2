#include <libsr2/ui/UnkWidgetBinTree0.h>

namespace sr2 {
    UnkWidgetBinTree0::UnkWidgetBinTree0() {
        m_root = new Node;
        m_root->field_0x0 = 0;
        m_root->parent = nullptr;
        m_root->left = m_root;
        m_root->right = m_root;
        m_someCount = 0;
    }

    UnkWidgetBinTree0::~UnkWidgetBinTree0() {
        if (m_someCount) {
            destroyNode(m_root->parent);
            m_root->parent = nullptr;
            m_root->left = m_root;
            m_root->right = m_root;
            m_someCount = 0;
        }

        delete m_root;
        m_root = nullptr;
    }

    void UnkWidgetBinTree0::reset() {
        if (m_someCount) {
            destroyNode(m_root->parent);
            m_root->parent = nullptr;
            m_root->left = m_root;
            m_root->right = m_root;
            m_someCount = 0;
        }
    }

    void UnkWidgetBinTree0::destroyNode(Node* node) {
        if (!node) return;
        Node* cur = node->right;
        while (cur) {
            destroyNode(cur);
            Node* left = node->left;
            delete node;
            if (!left) break;
            cur = left->right;
            node = left;
        }
    }
    
    UnkWidgetBinTree0::Node* UnkWidgetBinTree0::FUN_0020b748(i32 value) {
        Node* result = m_root;
        Node* n = m_root->parent;

        while (n) {
            if (n->someSortingValue < value) n = n->right;
            else {
                result = n;
                n = n->left;
            }
        }

        if (result == m_root || value < result->someSortingValue) return m_root;
        return result;
    }
    
    UnkWidgetBinTree0::Node* UnkWidgetBinTree0::getRoot() const {
        return m_root;
    }
    
    void UnkWidgetBinTree0::setRoot(Node* root) {
        m_root = root;
    }
    
    void UnkWidgetBinTree0::setRoot(Node* root, u32 someCount) {
        m_root = root;
        m_someCount = someCount;
    }
};