#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/SomeAudioThing5.h>

namespace sr2 {
    class SomeAudioThing6 : public SomeAudioThing5 {
        public:
            SomeAudioThing6();
            virtual ~SomeAudioThing6();

            void FUN_00328220();
            void FUN_003287d0();
            bool FUN_00328968();
            void FUN_00328da8(f32 p1);
            void FUN_00329078(f32 p1);
            void FUN_00329708(undefined4 p1);
    };
};