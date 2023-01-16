#include <libsr2/utilities/argParser.h>
#include <malloc.h>
#include <string.h>

namespace sr2 {
    i32 datArgParser::argc = 0;
    char** datArgParser::argv = nullptr;
    HashTable datArgParser::argTable = HashTable();

    datArgParser::datArgParser() {
    }

    datArgParser::~datArgParser() {
    }

    struct HashElem {
        u32 count;
        char** param;
    };

    void datArgParser::Init(int _argc, char** _argv) {
        argc = _argc;
        argv = _argv;

        if ((argv != NULL) && (0 < argc)) {
            for (u32 i = 0;i < argc;i++) {
                char* arg = argv[i];
                if (arg[0] != '-') continue;

                if (argTable.Access(arg + 1)) continue;
                char* val = strchr(arg, '=');
                HashElem* elem = new HashElem({ 0, nullptr });

                u32 u = val == nullptr ? 0 : 1;
                u32 a = i;
                while (true) {
                    a++;
                    elem->count = u;
                    if (argc <= a) break;
                    if (argv[a][0] == '-' && 9 < (u8)argv[a][1] - '0') break;
                    u++;
                }

                if (val != nullptr) {
                    val[0] = 0;
                    argTable.Insert(argv[i] + 1, elem);
                    argv[i] = val + 1;
                } else {
                    argTable.Insert(argv[i] + 1, elem);
                }

                if (elem->count != 0) {
                    elem->param = new char*[elem->count];
                    u8 off = i;
                    if (!val) off++;
                    for (u32 a = 0;a < elem->count;a++) elem->param[a] = argv[off + a];
                }
            }
        }

        argTable.MakePermanent();
    }

    void datArgParser::Kill() {
        // todo
    }

    void datArgParser::InitFromMem(char* args) {
        char cVar1;
        char cVar2;
        bool bVar3;

        bVar3 = false;
        if (args != NULL) {
            cVar2 = *args;
            cVar1 = *args;
            while (cVar1 != '\0') {
                if (bVar3) {
                    if (cVar2 == ' ') {
                        *args = '\0';
                    }
                    else {
                        if (cVar2 == '\t') {
                            *args = '\0';
                        }
                        else {
                            if ((cVar2 != '\n') && ('\x1f' < cVar2)) {
                                args = args + 1;
                                cVar2 = *args;
                                cVar1 = *args;
                                if (bVar3) {
                                    argc += 1;
                                }
                                *args = '\0';
                                Init(argc, argv);
                                return;
                            }
                            *args = '\0';
                        }
                    }
                    bVar3 = false;
                    argc += 1;
                }
                else {
                    cVar1 = *args;
                    if (cVar1 == ' ') {
                        *args = '\0';
                    }
                    else {
                        if (cVar1 == '\t') {
                            *args = '\0';
                        }
                        else {
                            if (cVar1 == '\n') {
                                *args = '\0';
                            }
                            else {
                                if (cVar1 < ' ') {
                                    *args = '\0';
                                }
                                else {
                                    bVar3 = true;
                                    argv[argc] = args;
                                }
                            }
                        }
                    }
                }

                args = args + 1;
                cVar2 = *args;
                cVar1 = *args;
            }
            if (bVar3) {
                argc += 1;
            }
            *args = '\0';
            Init(argc, argv);
        }
    }

    bool datArgParser::GetBooleanArgument(const char* arg) {
        return argTable.Access(arg) != nullptr;
    }

    bool datArgParser::GetIntegerArgument(const char* arg, i32 index, i32* dest) {
        HashElem* elem = (HashElem*)argTable.Access(arg);
        if (!elem) return false;

        if (index < elem->count) {
            *dest = atoi(elem->param[index]);
            return true;
        }
        
        return false;
    }

    bool datArgParser::GetFloatArgument(const char* arg, i32 index, f32* dest) {
        HashElem* elem = (HashElem*)argTable.Access(arg);
        if (!elem) return false;

        if (index < elem->count) {
            *dest = atof(elem->param[index]);
            return true;
        }

        return false;
    }

    bool datArgParser::GetStringArgument(const char* arg, i32 index, char** dest) {
        HashElem* elem = (HashElem*)argTable.Access(arg);
        if (!elem) return false;

        if (index < elem->count) {
            *dest = elem->param[index];
            return true;
        }

        return false;
    }

    void datArgParser::AddReplace(const char* arg, char* value) {
        HashElem* ptr = (HashElem*)argTable.Access(arg);
        if (ptr) {
            if (ptr->param) delete [] ptr->param;
            delete ptr;
            argTable.Delete(arg);
        }

        ptr = new HashElem({ 1, nullptr });
        ptr->param = new char*[1];
        ptr->param[0] = value;
        argTable.Insert(arg, ptr);
    }

    void datArgParser::Remove(const char* arg) {
        HashElem* ptr = (HashElem*)argTable.Access(arg);
        if (ptr) {
            if (ptr->param) delete [] ptr->param;
            delete ptr;
            argTable.Delete(arg);
        }
    }
};