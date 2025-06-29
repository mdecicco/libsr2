#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phSleep.h>
#include <libsr2/sim/phUnkCollider.h>

namespace sr2 {
    struct phSegment;
    struct phIntersectionPoint;

    enum class SpatialPartitionerElementType {
        UNK0_ARR,
        UNK1_ARR,
        UNK2_ARR,
        UNK3_ARR,
        UNK3_ARR_AGAIN,
        UNKNOWN_INTENT
    };

    enum class SpatialPartitionerNodeArray {
        UNK3,
        UNK2,
        UNK0,
        UNK3_AGAIN,
        UNK1
    };
    
    class SpatialPartitioner {
        public:
            static SpatialPartitioner* get();
            static SpatialPartitioner* create();
            static void destroy();

            struct NodeBase {
                u16 next;
                u16 last;
            };

            struct Node1 : NodeBase {
                phInst inst;
                phInst* instRef;
                u16 cellIndices[2];
            };

            struct Node2 : NodeBase {
                u16 currentIndex;
                u16 cellIndex;
                phInst* inst;
            };

            struct Node3 : NodeBase {
                struct {
                    u16 cellIndex;
                    u16 field0x2;
                    phColliderBase* someCollider;
                    phUnkCollider collider;
                    u32 field0x8;
                    u32 field0xc;
                    phInst inst;
                    Node1* unk;
                } unk0;

                phInertialCS ics;
                phSleep sleep;
            };

            struct Cell {
                Cell();

                Node1* allocate(SpatialPartitionerNodeArray arrId, Node1* arr);
                Node2* allocate(SpatialPartitionerNodeArray arrId, Node2* arr);
                Node3* allocate(SpatialPartitionerNodeArray arrId, Node3* arr);

                void insertNode(SpatialPartitionerNodeArray arrId, Node1* node, Node1* arr, u16 cellIdx);
                void insertNode(SpatialPartitionerNodeArray arrId, Node2* node, Node2* arr, u16 cellIdx);
                void insertNode(SpatialPartitionerNodeArray arrId, Node3* node, Node3* arr, u16 cellIdx);
                
                u16 indices[5];
            };

            struct Search {
                Search();

                u16 capacity;
                u16 resultCount;
                u16* results;
            };

            void init();
            void setSomeBoolToFalse();
            void setWorldBounds(f32 cellSize, const vec3f& min, const vec3f& max);
            void initGrid();
            void FUN_00270080();
            bool testSphere(phColliderBase* collider, const vec3f& p2, const vec3f& p3, f32 radius);
            bool testSphere(phInst* inst, const vec3f& p2, const vec3f& p3, f32 radius);
            u16 insertInstance(phInst* inst);
            u16 insertInstance(phInst* inst, undefined4 p2);
            u16 insertEntity(PhysEntity* ent, const mat3x4f& transform);
            u16 insertBound(PhysEntity* ent, const mat3x4f& transform, undefined4 p3);
            u16 insertCollider(phColliderBase* collider);
            void setsSleepToNull(u16 liveIndicesIdx);
            void removeElemAtIdx(u16 idx);

            void initSearch();
            u16 calcCellIndex(const vec3f& pos) const;
            bool isIndexLive(u16 idx) const;
            void raiseLivenessFlag(u16 idx);
            void maybeReinsertAny(u16 idx);
            void freeLive(u16 idx);
            void setTransform(i32 idx, const mat3x4f& transform);
            void updateCellIndex(Node3* node);
            void updateCellIndex(Node2* node);
            void updateCellIndex(i32 idx);
            void updateUnk3ArrCellIndex(i32 idx);
            void updateUnk0ArrCellIndex(i32 idx);
            void updateCellIndices();
            u16 getSomeColliderIndexFrom0();
            u16 getSomeColliderIndex();
            mat3x4f* getTransform(i32 idx);
            bool isIndexNotInUnk0Arr(u16 idx) const;
            phColliderBase* getColliderAt(i32 idx);
            phColliderBase* getCollider(i32 idx);
            phInst* getPhInstFromUnk2(u16 idx);
            phInst* getPhInstFromUnk0(u16 idx);
            u32 getCellIndex(i32 idx);
            phInst* getPhys(i32 idx);
            phColliderBase* getSearch0ResultCollider(u32 idx);
            phInst* getSearch1ResultPhys(u32 idx);
            phInst* getSearch2ResultPhys(u32 idx);
            phInst* getSearch3ResultPhys(u32 idx);
            u16 getSearch0ResultCount();
            u16 getSearch1ResultCount();
            u16 getSearch2ResultCount();
            u16 getSearch3ResultCount();
            u16 getSearch0ResultIndex(u32 idx);
            u16 getSearch1ResultIndex(u32 idx);
            u16 getSearch2ResultIndex(u32 idx);
            u16 getSearch3ResultIndex(u32 idx);
            bool searchForNearbyEntities(f32 radius, const vec3f& pos, undefined4 unused, u32 matchFlags);
            void searchForNearbyUnk1(f32 radius, const vec3f& pos);
            undefined4 FUN_00273988();
            bool FUN_00273b20(u32 idx);
            bool FUN_00273b88(u32 idx);
            void FUN_00273c88(u16 idx);
            phColliderBase* getNewColliderFromUnk0(u32 idx);
            phColliderBase* getNewColliderFromUnk2(u32 idx);
            phColliderBase* getNewCollider(u16 idx);
            void setUseUnkColliderIdx1();
            void FUN_00274100();
            bool testSegment(phSegment& test, phIntersectionPoint& out, phInst* inst, u32 flags);
            bool testManySegments(u32 count, phSegment* tests, phIntersectionPoint* out, phInst* inst, u32 flags);
            bool testSphere(f32 radius, const vec3f& center, phIntersectionPoint* out, phInst* inst, u32 flags);
            void configureSphereQuery(f32 radius, const vec3f& center);
            void configureSegmentQuery(const vec3f& a, const vec3f& b);
            void configureMultiSegmentQuery(u32 count, phSegment* segments);
            i32 getNeighborCellIndex(i32 cellIndex, i32 offsetX, i32 offsetZ);

