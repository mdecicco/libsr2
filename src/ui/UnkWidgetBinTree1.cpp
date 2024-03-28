#include <libsr2/ui/UnkWidgetBinTree1.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    UnkWidgetBinTree1::UnkWidgetBinTree1() {
        m_root = new Node;
        m_root->field_0x0 = false;
        m_root->parent = nullptr;
        m_root->left = m_root;
        m_root->right = m_root;
        m_someCount = 0;
    }

    UnkWidgetBinTree1::~UnkWidgetBinTree1() {
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

    void UnkWidgetBinTree1::reset() {
        if (m_someCount) {
            destroyNode(m_root->parent);
            m_root->parent = nullptr;
            m_root->left = m_root;
            m_root->right = m_root;
            m_someCount = 0;
        }
    }

    void UnkWidgetBinTree1::destroyNode(Node* node) {
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

    UnkWidgetBinTree1::Node* UnkWidgetBinTree1::FUN_0020b7b0(const UnkData& unk) {
        Node* result = m_root;
        Node* n = m_root->parent;

        while (n) {
            if (n->someSortingValue < unk.a) n = n->right;
            else {
                result = n;
                n = n->left;
            }
        }

        if (result == m_root || unk.a < result->someSortingValue) return m_root;
        return result;
    }
    
    UnkWidgetBinTree1::Node* UnkWidgetBinTree1::FUN_0020b818(Node* p1, Node* p2, const UnkData& unk) {
        Node* newNode = new Node;
        if (p2 == m_root || p1 || unk.a < p2->someSortingValue) {
            newNode->someSortingValue = unk.a;
            newNode->field_0x14 = unk.b;
            newNode->widget = unk.widget;

            p2->left = newNode;
            if (p2 == m_root) {
                p2->parent = newNode;
                m_root->right = newNode;
                newNode->parent = p2;
            } else {
                if (p2 != m_root->left) {
                    newNode->parent = p2;
                } else {
                    m_root->left = newNode;
                    newNode->parent = p2;
                }
            }
        } else {
            newNode->someSortingValue = unk.a;
            newNode->field_0x14 = unk.b;
            newNode->widget = unk.widget;

            p2->right = newNode;
            if (p2 == m_root->right) m_root->right = newNode;
            newNode->parent = p2;
        }

        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->field_0x0 = false;

        auto ppUVar7 = &m_root->parent;
        if (newNode == m_root->parent) {
            m_root->field_0x0 = true;
            m_someCount++;
            return newNode;
        } else {
            if (!p2->field_0x0) {
                auto pUVar8 = newNode->parent;
                auto pUVar6 = newNode;
                do {
                    auto pUVar5 = pUVar8->parent->left;
                    if (pUVar8 == pUVar5) {
                        pUVar8 = pUVar8->parent->right;
                        if (!pUVar8 || pUVar8->field_0x0) {
                            pUVar8 = pUVar6->parent;
                            pUVar5 = pUVar8->right;
                            if (pUVar6 == pUVar5) {
                                pUVar8->right = pUVar5->left;
                                if (pUVar5->left) {
                                    pUVar5->left->parent = pUVar8;
                                }
                                pUVar5->parent = pUVar8->parent;
                                if (pUVar8 == *ppUVar7) *ppUVar7 = pUVar5;
                                else {
                                    pUVar6 = pUVar8->parent;
                                    if (pUVar8 == pUVar6->left) pUVar6->left = pUVar5;
                                    else pUVar6->right = pUVar5;
                                }
                                pUVar5->left = pUVar8;
                                pUVar8->parent = pUVar5;
                                pUVar5 = pUVar8->parent;
                            } else {
                                pUVar5 = pUVar6->parent;
                                pUVar8 = pUVar6;
                            }
                            
                            pUVar5->field_0x0 = true;
                            pUVar8->parent->parent->field_0x0 = false;
                            pUVar6 = pUVar8->parent->parent;
                            pUVar5 = pUVar6->left;
                            pUVar6->left = pUVar5->right;
                            if (pUVar5->right) pUVar5->right->parent = pUVar6;
                            pUVar5->parent = pUVar6->parent;
                            if (pUVar6 == *ppUVar7) *ppUVar7 = pUVar5;
                            else {
                                if (pUVar6 == pUVar6->parent->right) pUVar6->parent->right = pUVar5;
                                else pUVar6->parent->left = pUVar5;
                            }
                            pUVar5->right = pUVar6;
                            pUVar6->parent = pUVar5;
                            pUVar6 = pUVar8;
                        } else {
                            pUVar5->field_0x0 = true;
                            pUVar8->field_0x0 = true;
                            pUVar6->parent->parent->field_0x0 = false;
                            pUVar6 = pUVar6->parent->parent;
                        }
                    } else {
                        if (pUVar5 && !pUVar5->field_0x0) {
                            pUVar8->field_0x0 = true;
                            pUVar5->field_0x0 = true;
                            pUVar6->parent->parent->field_0x0 = false;
                            pUVar6 = pUVar6->parent->parent;
                        } else {
                            pUVar8 = pUVar6->parent;
                            pUVar5 = pUVar8->left;
                            if (pUVar6 == pUVar5) {
                                pUVar8->left = pUVar5->right;
                                if (pUVar5->right) {
                                    pUVar5->right->parent = pUVar8;
                                }
                                pUVar5->parent = pUVar8->parent;
                                if (pUVar8 == *ppUVar7) {
                                    *ppUVar7 = pUVar5;
                                }
                                else {
                                    pUVar6 = pUVar8->parent;
                                    if (pUVar8 == pUVar6->right) {
                                        pUVar6->right = pUVar5;
                                    }
                                    else {
                                        pUVar6->left = pUVar5;
                                    }
                                }
                                pUVar5->right = pUVar8;
                                pUVar8->parent = pUVar5;
                                pUVar5 = pUVar8->parent;
                            }
                            else {
                                pUVar5 = pUVar6->parent;
                                pUVar8 = pUVar6;
                            }
                            pUVar5->field_0x0 = true;
                            pUVar8->parent->parent->field_0x0 = false;
                            pUVar6 = pUVar8->parent->parent;
                            pUVar5 = pUVar6->right;
                            pUVar6->right = pUVar5->left;
                            if (pUVar5->left) {
                                pUVar5->left->parent = pUVar6;
                            }
                            pUVar5->parent = pUVar6->parent;
                            if (pUVar6 == *ppUVar7) {
                                *ppUVar7 = pUVar5;
                            } else {
                                if (pUVar6 == pUVar6->parent->left) pUVar6->parent->left = pUVar5;
                                else pUVar6->parent->right = pUVar5;
                            }
                            pUVar5->left = pUVar6;
                            pUVar6->parent = pUVar5;
                            pUVar6 = pUVar8;
                        }
                    }

                    if (pUVar6 == *ppUVar7 || pUVar6->parent->field_0x0) {
                        (*ppUVar7)->field_0x0 = true;
                        m_someCount++;
                        return newNode;
                    }

                    pUVar8 = pUVar6->parent;
                } while( true );
            }
        }

        (*ppUVar7)->field_0x0 = true;
        m_someCount++;
        return newNode;
    }

    UnkWidgetBinTree1::Node* UnkWidgetBinTree1::FUN_0020bc00(const UnkData& unk) {
        Node* last = m_root;
        Node* n = m_root->parent;

        while (n) {
            last = n;
            if (unk.a < last->someSortingValue) n = last->left;
            else n = last->right;
        }

        return FUN_0020b818(n, last, unk);
    }
    
    void UnkWidgetBinTree1::maybeRemove(SOME_WIDGET_ENUM p1, const UnkData& unk) {
        // Code actually comes from FUN_0020a2b8, aka ui2Widget::method_0xb8
        Node *pUVar1;
        bool bVar4;
        bool BVar5;
        Node *pUVar6;
        Node **ppUVar7;
        Node *pUVar8;
        Node *pUVar9;
        Node *pUVar10;
        Node **ppUVar11;
        Node **ppUVar12;
        Node *pUVar13;
        Node *local_90;
        Node *local_80;
        Node *local_70;
        Node *local_60;

        local_80 = FUN_0020b7b0(unk);
        local_90 = local_80;
        while (true) {
            pUVar8 = m_root;
            bVar4 = false;
            if ((local_90 != pUVar8) && (bVar4 = true, local_90->someSortingValue != unk.a)) {
                bVar4 = false;
            }
            if (!bVar4) {
                return;
            }
            if (local_90->field_0x14 == p1) break;
            local_60 = local_90;
            pUVar8 = local_90->right;
            if (pUVar8 == NULL) {
                pUVar8 = local_90->parent;
                if (local_90 == pUVar8->right) {
                    do {
                        local_90 = pUVar8;
                        pUVar8 = local_90->parent;
                    } while (local_90 == pUVar8->right);
                }
                if (local_90->right != pUVar8) {
                    local_90 = pUVar8;
                }
            }
            else {
                pUVar13 = pUVar8->left;
                while (local_90 = pUVar8, pUVar13 != NULL) {
                    pUVar8 = pUVar8->left;
                    pUVar13 = pUVar8->left;
                }
            }
        }
        
        local_80 = local_90;
        ppUVar12 = &pUVar8->right;
        local_70 = local_90;
        ppUVar11 = &pUVar8->parent;
        ppUVar7 = &pUVar8->left;
        pUVar8 = local_90->left;
        pUVar13 = local_90;
        if (pUVar8 == NULL) {
            pUVar8 = local_90->right;
        }
        else {
            pUVar10 = local_90->right;
            if (pUVar10 != NULL) {
                if (pUVar10->left != NULL) {
                    for (pUVar10 = pUVar10->left; pUVar10->left != NULL; pUVar10 = pUVar10->left) {
                    }
                }
                pUVar8 = pUVar10->right;
                pUVar13 = pUVar10;
            }
        }
        if (pUVar13 == local_90) {
            pUVar10 = local_90->parent;
            if (pUVar8 != NULL) {
                pUVar8->parent = pUVar10;
            }
            if (*ppUVar11 == local_90) {
                *ppUVar11 = pUVar8;
            }
            else {
                pUVar6 = local_90->parent;
                if (pUVar6->left == local_90) {
                    pUVar6->left = pUVar8;
                }
                else {
                    pUVar6->right = pUVar8;
                }
            }
            if (*ppUVar7 == local_90) {
                if (local_90->right == NULL) {
                    *ppUVar7 = local_90->parent;
                }
                else {
                    pUVar6 = pUVar8;
                    if (pUVar8->left != NULL) {
                        for (pUVar6 = pUVar8->left; pUVar6->left != NULL; pUVar6 = pUVar6->left) {
                        }
                    }
                    *ppUVar7 = pUVar6;
                }
                pUVar6 = *ppUVar12;
            }
            else {
                pUVar6 = *ppUVar12;
            }
            if (pUVar6 != local_90) {
                BVar5 = pUVar13->field_0x0;
                goto LAB_0020a4b4;
            }
            if (local_90->left == NULL) {
                *ppUVar12 = local_90->parent;
            }
            else {
                pUVar6 = pUVar8;
                if (pUVar8->right != NULL) {
                    for (pUVar6 = pUVar8->right; pUVar6->right != NULL; pUVar6 = pUVar6->right) {
                    }
                }
                *ppUVar12 = pUVar6;
            }
        }
        else {
            local_90->left->parent = pUVar13;
            pUVar13->left = local_90->left;
            pUVar10 = pUVar13;
            if (pUVar13 != local_90->right) {
                pUVar10 = pUVar13->parent;
                if (pUVar8 != NULL) {
                    pUVar8->parent = pUVar10;
                }
                pUVar13->parent->left = pUVar8;
                pUVar13->right = local_90->right;
                local_90->right->parent = pUVar13;
            }
            if (*ppUVar11 == local_90) {
                *ppUVar11 = pUVar13;
            }
            else {
                pUVar6 = local_90->parent;
                if (pUVar6->left == local_90) {
                    pUVar6->left = pUVar13;
                }
                else {
                    pUVar6->right = pUVar13;
                }
            }
            BVar5 = pUVar13->field_0x0;
            pUVar13->parent = local_90->parent;
            pUVar13->field_0x0 = local_90->field_0x0;
            local_90->field_0x0 = BVar5;
            pUVar13 = local_90;
        }
        BVar5 = pUVar13->field_0x0;
    LAB_0020a4b4:
        if (BVar5 != false) {
            pUVar6 = *ppUVar11;
            while (pUVar8 != pUVar6) {
                if (pUVar8 == NULL) {
                    pUVar6 = pUVar10->left;
                }
                else {
                    if (pUVar8->field_0x0 != true) break;
                    pUVar6 = pUVar10->left;
                }
                if (pUVar8 == pUVar6) {
                    pUVar6 = pUVar10->right;
                    if (pUVar6->field_0x0 == false) {
                        pUVar6->field_0x0 = true;
                        pUVar10->field_0x0 = false;
                        pUVar6 = pUVar10->right;
                        pUVar10->right = pUVar6->left;
                        if (pUVar6->left != NULL) {
                            pUVar6->left->parent = pUVar10;
                        }
                        pUVar6->parent = pUVar10->parent;
                        if (pUVar10 == *ppUVar11) {
                            *ppUVar11 = pUVar6;
                        }
                        else {
                            pUVar9 = pUVar10->parent;
                            if (pUVar10 == pUVar9->left) {
                                pUVar9->left = pUVar6;
                            }
                            else {
                                pUVar9->right = pUVar6;
                            }
                        }
                        pUVar6->left = pUVar10;
                        pUVar10->parent = pUVar6;
                        pUVar6 = pUVar10->right;
                        pUVar9 = pUVar6->left;
                    }
                    else {
                        pUVar9 = pUVar6->left;
                    }
                    pUVar1 = pUVar6->right;
                    if ((pUVar9 != NULL) && (pUVar9->field_0x0 != true)) {
    LAB_0020a564:
                        if ((pUVar1 == NULL) || (pUVar1->field_0x0 == true)) {
                            if (pUVar9 != NULL) {
                                pUVar9->field_0x0 = true;
                            }
                            pUVar9 = pUVar6->left;
                            pUVar6->field_0x0 = false;
                            pUVar6->left = pUVar9->right;
                            if (pUVar9->right != NULL) {
                                pUVar9->right->parent = pUVar6;
                            }
                            pUVar9->parent = pUVar6->parent;
                            if (pUVar6 == *ppUVar11) {
                                *ppUVar11 = pUVar9;
                            }
                            else {
                                pUVar1 = pUVar6->parent;
                                if (pUVar6 == pUVar1->right) {
                                    pUVar1->right = pUVar9;
                                }
                                else {
                                    pUVar1->left = pUVar9;
                                }
                            }
                            pUVar9->right = pUVar6;
                            pUVar6->parent = pUVar9;
                            pUVar6 = pUVar10->right;
                            BVar5 = pUVar10->field_0x0;
                        }
                        else {
                            BVar5 = pUVar10->field_0x0;
                        }
                        pUVar6->field_0x0 = BVar5;
                        pUVar10->field_0x0 = true;
                        if (pUVar6->right != NULL) {
                            pUVar6->right->field_0x0 = true;
                        }
                        pUVar6 = pUVar10->right;
                        pUVar10->right = pUVar6->left;
                        if (pUVar6->left != NULL) {
                            pUVar6->left->parent = pUVar10;
                        }
                        pUVar6->parent = pUVar10->parent;
                        if (pUVar10 == *ppUVar11) {
                            *ppUVar11 = pUVar6;
                        }
                        else {
                            pUVar9 = pUVar10->parent;
                            if (pUVar10 == pUVar9->left) {
                                pUVar9->left = pUVar6;
                            }
                            else {
                                pUVar9->right = pUVar6;
                            }
                        }
                        pUVar6->left = pUVar10;
                        pUVar10->parent = pUVar6;
                        break;
                    }
                    if (pUVar1 == NULL) {
                        pUVar6->field_0x0 = false;
                    }
                    else {
                        if (pUVar1->field_0x0 != true) goto LAB_0020a564;
                        pUVar6->field_0x0 = false;
                    }
                }
                else {
                    if (pUVar6->field_0x0 == false) {
                        pUVar6->field_0x0 = true;
                        pUVar10->field_0x0 = false;
                        pUVar6 = pUVar10->left;
                        pUVar10->left = pUVar6->right;
                        if (pUVar6->right != NULL) {
                            pUVar6->right->parent = pUVar10;
                        }
                        pUVar6->parent = pUVar10->parent;
                        if (pUVar10 == *ppUVar11) {
                            *ppUVar11 = pUVar6;
                        }
                        else {
                            pUVar9 = pUVar10->parent;
                            if (pUVar10 == pUVar9->right) {
                                pUVar9->right = pUVar6;
                            }
                            else {
                                pUVar9->left = pUVar6;
                            }
                        }
                        pUVar6->right = pUVar10;
                        pUVar10->parent = pUVar6;
                        pUVar6 = pUVar10->left;
                        pUVar9 = pUVar6->right;
                    }
                    else {
                        pUVar9 = pUVar6->right;
                    }
                    pUVar1 = pUVar6->left;
                    if ((pUVar9 != NULL) && (pUVar9->field_0x0 != true)) {
    LAB_0020a6e0:
                        if ((pUVar1 == NULL) || (pUVar1->field_0x0 == true)) {
                            if (pUVar9 != NULL) {
                                pUVar9->field_0x0 = true;
                            }
                            pUVar9 = pUVar6->right;
                            pUVar6->field_0x0 = false;
                            pUVar6->right = pUVar9->left;
                            if (pUVar9->left != NULL) {
                                pUVar9->left->parent = pUVar6;
                            }
                            pUVar9->parent = pUVar6->parent;
                            if (pUVar6 == *ppUVar11) {
                                *ppUVar11 = pUVar9;
                            }
                            else {
                                pUVar1 = pUVar6->parent;
                                if (pUVar6 == pUVar1->left) {
                                    pUVar1->left = pUVar9;
                                }
                                else {
                                    pUVar1->right = pUVar9;
                                }
                            }
                            pUVar9->left = pUVar6;
                            pUVar6->parent = pUVar9;
                            pUVar6 = pUVar10->left;
                            BVar5 = pUVar10->field_0x0;
                        }
                        else {
                            BVar5 = pUVar10->field_0x0;
                        }
                        pUVar6->field_0x0 = BVar5;
                        pUVar10->field_0x0 = true;
                        if (pUVar6->left != NULL) {
                            pUVar6->left->field_0x0 = true;
                        }
                        pUVar6 = pUVar10->left;
                        pUVar10->left = pUVar6->right;
                        if (pUVar6->right != NULL) {
                            pUVar6->right->parent = pUVar10;
                        }
                        pUVar6->parent = pUVar10->parent;
                        if (pUVar10 == *ppUVar11) {
                            *ppUVar11 = pUVar6;
                        }
                        else {
                            pUVar9 = pUVar10->parent;
                            if (pUVar10 == pUVar9->right) {
                                pUVar9->right = pUVar6;
                            }
                            else {
                                pUVar9->left = pUVar6;
                            }
                        }
                        pUVar6->right = pUVar10;
                        pUVar10->parent = pUVar6;
                        break;
                    }
                    if (pUVar1 == NULL) {
                        pUVar6->field_0x0 = false;
                    }
                    else {
                        if (pUVar1->field_0x0 != true) goto LAB_0020a6e0;
                        pUVar6->field_0x0 = false;
                    }
                }
                pUVar8 = pUVar10;
                pUVar10 = pUVar10->parent;
                pUVar6 = *ppUVar11;
            }
            if (pUVar8 != NULL) {
                pUVar8->field_0x0 = true;
            }
        }
        delete pUVar13;
        m_someCount--;
    }

    UnkWidgetBinTree1::Node* UnkWidgetBinTree1::getRoot() const {
        return m_root;
    }
};