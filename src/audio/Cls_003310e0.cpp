#include <libsr2/audio/Cls_003310e0.h>
#include <libsr2/managers/audManager.h>
#include <libsr2/audio/audSound.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/stream.h>

#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cctype>

namespace sr2 {
    Cls_003310e0::Cls_003310e0(undefined4 p1, undefined4 p2) {
        field_0x8c = p1;
        field_0x90 = p2;

        field_0x0 = 0;
        m_unk1Count = 0;
        m_unk0Count = 0;
        field_0xc[0] = 0;
    }

    Cls_003310e0::~Cls_003310e0() {
    }

    i32 Cls_003310e0::parseBanks(u32 p1, const char* path) {
        char buf[256] = { 0 };
        strncpy(buf, audManager::get()->getSomePath(), 256);
        strcat(buf, path);

        Stream* fp = datAssetManager::open("", buf, "csv", 0, true);
        if (!fp) return -1;

        char line[256] = { 0 };
        if (fgets(line, 256, fp->getRaw()) == 0) {
            delete fp;
            return 0;
        }

        PathTokenMOB token = PathTokenMOB::None;
        i32 iVar8 = 0;
        i32 bankCount = 0;
        bool bVar1 = false;
        bool bVar2 = true;
        bool readNextLine = true;

        while (iVar8 < p1) {
            if (readNextLine) {
                if (fgets(line, 256, fp->getRaw()) == 0) break;
                if (line[0] == '#' || line[1] == '#') continue;
                processLine(line);
            }

            if (token == PathTokenMOB::None) {
                token = (PathTokenMOB)getPathTokenIndex(PathTokenType::ModelObjectBank, line);
                readNextLine = true;
            } else {
                iVar8++;
                token = PathTokenMOB::None;
                m_bankFilenames[bankCount];

                do {
                    strncpy(m_bankFilenames[bankCount], line, 32);
                    bankCount++;

                    if (fgets(line, 256, fp->getRaw()) == 0) {
                        delete fp;
                        return bankCount;
                    }

                    processLine(line);

                    token = (PathTokenMOB)getPathTokenIndex(PathTokenType::ModelObjectBank, line);
                } while (token == PathTokenMOB::None);

                readNextLine = false;
            }
        }

        return bankCount;
    }

