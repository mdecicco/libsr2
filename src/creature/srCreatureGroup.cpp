#include <libsr2/creature/srCreatureGroup.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/Stream.h>

#include <string.h>

namespace sr2 {
    srCreatureGroup::srCreatureGroup() {
        name[0] = 0;
        field1_0x20 = 0;
        pointCount = 0;
        points = nullptr;
        currentPointIdx = 0;
        pathInterpolationFactor = 0.0f;
        unk0 = 0;
        spawnRadius = 0.0f;
        currentPosition = { 0.0f, 0.0f, 0.0f };
        type = nullptr;
    }

    srCreatureGroup::~srCreatureGroup() {
        // memory leak on points???
    }

    void srCreatureGroup::reset() {
        field1_0x20 = 0;
        currentPointIdx = 0;
        pathInterpolationFactor = 0.0f;
        currentPosition = points[0];
    }

    void srCreatureGroup::traversePath() {
        if (pathInterpolationFactor > 1.0f) {
            pathInterpolationFactor = 0.0f;
            if (pointCount == 2) currentPointIdx = 1;
            else {
                currentPointIdx++;
                currentPointIdx = currentPointIdx % pointCount;
            }
        }

        if (pointCount == 0 || pointCount != 2) {
            vec3f& a = points[currentPointIdx];
            vec3f& b = points[(currentPointIdx + 1) % pointCount];
            currentPosition = a + (b - a) * pathInterpolationFactor;
            pathInterpolationFactor += 0.005f;
        }
    }
    
    bool srCreatureGroup::load(TaggedStream& ts) {
        Stream* fp = ts.getStream();
        u16 tag;
        u32 size;

        while (ts.readTag(&tag, &size)) {
            switch (tag) {
                case 0x891: {
                    if (!fp->read(&unk0, 4)) return false;
                    if (!fp->read(&spawnRadius, 4)) return false;
                    if (!fp->read(&unk2, 4)) return false;
                    break;
                }
                case 0x890: {
                    if (!fp->read(&pointCount, 4)) return false;
                    initPoints(pointCount);

                    for (u32 i = 0;i < pointCount;i++) {
                        if (!fp->read(&points[i], 12)) return false;
                    }

                    currentPosition = points[0];
                    break;
                }
                case 0x892: {
                    if (!fp->read(name, 32)) return false;
                    char* p = strstr(name, "_set");
                    if (p) *p = 0;
                }
                case 0x899: return true;
            }
        }

        return false;
    }

    void srCreatureGroup::initPoints(u32 _pointCount) {
        pointCount = _pointCount;
        points = new vec3f[pointCount];
    }
};