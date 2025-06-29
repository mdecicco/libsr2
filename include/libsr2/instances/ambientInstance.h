#pragma once
#include <libsr2/types.h>
#include <libsr2/instances/PhysInstance.h>

namespace sr2 {
    class ambientAgent;
    class ambientData;
    class aiAmbientVehicleAudio;
    
    class ambientInstance : public PhysInstance {
        public:
            ambientInstance();
            ~ambientInstance();

            virtual mat3x4f getTransform() const;
            virtual void setTransform(const mat3x4f& transform);

            ambientData* getData() const;
            aiAmbientVehicleAudio* getImpactSound() const;

            void FUN_0017de08(bool param_1);

            f32 field_0xa4;
            bool field_0xac;
            undefined4 field_0xb0;
        
        private:
            ambientAgent* m_agent;
            aiAmbientVehicleAudio* m_impactSound;
    };
};