    u32 Cls_003310e0::loadBank(const char* path) {
        audManager* gat = audManager::get();
        
        char csvPath[512] = { 0 };
        strncpy(csvPath, gat->getSomePath(), 512);
        strcat(csvPath, path);

        Stream* fp = datAssetManager::open("", csvPath, "csv", 0, true);
        if (!fp) return -1;

        char line[256] = { 0 };

        u32 result = 0;

        if (fgets(line, 256, fp->getRaw()) == 0) {
            delete fp;
            return 0;
        }

        bool loadWin = true;
        bool loadPsx = true;
        bool readNextLine = true;

        auto parseRows = [&](i32 duplicateCount, bool singleRow) {
            static SampleRateCode srcMap[] = {
                (SampleRateCode)1,
                (SampleRateCode)1,
                (SampleRateCode)1,
                (SampleRateCode)1,
                (SampleRateCode)1,
                (SampleRateCode)1,
                SampleRateCode::sr22050,
                SampleRateCode::sr44100,
                SampleRateCode::srDual44100
            };

            if (fgets(line, 256, fp->getRaw()) == 0) {
                delete fp;
                return false;
            }
            
            processLine(line);
            char sampleName[256];

            do {
                char* sampleFilename = line;
                strcpy(sampleName, sampleFilename);

                u32 fnLen = strlen(sampleFilename);
                u32 loop = 0, flags = 0, bufType = 0;
                f32 volume = 0.0f, radius = 0.0f, minDist = 0.0f, maxDist = 0.0f;
                bool didRead = sscanf(line + fnLen + 1, "%d %f %f %d %d %f %f", &loop, &volume, &radius, &flags, &bufType, &minDist, &maxDist) != 0;

                if (didRead) {
                    if (volume == 0.0f) volume = 1.0f;
                    if (radius == 0.0f) radius = 100.0f;
                }
                
                i32 duplicateIdx = 0;
                do {
                    if (duplicateCount > 0) {
                        if (duplicateIdx == 0) strcpy(sampleName, sampleFilename);

                        duplicateIdx += 1;
                        char index[16] = { 0 };
                        sprintf(index, "%02d", duplicateIdx);

                        char indexedName[512] = { 0 };
                        strcpy(indexedName, sampleName);
                        strcat(indexedName, index);
                        strcpy(sampleFilename, indexedName);
                    }

                    if (didRead && loop && bufType < 6) loop = 0;
                    if (!loadPsx) return true;

                    gat->setNextSampleRateCode(srcMap[bufType]);
                    if (gat->loadSoundAtIndex(sampleFilename, gat->getFreeIndex())) {
                        m_unk1Count++;

                        if (loop) gat->m_lastLoaded->setFlags(loop & 1);
                        if (volume != 0.0f) gat->m_lastLoaded->setVolumeOnly(volume);;
                        if (radius != 0.0f) gat->m_lastLoaded->setRadius(radius);

                        if (bufType == 1 || bufType == 3 || bufType == 4) {
                            if (minDist == 0.0f) minDist = 1.0f;
                            if (maxDist == 0.0f) maxDist = 99.0f;
                            if (volume == 0.0f) volume = 1.0f;
                            // if (minDist != 0.0f) FUN_00329ba8();
                            // if (maxDist != 0.0f) FUN_00329b98();
                        }

                        result++;
                        field_0x0 = gat->m_lastLoaded->getIndex();
                        m_unk1[m_unk1Count].sfxIndex = field_0x0;
                        m_unk1[m_unk1Count].flags = loop;
                    }
                } while (duplicateIdx < duplicateCount);

                if (singleRow) break;

                if (fgets(line, 256, fp->getRaw()) == 0) {
                    delete fp;
                    return false;
                }
                processLine(line);

                if ((PathTokenMOB)getPathTokenIndex(PathTokenType::ModelObjectBank, line) != PathTokenMOB::None) {
                    readNextLine = false;
                    break;
                }
                if ((PathTokenSSV)getPathTokenIndex(PathTokenType::SampleSamplesVariants, line) != PathTokenSSV::None) {
                    readNextLine = false;
                    break;
                }
                if ((PathTokenMSE)getPathTokenIndex(PathTokenType::MusicSpeechEngine, line) != PathTokenMSE::None) {
                    readNextLine = false;
                    break;
                }
            } while (true);

            return true;
        };

        while (true) {
            if (readNextLine) {
                if (fgets(line, 256, fp->getRaw()) == 0) {
                    delete fp;
                    return result;
                }

                if (line[0] == '#' || line[1] == '#') continue;
                processLine(line);
            } else readNextLine = true;

            PathTokenMOB tokenMOB = (PathTokenMOB)getPathTokenIndex(PathTokenType::ModelObjectBank, line);
            switch (tokenMOB) {
                case PathTokenMOB::None: break;
                case PathTokenMOB::Model: {
                    u32 len = strlen(line);
                    char buf[256] = { 0 };
                    strcpy(buf, line + len + 1);
                    processLine(buf);
                    if (sscanf(buf, "%s", field_0xc) == -1) continue;

                    field_0x0 = 0;
                    m_unk0[m_unk0Count].field_0x0 = m_unk1Count + 1;
                    strcpy(m_unk0[m_unk0Count].field_0x4, field_0xc);
                    m_unk0Count++;
                    continue;
                }
                case PathTokenMOB::Object: {
                    u32 len = strlen(line);
                    char buf[256] = { 0 };
                    strcpy(buf, line + len + 1);
                    processLine(buf);
                    if (sscanf(buf, "%s", field_0xc) == -1) continue;

                    field_0x0 = 0;
                    continue;
                }
                case PathTokenMOB::PathPsx: {
                    u32 len = strlen(line);
                    char buf[256] = { 0 };
                    strcpy(buf, line + len + 1);
                    processLine(buf);
                    gat->setPathBase(buf);
                    continue;
                }
                case PathTokenMOB::LoadWin: {
                    loadWin = true;
                    loadPsx = false;
                    continue;
                }
                case PathTokenMOB::LoadPsx: {
                    loadWin = false;
                    loadPsx = true;
                    continue;
                }
                case PathTokenMOB::LoadAll: {
                    loadWin = true;
                    loadPsx = true;
                    continue;
                }
                default: continue;
            }

            PathTokenSSV tokenSSV = (PathTokenSSV)getPathTokenIndex(PathTokenType::SampleSamplesVariants, line);
            switch (tokenSSV) {
                case PathTokenSSV::None: break;
                case PathTokenSSV::Sample: {
                    if (!parseRows(-1, true)) return result;
                    continue;
                }
                case PathTokenSSV::Samples: {
                    if (!parseRows(-1, false)) return result;
                    continue;
                }
                case PathTokenSSV::Variants: {
                    u32 len = strlen(line);
                    char buf[256] = { 0 };
                    strcpy(buf, line + len + 1);
                    processLine(buf);

                    i32 variantCount = atoi(buf);
                    if (!parseRows(variantCount, false)) return result;
                    continue;
                }
                default: continue;
            }

            if (!loadPsx) continue;

            PathTokenMSE tokenMSE = (PathTokenMSE)getPathTokenIndex(PathTokenType::MusicSpeechEngine, line);
            switch (tokenMSE) {
                case PathTokenMSE::None: break;
                case PathTokenMSE::Speech: {
                    i32 variantCount, b, c;
                    sscanf(line, " %d,%d,%d", &variantCount, &b, &c);

                    // is this wrong?
                    m_unk0[m_unk0Count].field_0x44 = variantCount;
                    m_unk0[m_unk0Count].field_0x48 = variantCount;
                    m_unk0[m_unk0Count].field_0x4c = variantCount;
                    
                    m_unk0Count++;

                    if (!parseRows(variantCount, false)) return result;
                    continue;
                }
                case PathTokenMSE::Engine: {
                    f32 a, b, c, d, e;
                    if (sscanf(line,"%f %f %f %f %f", &a, &b, &c, &d, &e) != 0) {
                        m_unk0[m_unk0Count].field_0x44 = a;
                        m_unk0[m_unk0Count].field_0x48 = b;
                        m_unk0[m_unk0Count].field_0x4c = c;
                        m_unk0[m_unk0Count].field_0x50 = d;
                        m_unk0[m_unk0Count].field_0x54 = e;
                    }

                    m_unk0Count++;
                    continue;
                }
                default: continue;
            }
        }

        delete fp;
        return result;
    }