            SpatialPartitionerElementType getElementType(u16 idx) const;
            phInst* getPhInst(u16 idx) const;

        protected:
            SpatialPartitioner();
            ~SpatialPartitioner();
            static SpatialPartitioner* instance;
            static u32 SpatialQueryCellCount;
            static u32 SpatialQueryCellIndices[75];

            Node2* allocateArr0();
            Node1* allocateArr1();
            Node1* allocateArr2();
            Node3* allocateArr3();
            u16 allocateIndex();
            void freeIndex(u16 idx);
            void insertUnk0_Cell(Node2* node);
            void insertUnk1_Cell(Node1* node);
            void insertUnk2_Cell(Node1* node);
            void insertUnk3_Cell(Node3* node);

            void insertIntoUnk0Arr(Node2* node);
            void insertIntoUnk1Arr(Node1* node);
            void insertIntoUnk2Arr(Node1* node);
            void insertIntoUnk3Arr(Node3* node);
            void removeFromUnk0Arr(Node2* node);
            void removeFromUnk1Arr(Node1* node);
            void removeFromUnk2Arr(Node1* node);
            void removeFromUnk3Arr(Node3* node);
            void removeFromUnk3Arr_(Node3* node);
            void removeFromUnk2Arr(u16 idx);

            void maybeReinsertUnk0Arr(Node2* node);
            void maybeReinsertUnk1Arr(Node1* node);
            void maybeReinsertUnk2Arr(Node1* node);
            void maybeReinsertUnk3Arr(Node3* node);
            void maybeReinsertUnk3Arr_(Node3* node);

            void maybeReinsertUnk0ArrByIndex(u16 idx);

            void searchStep(u16 cellIdx, f32 radius, const vec3f& pos, u32 matchFlags);


            u16 m_field0x1a;
            u16 m_field0x26;
            u16 m_field0x5c;
            u16 m_field0x80;
            u16 m_field0x82;
            u16 m_field0x84;
            u16 m_field0x86;
            u16 m_field0x88;
            u16 m_field0x8a;
            u16 m_field0x8c;
            u16 m_field0x8e;
            

            bool m_someBool;
            u16 m_activeObjectCount;
            u16 m_someCount4;
            u16 m_allocationCount;
            u16* m_freeIndices;
            u16* m_liveIndices;
            SpatialPartitionerElementType* m_elementTypes;
            u16 m_someCount2;
            bool m_useUnkColliderIndex1;
            i16 m_unkColliderIndex0;
            i16 m_unkColliderIndex1;
            u16* m_colliderIndices;
            u8* m_colliderFlags;

            vec3f m_worldMin;
            vec3f m_worldMax;
            i16 m_scaledMinX;
            i16 m_scaledMaxX;
            i16 m_scaledMinZ;
            i16 m_scaledMaxZ;
            f32 m_cellSize;
            f32 m_invCellSize;
            u16 m_cellCountPlus3;

            u16 m_cellCount;
            Cell* m_cells;

            Search m_search0;
            Search m_search1;
            Search m_search2;
            Search m_search3;

            u16 m_totalCapacity;
            
            u16 m_unk0Capacity;
            u16 m_unk0Count;
            Node2* m_unk0Arr;

            u16 m_unk1Capacity;
            u16 m_unk1Count;
            Node1* m_unk1Arr;

            u16 m_unk2Capacity;
            u16 m_unk2Count;
            Node1* m_unk2Arr;

            u16 m_colliderCapacity;
            u16 m_unk3Count;
            Node3* m_unk3Arr;
    };
};