#pragma once
#include <libsr2/types.h>
#include <libsr2/audio/aud3DObject.h>

namespace sr2 {
    class audImpact;

    class aiAmbientVehicleAudio : public aud3DObject {
        public:
            aiAmbientVehicleAudio();
            virtual ~aiAmbientVehicleAudio();

            void FUN_0024afb0(f32 p1);

            audImpact* getPlayer() const;
        
        private:
            audImpact* m_impactPlayer;
    };
};