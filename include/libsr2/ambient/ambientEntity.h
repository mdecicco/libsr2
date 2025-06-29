#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phSleep.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/ambient/ambientWheel.h>
#include <libsr2/particles/asParticles.h>
#include <libsr2/particles/asBirthRule.h>

namespace sr2 {
    class datCallback;
    class ambientInstance;

    class ambientEntity : public phColliderBase {
        public:
            ambientEntity();
            virtual ~ambientEntity();

            virtual void reset();
            virtual void update();
            virtual void move();

            void initFromCtor(phBound* bnd, phInertialCS* ics, phSleep* sleep);

        private:
            u32 m_spatialPartitionerIdx;
            f32 m_maxDamage;
            datCallback* m_impactCb;
            ambientInstance* m_ambInst;

            phInertialCS m_ics;
            phSleep m_sleep;
            phInst m_inst;
            ambientWheel m_wheels[4];

            asParticles field_0xe4;
            asParticles field_0x168;
            asBirthRule field_0x1ec;
            asBirthRule field_0x294;
            f32 m_currentDamage;

            static void ImpactCB(ambientEntity* self, phColliderImpactCallbackData* data);
    };
};