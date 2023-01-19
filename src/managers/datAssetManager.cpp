#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/stream.h>
#include <string.h>

namespace sr2 {
    char datAssetManager::m_path[256] = { 0 };

    Stream* datAssetManager::open(const char* dir, const char* filename, const char* ext, i32 unk0, bool unk1) {
        char path[128];
        full_path(path, 128, dir, filename, ext);
        return Stream::open(path, unk1);
    }

    bool datAssetManager::exists(const char* dir, const char* filename, const char* ext) {
        Stream* s = open(dir, filename, ext, 1, true);
        if (s) s->close();
        return s != nullptr;
    }

    void datAssetManager::full_path(char* buf, u32 bufSz, const char* dir, const char* filename, const char* ext) { 
        int iVar1;
        const char *pcVar2 = nullptr;

        if (((*filename == '\\') || (*filename == '/')) || (filename[1] == ':')) {
            *buf = '\0';
        }
        else {
            strcpy(buf, m_path);

            if (dir && (*dir != '\0')) {
                iVar1 = strlen(dir);
                iVar1 = strncmp(filename, dir, iVar1);
                if (iVar1 == 0) {
                    iVar1 = strlen(dir);
                    pcVar2 = strchr("/\\", filename[iVar1]);
                }

                if (!pcVar2) {
                    strcat(buf, dir);
                    strcat(buf, "/");
                }
            }
        }

        pcVar2 = strcat(buf, filename);
        if (ext && *ext) {
            pcVar2 = strrchr(filename, '.');
            if (pcVar2) {
                pcVar2 = strrchr(filename, '.');
                iVar1 = strcmp(pcVar2 + 1, ext);
                if (iVar1 == 0) {
                    return;
                }
            }
            strncat(buf, ".", bufSz);
            pcVar2 = strcat(buf, ext);
        }
    }

    char* datAssetManager::get_path() {
        return m_path;
    }

    char* datAssetManager::set_path(const char* path) {
        char cVar1;
        char *pcVar2;
        int iVar3;
        char *pcVar4;

        pcVar4 = m_path;
        pcVar2 = strcpy(m_path, path);
        if (*path != '\0') {
            iVar3 = strlen(m_path);
            cVar1 = m_path[0];
            while (cVar1 != '\0') {
                cVar1 = *pcVar4;
                if (cVar1 == '\\') {
                    *pcVar4 = '/';
                }
                else {
                    if (cVar1 == '?') {
                        *pcVar4 = '/';
                    }
                    else {
                        if (cVar1 == '*') {
                            *pcVar4 = ':';
                        }
                    }
                }
                pcVar4 = pcVar4 + 1;
                cVar1 = *pcVar4;
            }

            /*
            pcVar2 = (char *)0xffffffff;
            if ((iVar3 != 0) && (pcVar2 = &IO::psxMemoryCard::Entry + iVar3, *pcVar2 != '/')) {
                pcVar2 = &Path + iVar3;
                *pcVar2 = '/';
                (&DAT_00907fc1)[iVar3] = 0;
            }
            */
        }
        return pcVar2;
    }
};