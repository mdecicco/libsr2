#include <libsr2/utilities/parser.h>
#include <stdio.h>
#include <memory.h>

namespace sr2 {
    Parser::Parser() {
        m_targets = nullptr;
        m_count = 0;
    }

    Parser::~Parser() {
        if (m_targets) {
            node* n = m_targets;
            while (n) {
                m_targets = n->next;
                delete n;
                n = m_targets;
            }

            m_targets = nullptr;
            m_count = 0;
        }
    }

    Parser::node* Parser::add(PARSE_TYPE tp, const char* name, void* dest, i32 unk0, i32 unk1) {
        node* target = new node();
        if (!m_targets) m_targets = target;
        else {
            node* n = m_targets;
            while (n->next) {
                n = n->next;
            }
            n->next = target;
        }

        target->tp = tp;
        snprintf(target->name, 64, name);
        target->destination = dest;
        target->unk0 = unk0;
        target->unk1 = unk1;
        target->next = nullptr;
        return target;
    }

    void Parser::fulfill(node* n, void* src, size_t sz) {
        memcpy(n->destination, src, sz);
    }
};