#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phContactManager;
    class phImpact;
    class phInst;
    class SpatialPartitioner;

    class dgPhysManagerBase {
        public:
            dgPhysManagerBase();
            virtual ~dgPhysManagerBase();

            void setSpatialPartitioner(SpatialPartitioner* spatialPartitioner);

            virtual void method_0x10();
            virtual void method_0x18(phImpact* impact);
            virtual void method_0x20();

            void FUN_00268800(phImpact* impact, u16 p2, u32 p3);
            bool FUN_00269280(f32 squaredImpulseMag, u16 p2);
            void FUN_002695d8(f32 squaredImpulseMag, f32 p2, phInst* inst, phImpact* impact, undefined4 p4);

        protected:
            phContactManager* m_contactManager;
            SpatialPartitioner* m_partitioner;
            i32 m_tempOversampleAmount;
            f32 m_invSeconds;
            f32 m_seconds;
            u32 m_liveCount;
            u32 m_otherLiveCount;
            u32* m_liveIndices;
            u32* m_otherLiveIndices;

            undefined4 m_field2_0x8;
            undefined4 m_field3_0xc;
            undefined4 m_field4_0x10;
            f32 m_field5_0x14;
            i32 m_field8_0x20;
    };
};