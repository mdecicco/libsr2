#include <libsr2/creature/crAnimation.h>
#include <libsr2/creature/crAnimFrame.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/stream.h>

namespace sr2 {
    std::unordered_map<const char*, crAnimation*> crAnimation::cachedAnims;

    crAnimation::crAnimation() {
        frameCount = 0;
        channelCount = 0;
        unk2 = 0;
        duration = 1.0f;
        unk4 = true;
        frames = nullptr;
        unk6 = 0;
        unk7 = -1;
    }

    crAnimation::~crAnimation() {
        if (frames) {
            delete [] frames;
            frames = nullptr;
        }
    }

    bool crAnimation::load(const char* dir, const char* file, i32 u0) {
        Stream* fp = datAssetManager::open(dir, file, "anim", u0, true);
        if (!fp) return false;

        i32 magic = -1;
        if (!fp->read(&magic, 4)) {
            fp->close();
            return false;
        }

        if (magic && magic != 0x706d63) {
            fp->close();
            return false;
        }

        if (!fp->read(&frameCount, 4)) {
            fp->close();
            return false;
        }

        if (!fp->read(&channelCount, 4)) {
            fp->close();
            return false;
        }

        if (!fp->read(&unk2, 4)) {
            fp->close();
            return false;
        }

        u8 someChar = fp->getCh();
        unk4 = someChar != 0;

        frames = new crAnimFrame[frameCount];

        if (magic) {
            // This path does exist in the game, but the decompilation
            // is quite mangled and the game files appear to never result
            // in this code path being executed... so I'll ignore it for
            // now
            delete [] frames;
            fp->close();
            abort();
        } else {
            for (u32 i = 0;i < frameCount;i++) {
                if (!frames[i].load(fp, channelCount)) {
                    delete [] frames;
                    frames = nullptr;
                    fp->close();
                    return false;
                }
            }

            fp->close();
            unk6 = 0;
            duration = 30.0f / f32(frameCount);
        }

        return true;
    }

    void crAnimation::normalize(i32 u0) {
        f32 t0 = u0 ? frames[0].buffer[2] : 0.0f;
        for (u32 i = 0;i < frameCount;i++) {
            frames[i].buffer[2] -= (f32(i) * (-unk2 / f32(frameCount)) + t0);
        }
    }

    void crAnimation::subtract(crAnimFrame* frame) {
        for (u32 i = 0;i < frameCount;i++) {
            frames[i].addScaled(frame, -1.0f, -1, -1);
        }
    }

    bool crAnimation::animExists(const char* dir, const char* file) {
        return datAssetManager::exists(dir, file, "anim");
    }

    bool crAnimation::animExists(const char* file) {
        return datAssetManager::exists("anim", file, "anim");
    }

    crAnimation* crAnimation::getAnim(const char* file, i32 param_3, i32 param_4, crAnimFrame* param_5, i32 param_6, i32 param_7) {
        return getAnimPrefix("anim", file, param_3, param_4, param_5, param_6, param_7);
    }

    crAnimation* crAnimation::getAnimPrefix(const char* prefix, const char* file, i32 param_3, i32 param_4, crAnimFrame* param_5, i32 param_6, i32 param_7) {
        auto existing = cachedAnims.find(file);
        if (existing == cachedAnims.end()) {
            crAnimation* anim = new crAnimation();
            anim->unk7 = param_7;
            anim->unk5 = param_3;

            if (!anim->load(prefix, file, param_6)) {
                anim->decRef();
                return nullptr;
            }

            cachedAnims[file] = anim;
            if (param_3) anim->normalize(param_4);
            if (param_5) anim->subtract(param_5);

            return anim;
        }

        crAnimation* anim = existing->second;
        anim->incRef();
        return anim;
    }
};