#include <libsr2/managers/datTimeManager.h>

namespace sr2 {
    datTimeManager::TIME_MODE datTimeManager::Mode = datTimeManager::REAL_TIME;
    u32 datTimeManager::FrameStep = 0;
    u32 datTimeManager::OverSamples = 0;
    f32 datTimeManager::FrameRate = 60.0f;
    u32 datTimeManager::FrameCount = 0;
    f32 datTimeManager::SampleStep = 0.016666668f;
    bool datTimeManager::TempOverSampling = false;
    i32 datTimeManager::TempOverSampleAmount = 0;
    bool datTimeManager::FirstFrame = true;
    f32 datTimeManager::Seconds = 0.016666668f;
    f32 datTimeManager::ActualSeconds = 0.016666668f;
    f32 datTimeManager::TempSeconds = 0.0f;
    f32 datTimeManager::InvSeconds = 60.0f;
    f32 datTimeManager::PrevElapsedTime = 0.0f;
    f32 datTimeManager::TimeWarp = 1.0f;
    f32 datTimeManager::ElapsedTime = 0.0f;
    f32 datTimeManager::ClampMin = 1.0e-4f;
    f32 datTimeManager::ClampMax = 0.1f;
    std::chrono::high_resolution_clock::time_point datTimeManager::Time = std::chrono::high_resolution_clock::now();

    void datTimeManager::reset() { 
        FirstFrame = 1;
        Seconds = 0.01666667f;
        InvSeconds = 60.0f;
        PrevElapsedTime = 0.0f;
        TimeWarp = 1.0f;
        ElapsedTime = 0.0f;
        FrameCount = 0;
    }

    void datTimeManager::realTime(f32 frameTime) {
        Mode = TIME_MODE::REAL_TIME;
        FrameRate = 60.0f;

        if (frameTime == 0.0f) {
            SampleStep = 0.0f;
            return;
        }

        SampleStep = 1.0f / frameTime;
    }
    
    void datTimeManager::fixedFrame(f32 frameRate, i32 overSamples) {
        FrameRate = frameRate;
        Mode = TIME_MODE::FIXED_FRAME;
        OverSamples = overSamples;

        f32 unk = overSamples;
        if (overSamples < 0) unk = f32(overSamples & 1 | overSamples >> 1) * 2.0f;

        SampleStep = 1.0f / (frameRate * unk);
        Seconds = SampleStep;
        InvSeconds = 1.0f / Seconds;
    }
    
    void datTimeManager::setTempOversampling(bool enable, i32 amount) {
        if (enable) {
            if (!TempOverSampling) Seconds = TempSeconds;
            TempOverSampling = true;
            
            f32 tmp = Seconds / f32(amount);
            TempSeconds = Seconds;
            TempOverSampleAmount = amount;
            Seconds = tmp;
            InvSeconds = 1.0f / tmp;
        }

        if (TempOverSampling) {
            TempOverSampling = false;
            InvSeconds = 1.0f / TempSeconds;
            Seconds = TempSeconds;
        }
    }

    void datTimeManager::update() {
        auto ct = std::chrono::high_resolution_clock::now();
        f32 deltaTime = std::chrono::duration<f32>(ct - Time).count() * TimeWarp;
        Time = ct;
        
        ActualSeconds = deltaTime;
        if (!FirstFrame) {
            PrevElapsedTime = ElapsedTime;
            if (FrameStep == 0 && Mode == 0) {
                ElapsedTime += deltaTime;
                Seconds = deltaTime;

                if (SampleStep != 0.0f) {
                    if (deltaTime < SampleStep) OverSamples = 1;
                    else Seconds = SampleStep;
                }
            } else {
                ElapsedTime += SampleStep;
                ActualSeconds = SampleStep;
                Seconds = SampleStep;
            }
        } else {
            ElapsedTime += deltaTime;
            Seconds = SampleStep;
            FirstFrame = false;
        }

        FrameCount++;
        if (Seconds < ClampMin) Seconds = ClampMin;
        if (Seconds > ClampMax) Seconds = ClampMax;
        InvSeconds = 1.0f / Seconds;
    }
};