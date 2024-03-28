#pragma once
#include <libsr2/types.h>

#include <chrono>

namespace sr2 {
    class datTimeManager {
        public:
            enum TIME_MODE {
                REAL_TIME = 0,
                FIXED_FRAME = 1
            };

            static void reset();
            static void realTime(f32 frameTime);
            static void fixedFrame(f32 frameRate, i32 overSamples);
            static void setTempOversampling(bool enable, i32 amount);
            static void update();

            static TIME_MODE Mode;
            static u32 FrameStep;
            static u32 OverSamples;
            static f32 FrameRate;
            static u32 FrameCount;
            static f32 SampleStep;
            static bool TempOverSampling;
            static i32 TempOverSampleAmount;
            static bool FirstFrame;
            static f32 Seconds;
            static f32 ActualSeconds;
            static f32 TempSeconds;
            static f32 InvSeconds;
            static f32 PrevElapsedTime;
            static f32 TimeWarp;
            static f32 ElapsedTime;
            static f32 ClampMin;
            static f32 ClampMax;
            static std::chrono::high_resolution_clock::time_point Time;
    };
};