    void Cls_003310e0::processLine(char* line) {
        // The reimplementation of this function might not match the original exactly...
        // I cleaned it up a lot, but also it really seems like this while loop (and the
        // original version of it) would never execute any iterations? See 0x00331210...

        u32 len = strlen(line);
        bool foundFirstSpace = false;
        for (u32 i = 0;i < len;i++) {
            line[i] = toupper(line[i]);
            if (line[i] == ',' || line[i] < 0x15) line[i] = ' ';
            if (line[i] == ' ' && !foundFirstSpace) {
                line[i] = 0;
                foundFirstSpace = true;
            }
        }
    }

    i32 Cls_003310e0::getPathTokenIndex(PathTokenType ptt, const char* line) {
        static const char* PathTokensMOB[] = {
            "MODEL",
            "OBJECT",
            "BANK",
            "PATHWIN",
            "PATHPSX",
            "LOADWIN",
            "LOADPSX",
            "LOADALL",
            "@"
        };
        static const char* PathTokensSSV[] = {
            "SAMPLE",
            "SAMPLES",
            "VARIANTS",
            "@"
        };
        static const char* PathTokensMSE[] = {
            "MUSIC",
            "SPEECH",
            "ENGINE",
            "@"
        };

        const char** tokens = PathTokensMSE;
        if (ptt == PathTokenType::ModelObjectBank) tokens = PathTokensMOB;
        else if (ptt == PathTokenType::SampleSamplesVariants) tokens = PathTokensSSV;

        const char** cToken = tokens;
        i32 index = 0;
        while (*cToken[0] != '@') {
            if (strcmp(*cToken, line) == 0) return index + 1;

            cToken++;
            index++;
        }

        return 0;
    }
};