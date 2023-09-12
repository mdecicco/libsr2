#include <libsr2/creature/crSkeleton.h>
#include <libsr2/creature/crSkeletonData.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/creature/crBoneData.h>
#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    crSkeleton::crSkeleton(u32 boneCount) {
        data = nullptr;
        transform = nullptr;
        bones = new crBone[boneCount];
        boneTransforms = new mat3x4f[boneCount];

        for (u32 i = 0;i < boneCount;i++) {
            bones[i].transformPtr = &boneTransforms[i];
        }
    }

    crSkeleton::~crSkeleton() {
        if (boneTransforms) delete [] boneTransforms;
        // memory leak on crSkeleton::bones???
    }

    void crSkeleton::init(crSkeletonData* _data, mat3x4f* _transform) {
        data = _data;
        transform = nullptr;

        for (u32 i = 0;i < data->boneCount;i++) {
            math::identity_3x3(bones[i].transform);
            data->boneData[i].offset = bones[i].transform.w;
        }

        update();

        transform = _transform;
        for (u32 i = 0;i < data->boneCount;i++) {
            data->boneData[i].unkPos = bones[i].transformPtr->w;
        }
    }

    void crSkeleton::update() {
        data->boneData[0].transform(transform, bones);
    }
    
    void crSkeleton::attach(mat4x4f* transforms) {
        for (u32 i = 0;i < data->boneCount;i++) {
            transforms[i] = {
                {
                    bones[i].transform.x.x,
                    bones[i].transform.x.y,
                    bones[i].transform.x.z,
                    0.0f
                }, {
                    bones[i].transform.y.x,
                    bones[i].transform.y.y,
                    bones[i].transform.y.z,
                    0.0f
                }, {
                    bones[i].transform.z.x,
                    bones[i].transform.z.y,
                    bones[i].transform.z.z,
                    0.0f
                }, {
                    bones[i].transform.w.x,
                    bones[i].transform.w.y,
                    bones[i].transform.w.z,
                    1.0f
                }
            };
        }
    }
};