#include <libsr2/io/datParser.h>
#include <libsr2/io/stream.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/datMultiTokenizer.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

namespace sr2 {
    datParser::datParser(const char* ext) {
        snprintf(m_last_token, 64, "%s", ext);
        m_targets = nullptr;
        m_count = 0;
        field_0x50 = 0;
    }

    datParser::~datParser() {
        if (m_targets) {
            datParserNode* n = m_targets;
            while (n) {
                m_targets = n->next;
                delete n;
                n = m_targets;
            }

            m_targets = nullptr;
            m_count = 0;
        }
    }

    datParser* datParser::add_parser(const char* nodeName, datCallback* cb) {
        datParser* newOne = new datParser(nodeName);
        newOne->field_0x50 = field_0x50 + 2;
        add(PARSE_TYPE::STRUCTURE, nodeName, (void*)newOne, 1, cb);
        return newOne;
    }

    datParserNode* datParser::add(PARSE_TYPE tp, const char* name, void* dest, u16 element_count, datCallback* cb) {
        datParserNode* target = new datParserNode();
        if (!m_targets) m_targets = target;
        else {
            datParserNode* n = m_targets;
            while (n->next) {
                n = n->next;
            }
            n->next = target;
        }

        target->tp = tp;
        snprintf(target->name, 64, name);
        target->destination = dest;
        target->element_count = element_count;
        target->callback = cb;
        target->next = nullptr;
        return target;
    }

    i32 datParser::read_inner(datBaseTokenizer* tok) {
        char buf[64] = { 0 };

        while (true) {
            // begin structure

            // iterate over every field in the structure
            while (true) {
                // find the opening block or return if it doesn't exist, I think
                i32 a = 0;
                do {
                    a = tok->getToken(buf, 64);
                    if (buf[0] == 0) return 0;
                    if (strcmp(buf, "}") == 0) return 0;
                    a = strcmp(buf, "{");
                } while (a == 0);

                // buf now contains a field name

                datParserNode* n = m_targets;
                bool break_outer = false;
                // find node that matches the field name, otherwise continue to the next structure?
                while (n) {
                    if (!n) {
                        break_outer = true;
                        break;
                    }

                    if (strcmp(n->name, buf) == 0) break;
                    n = n->next;
                }

                if (!n || break_outer) break;

                // parse value to node destination
                i32 unk1 = n->element_count;
                i32 unk8 = unk1;
                i32 unk9 = unk1;
                void* cb = nullptr;
                u8* dest = (u8*)n->destination;
                switch (n->tp) {
                    case STRING: {
                        i32 u2 = unk1 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                tok->getToken((char*)dest, n->string_len);
                                dest += n->string_len;
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case BOOLEAN: {
                        i32 u2 = unk8 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                i32 i = tok->readInt32();
                                *(bool*)dest = i != 0;
                                dest += sizeof(bool);
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case INT8: {
                        i32 u2 = unk8 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                i32 i = tok->readInt32();
                                *dest = u8(i);
                                dest++;
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case INT16: {
                        i32 u2 = unk8 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                i32 i = tok->readInt32();
                                *(u16*)dest = u16(i);
                                dest += 2;
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case INT32: {
                        i32 u2 = unk8 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                *(u32*)dest = tok->readInt32();
                                dest += 4;
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case FLOAT: {
                        i32 u2 = unk9 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                *(f32*)dest = tok->readFloat();
                                dest += 4;
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case VEC2F: {
                        i32 u2 = unk9 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                *(vec2f*)dest = tok->readVec2();
                                dest += sizeof(vec2f);
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case VEC3F: {
                        i32 u2 = unk9 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                *(vec3f*)dest = tok->readVec3();
                                dest += sizeof(vec3f);
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case VEC4F: {
                        i32 u2 = unk9 - 1;
                        if (u2 != -1) {
                            do {
                                u2--;
                                *(vec4f*)dest = tok->readVec4();
                                dest += sizeof(vec4f);
                            } while (u2 != -1);
                        }
                        break;
                    }
                    case STRUCTURE: {
                        ((datParser*)n->destination)->read_inner(tok);
                        break;
                    }
                    default: {
                        break;
                    }
                }

                if (n->callback) {
                    // Utils::datCallback::Call(n->callback, nullptr);
                }
            }

            // find matching closing bracket for this structure
            tok->getToken(buf, 64);
            if (strcmp(buf, "{") == 0) {
                u32 i = 1;
                do {
                    tok->getToken(buf, 64);
                    if (strcmp(buf, "{") == 0) i++;
                    else {
                        if (strcmp(buf, "}") == 0) i--;
                    }
                } while (i != 0);
            } else {
                tok->skipToEndOfLine();
            }
        }
    }

    void datParser::read(datBaseTokenizer* tokenizer) {
        tokenizer->getToken(m_last_token, 64);
        read_inner(tokenizer);
    }

    void datParser::load(Stream* file, const char* filename) {
        datMultiTokenizer mt;
        read(mt.GetReadTokenizer(filename, file, "type: a", "type: b"));
    }

    bool datParser::load(const char* dir, const char* filename, const char* ext){
        Stream* f = datAssetManager::open(dir, filename, ext, 0, true);
        if (f) {
            load(f, filename);
            f->close();
        }
        return f != nullptr;
    }

    bool datParser::save(const char* dir, const char* filename, const char* ext){
        return false;
    }

    void datParser::fulfill(datParserNode* n, void* src, size_t sz) {
        memcpy(n->destination, src, sz);
    }
};