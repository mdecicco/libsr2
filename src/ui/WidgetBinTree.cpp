#include <libsr2/ui/WidgetBinTree.h>

#include <string.h>

namespace sr2 {
    WidgetBinTree::WidgetBinTree() {
        m_root = new Node;
        m_root->field_0x0 = 0;
        m_root->parent = nullptr;
        m_root->left = m_root;
        m_root->right = m_root;
        m_someCount = 0;
        field_0x8 = 0;
    }

    WidgetBinTree::~WidgetBinTree() {
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

    void WidgetBinTree::reset() {
        if (m_someCount) {
            destroyNode(m_root->parent);
            m_root->parent = nullptr;
            m_root->left = m_root;
            m_root->right = m_root;
            m_someCount = 0;
        }
    }

    void WidgetBinTree::destroyNode(Node* node) {
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

    WidgetBinTree WidgetBinTree::FUN_00204b68(const char* name) {
        WidgetBinTree result;
        return result;
    }
    
    u32 WidgetBinTree::FUN_00205458(const char* name) {
        // todo
        return 0;
    }
    
    WidgetBinTree::Node* WidgetBinTree::FUN_0020b748(i32 value) {
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
    
    WidgetBinTree WidgetBinTree::findByName(const char* name) {
        WidgetBinTree result;
        delete result.m_root;
        result.m_root = nullptr;

        Node* found = m_root;
        Node* n = m_root->parent;

        while (n) {
            if (strcmp(n->text.get(), name) < 0) n = n->left;
            else {
                found = n;
                n = n->right;
            }
        }

        if (found != m_root && strcmp(name, found->text.get()) > -1) {
            result.m_root = found;
            return result;
        }

        result.m_root = m_root;
        return result;
    }
    
    WidgetBinTree::Node* WidgetBinTree::findWidgetByName(const char* name) {
        Node* found = m_root;
        Node* n = m_root->parent;

        while (n) {
            if (strcmp(n->text.get(), name) < 0) n = n->left;
            else {
                found = n;
                n = n->right;
            }
        }

        if (found != m_root && strcmp(name, found->text.get()) < 0) {
            return m_root;
        }

        return found;
    }
    
    WidgetBinTree::Node* WidgetBinTree::getRoot() const {
        return m_root;
    }
    
    void WidgetBinTree::setRoot(Node* root) {
        m_root = root;
    }
    
    void WidgetBinTree::setRoot(Node* root, u32 someCount) {
        m_root = root;
        m_someCount = someCount;
    }
};