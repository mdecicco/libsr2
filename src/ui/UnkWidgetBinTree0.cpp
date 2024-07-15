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

    UnkWidgetBinTree0::Node* UnkWidgetBinTree0::FUN_0020b2e8(UnkWidgetBinTree0::Node* p1, UnkWidgetBinTree0::Node* p2, const UnkWidgetBinTree0::UnkData& unk) {
        Node* newNode = new Node;
        newNode->unk = unk;

        if (p2 == m_root || p1 || unk.event < p2->unk.event) {
            newNode->left = newNode;
            if (p2 == m_root) {
                p2->parent = newNode;
                m_root->right = newNode;
            } else if (p2 == m_root->left) {
                m_root->left = newNode;
            }
        } else {
            p2->right = newNode;
            if (p2 == m_root->right) m_root->right = newNode;
        }
        
        newNode->parent = p2;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->field_0x0 = 0;

        Node** ppUVar4 = &m_root->parent;
        Node* pUVar5 = m_root;
        if (newNode == m_root->parent) {
            pUVar5 = *ppUVar4;
        } else {
            if (p2->field_0x0 == 0) {
                pUVar5 = newNode->parent;
                Node* pUVar3 = newNode;
                do {
                    Node* pUVar2 = pUVar5->parent->left;
                    if (pUVar5 == pUVar2) {
                        pUVar5 = pUVar5->parent->right;
                        if (!pUVar5) {
                            pUVar5 = pUVar3->parent;
    LAB_0020b4dc:
                            pUVar2 = pUVar5->right;
                            if (pUVar3 == pUVar2) {
                                pUVar5->right = pUVar2->left;
                                if (pUVar2->left) {
                                    pUVar2->left->parent = pUVar5;
                                }
                                pUVar2->parent = pUVar5->parent;
                                if (pUVar5 == *ppUVar4) {
                                    *ppUVar4 = pUVar2;
                                } else {
                                    pUVar3 = pUVar5->parent;
                                    if (pUVar5 == pUVar3->left) {
                                        pUVar3->left = pUVar2;
                                    } else {
                                        pUVar3->right = pUVar2;
                                    }
                                }
                                pUVar2->left = pUVar5;
                                pUVar5->parent = pUVar2;
                                pUVar2 = pUVar5->parent;
                            } else {
                                pUVar2 = pUVar3->parent;
                                pUVar5 = pUVar3;
                            }

                            pUVar2->field_0x0 = 1;
                            pUVar5->parent->parent->field_0x0 = 0;
                            pUVar3 = pUVar5->parent->parent;
                            pUVar2 = pUVar3->left;
                            pUVar3->left = pUVar2->right;
                            if (pUVar2->right) {
                                pUVar2->right->parent = pUVar3;
                            }
                            pUVar2->parent = pUVar3->parent;
                            if (pUVar3 == *ppUVar4) {
                                *ppUVar4 = pUVar2;
                            } else {
                                if (pUVar3 == pUVar3->parent->right) {
                                    pUVar3->parent->right = pUVar2;
                                } else {
                                    pUVar3->parent->left = pUVar2;
                                }
                            }
                            pUVar2->right = pUVar3;
                            goto LAB_0020b68c;
                        }

                        if (pUVar5->field_0x0 != 0) {
                            pUVar5 = pUVar3->parent;
                            goto LAB_0020b4dc;
                        }

                        pUVar2->field_0x0 = 1;
                        pUVar5->field_0x0 = 1;
    LAB_0020b5b8:
                        pUVar3->parent->parent->field_0x0 = 0;
                        pUVar3 = pUVar3->parent->parent;
                    }
                    else {
                        if (!pUVar2) {
                            pUVar5 = pUVar3->parent;
                        } else {
                            if (pUVar2->field_0x0 == 0) {
                                pUVar5->field_0x0 = 1;
                                pUVar2->field_0x0 = 1;
                                goto LAB_0020b5b8;
                            }
                            pUVar5 = pUVar3->parent;
                        }
                        pUVar2 = pUVar5->left;
                        if (pUVar3 == pUVar2) {
                            pUVar5->left = pUVar2->right;
                            if (pUVar2->right) {
                                pUVar2->right->parent = pUVar5;
                            }
                            pUVar2->parent = pUVar5->parent;
                            if (pUVar5 == *ppUVar4) {
                                *ppUVar4 = pUVar2;
                            }
                            else {
                                pUVar3 = pUVar5->parent;
                                if (pUVar5 == pUVar3->right) {
                                    pUVar3->right = pUVar2;
                                }
                                else {
                                    pUVar3->left = pUVar2;
                                }
                            }
                            pUVar2->right = pUVar5;
                            pUVar5->parent = pUVar2;
                            pUVar2 = pUVar5->parent;
                        }
                        else {
                            pUVar2 = pUVar3->parent;
                            pUVar5 = pUVar3;
                        }
                        pUVar2->field_0x0 = 1;
                        pUVar5->parent->parent->field_0x0 = 0;
                        pUVar3 = pUVar5->parent->parent;
                        pUVar2 = pUVar3->right;
                        pUVar3->right = pUVar2->left;
                        if (pUVar2->left) {
                            pUVar2->left->parent = pUVar3;
                        }
                        pUVar2->parent = pUVar3->parent;
                        if (pUVar3 == *ppUVar4) {
                            *ppUVar4 = pUVar2;
                        } else {
                            if (pUVar3 == pUVar3->parent->left) {
                                pUVar3->parent->left = pUVar2;
                            } else {
                                pUVar3->parent->right = pUVar2;
                            }
                        }
                        pUVar2->left = pUVar3;
    LAB_0020b68c:
                        pUVar3->parent = pUVar2;
                        pUVar3 = pUVar5;
                    }

                    if (pUVar3 == *ppUVar4) break;
                    if (pUVar3->parent->field_0x0 != 0) break;

                    pUVar5 = pUVar3->parent;
                } while (true);
            }
            pUVar5 = *ppUVar4;
        }

        pUVar5->field_0x0 = 1;
        m_someCount++;

        return newNode;
    }

    UnkWidgetBinTree0::Node* UnkWidgetBinTree0::FUN_0020b6e8(const UnkWidgetBinTree0::UnkData& unk) {
        Node* last = m_root;
        Node* n = m_root->parent;
        while (n) {
            last = n;

            if (unk.event < last->unk.event) n = n->left;
            else n = n->right;
        }

        return FUN_0020b2e8(n, last, unk);
    }
    
    UnkWidgetBinTree0::Node* UnkWidgetBinTree0::FUN_0020b748(WidgetEventType value) {
        Node* result = m_root;
        Node* n = m_root->parent;

        while (n) {
            if (n->unk.event < value) n = n->right;
            else {
                result = n;
                n = n->left;
            }
        }

        if (result == m_root || value < result->unk.event) return m_root;
        return result;
    }
    
    UnkWidgetBinTree0::Node* UnkWidgetBinTree0::getRoot() const {
        return m_root;
    }
};