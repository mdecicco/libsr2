#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phColliderBase;
    
    class phContactManager {
        public:
            phContactManager();
            ~phContactManager();

            void setTiming(f32 seconds);
            bool testHeldContact(phColliderBase* bodyA, phColliderBase* bodyB, u32* p3);
            void calcHeldContacts(phColliderBase* bodyA, phColliderBase* bodyB, u32* p3);
            void addHeldContact(phColliderBase* bodyA, phColliderBase* bodyB);
        
        protected:
            f32 m_seconds;
            f32 m_invSeconds;

            undefined4 m_field10_0x34;
    };
};