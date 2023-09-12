#pragma once
#include <libsr2/types.h>
#include <libsr2/utilities/datRefCount.h>
#include <unordered_map>

namespace sr2 {
    class crAnimFrame;
    class crAnimation : public datRefCount {
        public:
            crAnimation();
            virtual ~crAnimation();

            bool load(const char* dir, const char* file, i32 u0);
            void normalize(i32 u0);
            void subtract(crAnimFrame* frame);

            static bool animExists(const char* dir, const char* file);
            static bool animExists(const char* file);
            static crAnimation* getAnim(const char* file, i32 param_3, i32 param_4, crAnimFrame* param_5, i32 param_6, i32 param_7);
            static crAnimation* getAnimPrefix(const char* prefix, const char* file, i32 param_3, i32 param_4, crAnimFrame* param_5, i32 param_6, i32 param_7);

            i32 frameCount;
            i32 channelCount;
            i32 unk2;
            crAnimFrame* frames;
            f32 duration;
            bool unk4;
            i32 unk5;
            i32 unk6;
            i32 unk7;
        
        private:
            static std::unordered_map<const char*, crAnimation*> cachedAnims;
    };
};