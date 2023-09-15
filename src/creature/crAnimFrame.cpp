#include <libsr2/creature/crAnimFrame.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/creature/crSkeletonData.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/creature/crBoneData.h>
#include <libsr2/io/stream.h>
#include <libsr2/math/math.h>
#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    crAnimFrame::crAnimFrame() {
        channelCount = 0;
        buffer = nullptr;
    }

    crAnimFrame::~crAnimFrame() {
        if (buffer) {
            delete [] buffer;
            buffer = nullptr;
        }
    }

    void crAnimFrame::init(u32 _channelCount) {
        if (channelCount == _channelCount) return;

        channelCount = _channelCount;
        if (buffer) delete [] buffer;

        u32 count = math::max(channelCount, 16u);
        buffer = new f32[count];
    }

    bool crAnimFrame::load(Stream* fp, u32 _channelCount) {
        init(_channelCount);
        return fp->read(buffer, channelCount * 4);
    }

    void crAnimFrame::addScaled(crAnimFrame* frame, f32 u0, i32 u1, i32 u2) {
        u32 begin = u2 == -1 ? 0 : (u1 + 1) * 3;
        u32 end = u2 == -1 ? channelCount : (u2 + 2) * 3;

        for (u32 i = begin;i < end;i++) {
            buffer[i] += u0 * frame->buffer[i];
        }
    }

    void crAnimFrame::pose(crSkeleton* skel, bool unk) {
        vec3f* eulers = (vec3f*)(&buffer[3]);
        for (u32 i = 0;i < skel->data->boneCount;i++) {
            skel->bones[i].eulers = eulers[i];
            math::from_eulers_xzy(skel->bones[i].transform, skel->bones[i].eulers);
            skel->bones[i].transform.w = skel->data->boneData[i].offset;
        }

        if (!unk) return;
        skel->bones[0].transform.w = { buffer[0], buffer[1], buffer[2] };
    }
};