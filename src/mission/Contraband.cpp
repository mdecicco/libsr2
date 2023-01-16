#include <libsr2/mission/Contraband.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>
#include <libsr2/sim/dgBomb.h>

namespace sr2 {
    ContrabandInfo::ContrabandInfo() {
        unk7 = 1;
        name[0] = 0;
        unk2[0] = 0;
        geom_file_0[0] = 0;
        geom_file_1[0] = 0;
        unk4 = 1;
        cash_value = 1;
        unk5 = 0;
        time_to_detonate = 0;
        is_bomb = 0;
    }

    ContrabandInfo::~ContrabandInfo() {
    }

    void ContrabandInfo::SomeGeomFunc() {
    }

    bool ContrabandInfo::load(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();
        u16 tag;
        u32 size;
        if (!tsv->readTag(&tag, &size)) return false;
        if (((tag - 12) >= 2) && tag != 8) return false;

        if (!fp->read(&name, 32)) return false;
        if (!fp->read(&unk2, 32)) return false;
        if (!fp->read(geom_file_0, 32)) return false;
        if (tag == 13 && !fp->read(geom_file_1, 32)) return false;
        
        if (!fp->read(&unk4, 4)) return false;
        if (!fp->read(&cash_value, 4)) return false;
        if (!fp->read(&unk5, 4)) return false;
        if (!fp->read(&time_to_detonate, 2)) return false;
        if (!fp->read(&unk7, 2)) return false;
        if ((tag - 12) <= 1 && !fp->read(&is_bomb, 1)) return false;

        SomeGeomFunc();
        if (dgBomb::g_SomeBomb) return true;
        if (!is_bomb) return true;

        dgBomb::g_SomeBomb = new dgBomb();
        dgBomb::g_SomeBomb->enablePhysics(true);
        dgBomb::g_SomeBomb->setExplosionType(EXPLOSION_TYPE::unk);
        dgBomb::g_SomeBomb->setStrengthAlt();
        // dgBombManager::get()->attach(dgBomb::g_SomeBomb);
        // dgBombManager::get()->FUN_002237a8(1);

        return true;
    }
};