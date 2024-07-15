#include <libsr2/ui/WidgetBinTree.h>
#include <libsr2/ui/ui2Widget.h>

#include <string.h>

namespace sr2 {
    WidgetBinTree::WidgetBinTree() {
        m_root = nullptr;
        m_someCount = 0;
        field_0x8 = 0;
    }

    WidgetBinTree::~WidgetBinTree() {
    }

    void WidgetBinTree::init() {
        if (m_root) return;
        
        m_root = new Node;
        m_root->field_0x0 = 0;
        m_root->parent = nullptr;
        m_root->left = m_root;
        m_root->right = m_root;
    }

    void WidgetBinTree::deinit() {
        if (m_root) delete m_root;
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

    WidgetBinTree::Node* WidgetBinTree::FUN_00204760(bool p1, Node* node, const ui::NamedRef& widget) {
        Node *pWVar3;
        Node *newEle = new Node;
        Node *pWVar5;
        Node *pWVar6;
        Node **ppWVar7;
        Node *tmp;
        const char* name = widget->getName();
        
        if (node == m_root || p1 || strcmp(name, node->text.get()) < 0) {
            newEle->text.set(name);
            newEle->widget = widget;
            node->right = newEle;
            tmp = m_root;
            if (node == tmp) {
                node->parent = newEle;
                m_root->left = newEle;
            } else {
                if (node != tmp->right) {
                    newEle->parent = node;
                    goto LAB_002048e8;
                }
                tmp->right = newEle;
            }
        } else {
            newEle->text.set(name);
            newEle->widget = widget;
            node->left = newEle;
            if (node == m_root->left) {
                m_root->left = newEle;
            }
        }

        newEle->parent = node;

    LAB_002048e8:
        newEle->right = NULL;
        newEle->left = NULL;
        tmp = m_root;
        newEle->field_0x0 = 0;
        ppWVar7 = &tmp->parent;

        if (newEle == tmp->parent) {
    LAB_00204b24:
            tmp = *ppWVar7;
        }
        else {
            if (node->field_0x0 == 0) {
                tmp = newEle->parent;
                pWVar6 = newEle;
                do {
                    pWVar5 = tmp->parent->right;
                    if (tmp == pWVar5) {
                        tmp = tmp->parent->left;
                        if (tmp == NULL) {
                            tmp = pWVar6->parent;
    LAB_00204954:
                            pWVar5 = tmp->left;
                            if (pWVar6 == pWVar5) {
                                tmp->left = pWVar5->right;
                                if (pWVar5->right != NULL) {
                                    pWVar5->right->parent = tmp;
                                }
                                pWVar5->parent = tmp->parent;
                                if (tmp == *ppWVar7) {
                                    *ppWVar7 = pWVar5;
                                }
                                else {
                                    pWVar6 = tmp->parent;
                                    if (tmp == pWVar6->right) {
                                        pWVar6->right = pWVar5;
                                    }
                                    else {
                                        pWVar6->left = pWVar5;
                                    }
                                }
                                pWVar5->right = tmp;
                                tmp->parent = pWVar5;
                                pWVar5 = tmp->parent;
                            }
                            else {
                                pWVar5 = pWVar6->parent;
                                tmp = pWVar6;
                            }
                            pWVar5->field_0x0 = 1;
                            tmp->parent->parent->field_0x0 = 0;
                            pWVar6 = tmp->parent->parent;
                            pWVar5 = pWVar6->right;
                            pWVar6->right = pWVar5->left;
                            if (pWVar5->left != NULL) {
                                pWVar5->left->parent = pWVar6;
                            }
                            pWVar5->parent = pWVar6->parent;
                            if (pWVar6 == *ppWVar7) {
                                *ppWVar7 = pWVar5;
                            }
                            else {
                                pWVar3 = pWVar6->parent;
                                if (pWVar6 == pWVar3->left) {
                                    pWVar3->left = pWVar5;
                                }
                                else {
                                    pWVar3->right = pWVar5;
                                }
                            }
                            pWVar5->left = pWVar6;
                            goto LAB_00204b04;
                        }
                        if (tmp->field_0x0 != 0) {
                            tmp = pWVar6->parent;
                            goto LAB_00204954;
                        }
                        pWVar5->field_0x0 = 1;
                        tmp->field_0x0 = 1;
    LAB_00204a30:
                        pWVar6->parent->parent->field_0x0 = 0;
                        pWVar6 = pWVar6->parent->parent;
                    }
                    else {
                        if (pWVar5 == NULL) {
                            tmp = pWVar6->parent;
                        }
                        else {
                            if (pWVar5->field_0x0 == 0) {
                                tmp->field_0x0 = 1;
                                pWVar5->field_0x0 = 1;
                                goto LAB_00204a30;
                            }
                            tmp = pWVar6->parent;
                        }
                        pWVar5 = tmp->right;
                        if (pWVar6 == pWVar5) {
                            tmp->right = pWVar5->left;
                            if (pWVar5->left != NULL) {
                                pWVar5->left->parent = tmp;
                            }
                            pWVar5->parent = tmp->parent;
                            if (tmp == *ppWVar7) {
                                *ppWVar7 = pWVar5;
                            }
                            else {
                                pWVar6 = tmp->parent;
                                if (tmp == pWVar6->left) {
                                    pWVar6->left = pWVar5;
                                }
                                else {
                                    pWVar6->right = pWVar5;
                                }
                            }
                            pWVar5->left = tmp;
                            tmp->parent = pWVar5;
                            pWVar5 = tmp->parent;
                        }
                        else {
                            pWVar5 = pWVar6->parent;
                            tmp = pWVar6;
                        }
                        pWVar5->field_0x0 = 1;
                        tmp->parent->parent->field_0x0 = 0;
                        pWVar6 = tmp->parent->parent;
                        pWVar5 = pWVar6->left;
                        pWVar6->left = pWVar5->right;
                        if (pWVar5->right != NULL) {
                            pWVar5->right->parent = pWVar6;
                        }
                        pWVar5->parent = pWVar6->parent;
                        if (pWVar6 == *ppWVar7) {
                            *ppWVar7 = pWVar5;
                        }
                        else {
                            pWVar3 = pWVar6->parent;
                            if (pWVar6 == pWVar3->right) {
                                pWVar3->right = pWVar5;
                            }
                            else {
                                pWVar3->left = pWVar5;
                            }
                        }
                        pWVar5->right = pWVar6;
    LAB_00204b04:
                        pWVar6->parent = pWVar5;
                        pWVar6 = tmp;
                    }
                    if (pWVar6 == *ppWVar7) {
                        tmp = *ppWVar7;
                        goto LAB_00204b28;
                    }
                    if (pWVar6->parent->field_0x0 != 0) goto LAB_00204b24;
                    tmp = pWVar6->parent;
                } while( true );
            }
            tmp = *ppWVar7;
        }

    LAB_00204b28:
        tmp->field_0x0 = 1;
        m_someCount++;
        return newEle;
    }

    WidgetBinTree WidgetBinTree::FUN_00204b68(const ui::NamedRef& widget) {
        WidgetBinTree result;

        const char* name = widget->getName();
        Node* last = m_root;
        Node* n = last->parent;
        i32 iVar2 = 1;
        while (n) {
            iVar2 = -(strcmp(name, n->text.get()) >> 0x1f);
            last = n;
            if (!iVar2) n = n->left;
            else n = n->right;
        }

        Node* n1 = last;
        if (iVar2) {
            if (last == m_root->right) {
                result.m_someCount = 1;
                result.m_root = FUN_00204760(false, last, widget);
                return result;
            }

            if (!last->field_0x0 && last->parent->parent == last) {
                n1 = last->left;
            } else {
                n1 = last->right;

                if (!n1) {
                    n1 = last->parent;
                    n = n1;
                    if (last == n1->right) {
                        bool x = true;
                        do {
                            n1 = n->parent;
                            bool x = n == n1->right;
                            n = n1;
                        } while (x);
                    }
                } else if (n1->left) {
                    while (n1->left) n1 = n1->left;
                }
            }
        }

        iVar2 = strcmp(n1->text.get(), name);
        if (iVar2 >= 0) {
            result.m_someCount = 0;
            result.m_root = n1;
            return result;
        }

        result.m_someCount = 1;
        result.m_root = FUN_00204760(false, last, widget);
        return result;
    }
    
    void WidgetBinTree::FUN_00204de8(WidgetBinTree* p1, WidgetBinTree* p2) {
        Node *pWVar1;
        ui2Widget *puVar2;
        bool bVar4;
        Node *pWVar5;
        i32 iVar6;
        undefined4 uVar7;
        Node *pWVar8;
        Node *pWVar9;
        Node **ppWVar10;
        Node *pWVar11;
        Node **ppWVar12;
        Node **ppWVar13;
        Node *pWVar14;
        
        bVar4 = false;
        if (p1->m_root == m_root->right) {
            bVar4 = p2->m_root == m_root;
        }
        if (bVar4) {
            if (m_someCount != 0) {
                destroyNode(m_root->parent);
                m_root->right = m_root;
                m_root->parent = NULL;
                m_root->left = m_root;
                m_someCount = 0;
            }
        }
        else if (p1->m_root != p2->m_root) {
            pWVar8 = p1->m_root;
            do {
                pWVar5 = pWVar8->left;
                if (pWVar5 == NULL) {
                    pWVar5 = pWVar8->parent;
                    if (pWVar8 == pWVar5->left) {
                        p1->m_root = pWVar5;
                        while (pWVar5 = pWVar5->parent, p1->m_root == pWVar5->left) {
                            p1->m_root = pWVar5;
                        }
                    }
                    if (p1->m_root->left != pWVar5) {
                        p1->m_root = pWVar5;
                    }
                }
                else {
                    p1->m_root = pWVar5;
                    if (pWVar5->right != NULL) {
                        pWVar5 = p1->m_root;
                        while (pWVar5 = pWVar5->right, p1->m_root = pWVar5, pWVar5->right != NULL) {
                            pWVar5 = p1->m_root;
                        }
                    }
                }
                pWVar5 = m_root;
                ppWVar13 = &pWVar5->left;
                ppWVar12 = &pWVar5->parent;
                ppWVar10 = &pWVar5->right;
                pWVar5 = pWVar8->right;
                pWVar14 = pWVar8;
                if (pWVar5 == NULL) {
                    pWVar5 = pWVar8->left;
                }
                else {
                    pWVar11 = pWVar8->left;
                    if (pWVar11 != NULL) {
                        if (pWVar11->right == NULL) {
                            pWVar5 = pWVar11->left;
                            pWVar14 = pWVar11;
                        }
                        else {
                            for (pWVar14 = pWVar11->right; pWVar14->right != NULL; pWVar14 = pWVar14->right) {
                            }
                            pWVar5 = pWVar14->left;
                        }
                    }
                }
                if (pWVar14 == pWVar8) {
                    pWVar11 = pWVar14->parent;
                    if (pWVar5 != NULL) {
                        pWVar5->parent = pWVar11;
                    }
                    if (*ppWVar12 == pWVar14) {
                        *ppWVar12 = pWVar5;
                    }
                    else {
                        pWVar9 = pWVar14->parent;
                        if (pWVar9->right == pWVar14) {
                            pWVar9->right = pWVar5;
                        }
                        else {
                            pWVar9->left = pWVar5;
                        }
                    }
                    if (*ppWVar10 == pWVar8) {
                        if (pWVar8->left == NULL) {
                            *ppWVar10 = pWVar8->parent;
                        }
                        else {
                            pWVar9 = pWVar5;
                            if (pWVar5->right != NULL) {
                                for (pWVar9 = pWVar5->right; pWVar9->right != NULL; pWVar9 = pWVar9->right) {
                                }
                            }
                            *ppWVar10 = pWVar9;
                        }
                        pWVar9 = *ppWVar13;
                    }
                    else {
                        pWVar9 = *ppWVar13;
                    }
                    if (pWVar9 == pWVar8) {
                        if (pWVar8->right == NULL) {
                            *ppWVar13 = pWVar8->parent;
                        }
                        else {
                            pWVar8 = pWVar5;
                            if (pWVar5->left != NULL) {
                                for (pWVar8 = pWVar5->left; pWVar8->left != NULL; pWVar8 = pWVar8->left) {
                                }
                            }
                            *ppWVar13 = pWVar8;
                        }
                        goto LAB_002050a8;
                    }
                    iVar6 = pWVar14->field_0x0;
                }
                else {
                    pWVar8->right->parent = pWVar14;
                    pWVar14->right = pWVar8->right;
                    pWVar11 = pWVar14;
                    if (pWVar14 != pWVar8->left) {
                        pWVar11 = pWVar14->parent;
                        if (pWVar5 != NULL) {
                            pWVar5->parent = pWVar11;
                        }
                        pWVar14->parent->right = pWVar5;
                        pWVar14->left = pWVar8->left;
                        pWVar8->left->parent = pWVar14;
                    }
                    if (*ppWVar12 == pWVar8) {
                        *ppWVar12 = pWVar14;
                    }
                    else {
                        pWVar9 = pWVar8->parent;
                        if (pWVar9->right == pWVar8) {
                            pWVar9->right = pWVar14;
                        }
                        else {
                            pWVar9->left = pWVar14;
                        }
                    }
                    uVar7 = pWVar14->field_0x0;
                    pWVar14->parent = pWVar8->parent;
                    pWVar14->field_0x0 = pWVar8->field_0x0;
                    pWVar8->field_0x0 = uVar7;
                    pWVar14 = pWVar8;
    LAB_002050a8:
                    iVar6 = pWVar14->field_0x0;
                }
                if (iVar6 != 0) {
                    pWVar8 = *ppWVar12;
                    while (pWVar9 = pWVar11, pWVar5 != pWVar8) {
                        if (pWVar5 == NULL) {
                            pWVar8 = pWVar9->right;
                        }
                        else {
                            if (pWVar5->field_0x0 != 1) break;
                            pWVar8 = pWVar9->right;
                        }
                        if (pWVar5 == pWVar8) {
                            pWVar8 = pWVar9->left;
                            if (pWVar8->field_0x0 == 0) {
                                pWVar8->field_0x0 = 1;
                                pWVar9->field_0x0 = 0;
                                pWVar8 = pWVar9->left;
                                pWVar9->left = pWVar8->right;
                                if (pWVar8->right != NULL) {
                                    pWVar8->right->parent = pWVar9;
                                }
                                pWVar8->parent = pWVar9->parent;
                                if (pWVar9 == *ppWVar12) {
                                    *ppWVar12 = pWVar8;
                                }
                                else {
                                    pWVar11 = pWVar9->parent;
                                    if (pWVar9 == pWVar11->right) {
                                        pWVar11->right = pWVar8;
                                    }
                                    else {
                                        pWVar11->left = pWVar8;
                                    }
                                }
                                pWVar8->right = pWVar9;
                                pWVar9->parent = pWVar8;
                                pWVar8 = pWVar9->left;
                                pWVar11 = pWVar8->right;
                            }
                            else {
                                pWVar11 = pWVar8->right;
                            }
                            pWVar1 = pWVar8->left;
                            if ((pWVar11 != NULL) && (pWVar11->field_0x0 != 1)) {
    LAB_0020515c:
                                if ((pWVar1 == NULL) || (pWVar1->field_0x0 == 1)) {
                                    if (pWVar11 != NULL) {
                                        pWVar11->field_0x0 = 1;
                                    }
                                    pWVar11 = pWVar8->right;
                                    pWVar8->field_0x0 = 0;
                                    pWVar8->right = pWVar11->left;
                                    if (pWVar11->left != NULL) {
                                        pWVar11->left->parent = pWVar8;
                                    }
                                    pWVar11->parent = pWVar8->parent;
                                    if (pWVar8 == *ppWVar12) {
                                        *ppWVar12 = pWVar11;
                                    }
                                    else {
                                        pWVar1 = pWVar8->parent;
                                        if (pWVar8 == pWVar1->left) {
                                            pWVar1->left = pWVar11;
                                        }
                                        else {
                                            pWVar1->right = pWVar11;
                                        }
                                    }
                                    pWVar11->left = pWVar8;
                                    pWVar8->parent = pWVar11;
                                    pWVar8 = pWVar9->left;
                                    uVar7 = pWVar9->field_0x0;
                                }
                                else {
                                    uVar7 = pWVar9->field_0x0;
                                }
                                pWVar8->field_0x0 = uVar7;
                                pWVar9->field_0x0 = 1;
                                if (pWVar8->left != NULL) {
                                    pWVar8->left->field_0x0 = 1;
                                }
                                pWVar8 = pWVar9->left;
                                pWVar9->left = pWVar8->right;
                                if (pWVar8->right != NULL) {
                                    pWVar8->right->parent = pWVar9;
                                }
                                pWVar8->parent = pWVar9->parent;
                                if (pWVar9 == *ppWVar12) {
                                    *ppWVar12 = pWVar8;
                                }
                                else {
                                    pWVar11 = pWVar9->parent;
                                    if (pWVar9 == pWVar11->right) {
                                        pWVar11->right = pWVar8;
                                    }
                                    else {
                                        pWVar11->left = pWVar8;
                                    }
                                }
                                pWVar8->right = pWVar9;
                                pWVar9->parent = pWVar8;
                                break;
                            }
                            if (pWVar1 == NULL) {
                                pWVar8->field_0x0 = 0;
                            }
                            else {
                                if (pWVar1->field_0x0 != 1) goto LAB_0020515c;
                                pWVar8->field_0x0 = 0;
                            }
                        }
                        else {
                            if (pWVar8->field_0x0 == 0) {
                                pWVar8->field_0x0 = 1;
                                pWVar9->field_0x0 = 0;
                                pWVar8 = pWVar9->right;
                                pWVar9->right = pWVar8->left;
                                if (pWVar8->left != NULL) {
                                    pWVar8->left->parent = pWVar9;
                                }
                                pWVar8->parent = pWVar9->parent;
                                if (pWVar9 == *ppWVar12) {
                                    *ppWVar12 = pWVar8;
                                }
                                else {
                                    pWVar11 = pWVar9->parent;
                                    if (pWVar9 == pWVar11->left) {
                                        pWVar11->left = pWVar8;
                                    }
                                    else {
                                        pWVar11->right = pWVar8;
                                    }
                                }
                                pWVar8->left = pWVar9;
                                pWVar9->parent = pWVar8;
                                pWVar8 = pWVar9->right;
                                pWVar11 = pWVar8->left;
                            }
                            else {
                                pWVar11 = pWVar8->left;
                            }
                            pWVar1 = pWVar8->right;
                            if ((pWVar11 != NULL) && (pWVar11->field_0x0 != 1)) {
    LAB_002052d8:
                                if ((pWVar1 == NULL) || (pWVar1->field_0x0 == 1)) {
                                    if (pWVar11 != NULL) {
                                        pWVar11->field_0x0 = 1;
                                    }
                                    pWVar11 = pWVar8->left;
                                    pWVar8->field_0x0 = 0;
                                    pWVar8->left = pWVar11->right;
                                    if (pWVar11->right != NULL) {
                                        pWVar11->right->parent = pWVar8;
                                    }
                                    pWVar11->parent = pWVar8->parent;
                                    if (pWVar8 == *ppWVar12) {
                                        *ppWVar12 = pWVar11;
                                    }
                                    else {
                                        pWVar1 = pWVar8->parent;
                                        if (pWVar8 == pWVar1->right) {
                                            pWVar1->right = pWVar11;
                                        }
                                        else {
                                            pWVar1->left = pWVar11;
                                        }
                                    }
                                    pWVar11->right = pWVar8;
                                    pWVar8->parent = pWVar11;
                                    pWVar8 = pWVar9->right;
                                    uVar7 = pWVar9->field_0x0;
                                }
                                else {
                                    uVar7 = pWVar9->field_0x0;
                                }
                                pWVar8->field_0x0 = uVar7;
                                pWVar9->field_0x0 = 1;
                                if (pWVar8->right != NULL) {
                                    pWVar8->right->field_0x0 = 1;
                                }
                                pWVar8 = pWVar9->right;
                                pWVar9->right = pWVar8->left;
                                if (pWVar8->left != NULL) {
                                    pWVar8->left->parent = pWVar9;
                                }
                                pWVar8->parent = pWVar9->parent;
                                if (pWVar9 == *ppWVar12) {
                                    *ppWVar12 = pWVar8;
                                }
                                else {
                                    pWVar11 = pWVar9->parent;
                                    if (pWVar9 == pWVar11->left) {
                                        pWVar11->left = pWVar8;
                                    }
                                    else {
                                        pWVar11->right = pWVar8;
                                    }
                                }
                                pWVar8->left = pWVar9;
                                pWVar9->parent = pWVar8;
                                break;
                            }
                            if (pWVar1 == NULL) {
                                pWVar8->field_0x0 = 0;
                            }
                            else {
                                if (pWVar1->field_0x0 != 1) goto LAB_002052d8;
                                pWVar8->field_0x0 = 0;
                            }
                        }
                        pWVar11 = pWVar9->parent;
                        pWVar5 = pWVar9;
                        pWVar8 = *ppWVar12;
                    }
                    if (pWVar5 != NULL) {
                        pWVar5->field_0x0 = 1;
                    }
                }

                delete pWVar14;
                m_someCount = m_someCount + -1;
                pWVar8 = p1->m_root;
            } while (pWVar8 != p2->m_root);
        }
    }
    
    u32 WidgetBinTree::FUN_00205458(const char* name) {
        Node *pWVar1;
        Node *pWVar2;
        WidgetBinTree WStack_80;
        WidgetBinTree WStack_70;
        WidgetBinTree local_60;
        i32 local_40;
        
        WStack_80.findNearestNode0(this, name);
        WStack_70.findNearestNode1(this, name);
        local_40 = 0;
        local_60.m_root = WStack_80.m_root;
        if (WStack_80.m_root != WStack_70.m_root) {
            pWVar2 = (WStack_80.m_root)->left;
            while( true ) {
                if (pWVar2 == NULL) {
                    pWVar2 = (local_60.m_root)->parent;
                    if (local_60.m_root == pWVar2->left) {
                        do {
                            local_60.m_root = pWVar2;
                            pWVar2 = (local_60.m_root)->parent;
                        } while (local_60.m_root == pWVar2->left);
                    }
                    if ((local_60.m_root)->left != pWVar2) {
                        local_60.m_root = pWVar2;
                    }
                }
                else {
                    pWVar1 = pWVar2->right;
                    while (local_60.m_root = pWVar2, pWVar1 != NULL) {
                        pWVar2 = pWVar2->right;
                        pWVar1 = pWVar2->right;
                    }
                }
                local_40 += 1;
                if (local_60.m_root == WStack_70.m_root) break;
                pWVar2 = (local_60.m_root)->left;
            }
        }

        FUN_00204de8(&WStack_80, &WStack_70);

        return local_40;
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
    
    void WidgetBinTree::findNearestNode0(WidgetBinTree* p1, const char* search) { 
        i32 iVar1;
        const char *searchStr;
        Node *cur;
        Node *result;
        
        result = p1->m_root;
        cur = result->parent;
        if (cur != NULL) {
            searchStr = cur->text.get();
            while( true ) {
                iVar1 = strcmp(searchStr, search);
                if (iVar1 < 0) {
                    cur = cur->left;
                }
                else {
                    result = cur;
                    cur = cur->right;
                }
                if (cur == NULL) break;
                searchStr = cur->text.get();
            }
        }
        
        m_root = result;
    }

    void WidgetBinTree::findNearestNode1(WidgetBinTree* p1, const char* search) { 
        i32 iVar1;
        const char *searchStr;
        Node *cur;
        Node *result;
        
        result = p1->m_root;
        cur = result->parent;
        if (cur != NULL) {
            while( true ) {
                iVar1 = strcmp(search,cur->text.get());
                if (iVar1 < 0) {
                    result = cur;
                    cur = cur->right;
                }
                else {
                    cur = cur->left;
                }
                if (cur == NULL) break;
            }
        }
        m_root = result;
    }

    WidgetBinTree::Node* WidgetBinTree::getRoot() const {
        return m_root;
    }
};