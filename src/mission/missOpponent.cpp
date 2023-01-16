#include <libsr2/mission/missOpponent.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>

namespace sr2 {
    missPlayer::missPlayer() {
        team_idx = 0;
        geom_idx = 10;
        vehicle_tp = vpsupbug;
        agent_idx = 0;
        unk3 = 0;

        // This is not in the actual game
        is_human = false;
    }

    bool missPlayer::read(TaggedStream* tsv, u16 unk) {
        Stream* fp = tsv->getStream();
        if (!fp->read(&team_idx, 4)) return false;
        if (!fp->read(&vehicle_tp, 4)) return false;
        if (!fp->read(&geom_idx, 4)) return false;
        if (unk != 4) return true;

        if (!fp->read(&agent_idx, 2)) return false;
        if (!fp->read(&unk3, 2)) return false;

        return true;
    }
};