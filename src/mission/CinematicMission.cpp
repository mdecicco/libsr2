#include <libsr2/mission/CinematicMission.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>

namespace sr2 {
    bool CinematicMission::load(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();
        u16 tag;
        u32 size;
        u32 n = 0;

        do {
            while( true ) {
                if (!tsv->readTag(&tag, &size)) return false;
                if (tag == 0x804) break;
                if (tag < 0x805) {
                    if (tag == 0x802) {
                        if (size != 32) return false;
                        if (!fp->read(&b_unk6, 32)) return false;
                    }
                    else if (tag < 0x803) {
                        if (tag == 0x801) {
                            fp->read(&vehicle_positions[n++], 16);
                            if (n > 16) return false;
                        }
                    }
                    else fp->read(&unk0, 4);
                }
                else if (tag == 0x806) {
                    u32 sz = size / 8 << 2;
                    if (size / 8 > 16) return false;
                    fp->read(&b_unk3, sz);
                    fp->read(&b_unk4, sz);
                }
                else if (tag < 0x806) {
                    if (!fp->read(&b_unk5, 4)) return false;
                }
                else if (tag == 0x8ff) {
                    method_0x50();
                    return true;
                }
            }

            char fn[128];
            fn[0] = 0;
            if (!fp->read(fn, size)) return false;
            fn[size] = '\0';
            //printf("%s\n", fn);

            // lVar3 = MatchInt__17datAsciiTokenizerPCc((parFileIO *)&this->field_0x1c0,filename);
        } while (true);
    }
    void CinematicMission::method_0x40() { }
    void CinematicMission::method_0x50() { }
};