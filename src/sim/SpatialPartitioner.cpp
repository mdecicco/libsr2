#include <libsr2/sim/SpatialPartitioner.h>
#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/PhysEntity.h>
#include <libsr2/sim/phBoundPolygonal.h>
#include <libsr2/sim/phCollisionPrim.h>
#include <libsr2/sim/phSegment.h>
#include <libsr2/sim/phIntersectionPoint.h>
#include <libsr2/managers/PhysEntityManager.h>
#include <libsr2/math/math.h>
#include <cstring>
#include <cmath>

#define ColliderFlagMemSize(colliderCapacity) ((colliderCapacity + 7) >> 3)

namespace sr2 {
    SpatialPartitioner::Search::Search() {
        capacity = 0x80;
        resultCount = 0;
        results = nullptr;
    }

    SpatialPartitioner::Cell::Cell() {
        indices[0] = 0xffff;
        indices[1] = 0xffff;
        indices[2] = 0xffff;
        indices[3] = 0xffff;
        indices[4] = 0xffff;
    }

    SpatialPartitioner::Node1* SpatialPartitioner::Cell::allocate(SpatialPartitionerNodeArray arrId, Node1* arr) {
        u16 idx = indices[(u32)arrId];
        if (idx == 0xffff) return nullptr;

        Node1* ele = &arr[idx];
        u16 last = ele->last;
        u16 next = ele->next;

        indices[(u32)arrId] = last;
        if (last != 0xffff) arr[last].next = next;
        if (next != 0xffff) arr[next].last = last;

        ele->last = ele->next = 0xffff;
        return ele;
    }

    SpatialPartitioner::Node2* SpatialPartitioner::Cell::allocate(SpatialPartitionerNodeArray arrId, Node2* arr) {
        u16 idx = indices[(u32)arrId];
        if (idx == 0xffff) return nullptr;

        Node2* ele = &arr[idx];
        u16 last = ele->last;
        u16 next = ele->next;

        indices[(u32)arrId] = last;
        if (last != 0xffff) arr[last].next = next;
        if (next != 0xffff) arr[next].last = last;

        ele->last = ele->next = 0xffff;
        return ele;
    }

    SpatialPartitioner::Node3* SpatialPartitioner::Cell::allocate(SpatialPartitionerNodeArray arrId, Node3* arr) {
        u16 idx = indices[(u32)arrId];
        if (idx == 0xffff) return nullptr;

        Node3* ele = &arr[idx];
        u16 last = ele->last;
        u16 next = ele->next;

        indices[(u32)arrId] = last;
        if (last != 0xffff) arr[last].next = next;
        if (next != 0xffff) arr[next].last = last;

        ele->last = ele->next = 0xffff;
        return ele;
    }

    void SpatialPartitioner::Cell::insertNode(SpatialPartitionerNodeArray arrId, SpatialPartitioner::Node1* node, SpatialPartitioner::Node1* arr, u16 cellIdx) {
        u16 idx = indices[(u32)arrId];
        u16 nodeIdx = u16(node - arr);

        if (idx != 0xffff) {
            node->last = idx;
            node->next = arr[idx].next;

            arr[arr[idx].next].last = nodeIdx;
            arr[idx].next = nodeIdx;
        }

        node->cellIndices[0] = cellIdx;
        indices[(u32)arrId] = nodeIdx;
    }

    void SpatialPartitioner::Cell::insertNode(SpatialPartitionerNodeArray arrId, SpatialPartitioner::Node2* node, SpatialPartitioner::Node2* arr, u16 cellIdx) {
        u16 idx = indices[(u32)arrId];
        u16 nodeIdx = u16(node - arr);

        if (idx != 0xffff) {
            node->last = idx;
            node->next = arr[idx].next;

            arr[arr[idx].next].last = nodeIdx;
            arr[idx].next = nodeIdx;
        }

        node->cellIndex = cellIdx;
        indices[(u32)arrId] = nodeIdx;
    }

    void SpatialPartitioner::Cell::insertNode(SpatialPartitionerNodeArray arrId, SpatialPartitioner::Node3* node, SpatialPartitioner::Node3* arr, u16 cellIdx) {
        u16 idx = indices[(u32)arrId];
        u16 nodeIdx = u16(node - arr);

        if (idx != 0xffff) {
            node->last = idx;
            node->next = arr[idx].next;

            arr[arr[idx].next].last = nodeIdx;
            arr[idx].next = nodeIdx;
        }

        node->unk0.cellIndex = cellIdx;
        indices[(u32)arrId] = nodeIdx;
    }



    SpatialPartitioner* SpatialPartitioner::instance = nullptr;
    u32 SpatialPartitioner::SpatialQueryCellCount = 0;
    u32 SpatialPartitioner::SpatialQueryCellIndices[75] = { 0 };

    SpatialPartitioner* SpatialPartitioner::get() {
        return instance;
    }

    SpatialPartitioner* SpatialPartitioner::create() {
        if (instance) return instance;
        instance = new SpatialPartitioner();
        return instance;
    }

    void SpatialPartitioner::destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }

    void SpatialPartitioner::init() {
        initSearch();
        initGrid();

        m_totalCapacity = m_unk0Capacity + m_unk1Capacity + m_unk2Capacity + m_colliderCapacity;

        m_unk0Arr = new Node2[m_unk0Capacity];
        for (u32 i = 0;i < m_unk0Capacity;i++) {
            m_unk0Arr[i].last = 0xffff;
            m_unk0Arr[i].next = 0xffff;
            m_unk0Arr[i].currentIndex = 0xffff;
            m_unk0Arr[i].cellIndex = 0xffff;
            m_unk0Arr[i].inst = nullptr;
        }

        m_unk1Arr = new Node1[m_unk1Capacity];
        for (u32 i = 0;i < m_unk1Capacity;i++) {
            m_unk1Arr[i].last = 0xffff;
            m_unk1Arr[i].next = 0xffff;
            m_unk1Arr[i].cellIndices[0] = 0xffff;
            m_unk1Arr[i].cellIndices[1] = 0xffff;
            m_unk1Arr[i].instRef = nullptr;
        }

        m_unk2Arr = new Node1[m_unk2Capacity];
        for (u32 i = 0;i < m_unk2Capacity;i++) {
            m_unk2Arr[i].last = 0xffff;
            m_unk2Arr[i].next = 0xffff;
            m_unk2Arr[i].cellIndices[0] = 0xffff;
            m_unk2Arr[i].cellIndices[1] = 0xffff;
            m_unk2Arr[i].instRef = &m_unk2Arr[i].inst;
        }

        m_unk3Arr = new Node3[m_colliderCapacity];
        for (u32 i = 0;i < m_colliderCapacity;i++) {
            m_unk3Arr[i].last = 0xffff;
            m_unk3Arr[i].next = 0xffff;
            m_unk3Arr[i].unk0.field0x2 = 0xffff;
            m_unk3Arr[i].unk0.someCollider = &m_unk3Arr[i].unk0.collider;
            m_unk3Arr[i].unk0.field0x8 = 0;
            m_unk3Arr[i].unk0.field0xc = 0;
            m_unk3Arr[i].unk0.unk = nullptr;
            m_unk3Arr[i].unk0.cellIndex = 0xffff;
            m_unk3Arr[i].unk0.collider.reset();
            m_unk3Arr[i].sleep.init(nullptr, nullptr);
        }

        m_colliderIndices = new u16[m_colliderCapacity];
        m_colliderFlags = new u8[ColliderFlagMemSize(m_colliderCapacity)];
        m_liveIndices = new u16[m_totalCapacity];
        m_freeIndices = new u16[m_totalCapacity];
        m_elementTypes = new SpatialPartitionerElementType[m_totalCapacity];

        for (u16 i = 0;i < m_totalCapacity;i++) {
            m_liveIndices[i] = 0xffff;
            m_freeIndices[i] = i;
            m_elementTypes[i] = SpatialPartitionerElementType::UNKNOWN_INTENT;
        }
        
        m_allocationCount = 0;
        for (u32 i = 0;i < m_unk1Capacity;i++) {
            insertIntoUnk1Arr(&m_unk1Arr[i]);
        }

        m_someCount4 = 0;
        for (u32 i = 0;i < m_unk0Capacity;i++) {
            insertIntoUnk0Arr(&m_unk0Arr[i]);
        }

        m_field0x26 = 0;
        for (u32 i = 0;i < m_unk2Capacity;i++) {
            insertIntoUnk2Arr(&m_unk2Arr[i]);
        }

        m_field0x1a = 0;
        for (u32 i = 0;i < m_colliderCapacity;i++) {
            insertIntoUnk3Arr(&m_unk3Arr[i]);
        }

        m_activeObjectCount = 0;
        m_someCount2 = 0;
        m_unkColliderIndex0 = 0;
        memset(m_colliderFlags, 0, ColliderFlagMemSize(m_colliderCapacity));

        m_unk0Count = 0;
        m_unk1Count = 0;
        m_unk2Count = 0;
        m_unk3Count = 0;
        m_someBool = true;
        m_field0x80 = 0;
        m_field0x82 = 0;
        m_field0x84 = 0;
        m_field0x86 = 0;
        m_field0x88 = 0;
        m_field0x8a = 0;
        m_field0x8c = 0;
        m_field0x8e = 0;
    }

    void SpatialPartitioner::setSomeBoolToFalse() {
        m_someBool = false;
    }

    void SpatialPartitioner::setWorldBounds(f32 cellSize, const vec3f& min, const vec3f& max) {
        m_cellSize = cellSize;
        m_worldMin = min;
        m_worldMax = max;
    }

    void SpatialPartitioner::initGrid() {
        m_invCellSize = 1.0f / m_cellSize;
        m_scaledMinX = floorf(m_worldMin.x * m_invCellSize);
        m_scaledMaxX = ceilf(m_worldMax.x * m_invCellSize);
        m_scaledMinZ = floorf(m_worldMin.z * m_invCellSize);
        m_scaledMaxZ = ceilf(m_worldMax.z * m_invCellSize);

        m_cellCount = ((m_scaledMaxX - m_scaledMinX) + 1) * ((m_scaledMaxZ - m_scaledMinZ) + 1);
        m_cellCountPlus3 = m_cellCount + 3;
        m_cells = new Cell[m_cellCountPlus3];
    }

    void SpatialPartitioner::FUN_00270080() {
        Node2* n0 = nullptr;
        u16 idx0 = m_cells[1].indices[u32(SpatialPartitionerNodeArray::UNK0)];
        if (idx0 != 0xffff) n0 = &m_unk0Arr[idx0];

        while (n0) {
            Node2* next = nullptr;
            if (n0->last != 0xffff) next = &m_unk0Arr[n0->last];

            u16 curIdx = n0->currentIndex;
            SpatialPartitionerElementType type = m_elementTypes[curIdx];
            u16 liveIndex = m_liveIndices[curIdx];

            switch (type) {
                case SpatialPartitionerElementType::UNK2_ARR: {
                    Node1* n2 = &m_unk2Arr[liveIndex];
                    if (n2->cellIndices[0] != 0xffff) {
                        removeFromUnk2Arr(n2);
                    }

                    insertIntoUnk2Arr(n2);
                    break;
                }
                case SpatialPartitionerElementType::UNK3_ARR: {
                    Node3* n3 = &m_unk3Arr[liveIndex];
                    if (n3->unk0.cellIndex != 0xffff) {
                        removeFromUnk3Arr(n3);
                    }

                    insertIntoUnk3Arr(n3);
                    break;
                }
                case SpatialPartitionerElementType::UNK3_ARR_AGAIN: {
                    Node3* n3 = &m_unk3Arr[liveIndex];
                    if (n3->unk0.cellIndex != 0xffff) {
                        removeFromUnk3Arr_(n3);
                    }

                    insertIntoUnk3Arr(n3);
                    break;
                }
                default: break;
            }

            removeFromUnk0Arr(n0);
            insertUnk0_Cell(n0);

            m_elementTypes[curIdx] = SpatialPartitionerElementType::UNK0_ARR;
            m_liveIndices[curIdx] = n0 - m_unk0Arr;
            n0 = next;
        }
        
        m_unkColliderIndex0 = 0;
        memset(m_colliderFlags, 0, ColliderFlagMemSize(m_colliderCapacity));

        for (u32 i = 2;i < m_cellCountPlus3;i++) {
            u16 idx3 = m_cells[i].indices[u32(SpatialPartitionerNodeArray::UNK3)];
            if (idx3 == 0xffff) continue;
            Node3* n3 = &m_unk3Arr[idx3];

            while (n3) {
                removeFromUnk3Arr(n3);
                insertUnk3_Cell(n3);

                if (n3->last == 0xffff) break;
                n3 = &m_unk3Arr[n3->last];
            }
        }
    }

    bool SpatialPartitioner::testSphere(phColliderBase* collider, const vec3f& p2, const vec3f& p3, f32 radius) {
        mat3x4f* rm = collider->getReadMatrix();

        vec3f delta = p3 - rm->w;
        vec3f center = rm->basis().multTransposed(delta);
        vec3f dir;
        f32 penetration;
        return collider->getInstance()->unk->bound->testSphere(center, radius, delta, dir, penetration);
    }

    bool SpatialPartitioner::testSphere(phInst* inst, const vec3f& p2, const vec3f& p3, f32 radius) {
        vec3f delta = p3 - inst->transform.w;
        vec3f center = inst->transform.basis().multTransposed(delta);

        vec3f dir;
        f32 penetration;
        return inst->unk->bound->testSphere(center, radius, delta, dir, penetration);
    }



    u16 SpatialPartitioner::insertInstance(phInst* inst) {
        u16 idx = allocateIndex();
        Node2* n = allocateArr0();

        if (!n) return 0xffff;

        n->inst = inst;
        n->currentIndex = idx;

        m_elementTypes[idx] = SpatialPartitionerElementType::UNK0_ARR;
        m_liveIndices[idx] = n - m_unk0Arr;

        insertUnk0_Cell(n);

        return idx;
    }

    u16 SpatialPartitioner::insertInstance(phInst* inst, undefined4 p2) {
        u16 idx = allocateIndex();
        Node3* n = allocateArr3();

        if (!n) return 0xffff;

        PhysEntityManager* pm = PhysEntityManager::get();

        n->unk0.someCollider = &n->unk0.collider;
        n->unk0.collider.setActive(true);
        pm->initEntity(inst->unk, &n->ics);
        n->ics.resetTransform();
        n->ics.active = true;
        
        Node1* n2 = allocateArr2();
        n2->cellIndices[1] = idx;
        n2->instRef = inst;
        n2->instRef->transform = inst->transform;
        n->unk0.unk = n2;

        n->unk0.someCollider->init(n2->instRef, &n->ics, &n->sleep);
        n->unk0.someCollider->reset();
        n->sleep.init(&n->ics, n->unk0.someCollider);

        n->unk0.field0x8 = p2;
        n->unk0.field0xc = 0;
        n->unk0.field0x2 = idx;

        m_elementTypes[idx] = SpatialPartitionerElementType::UNK3_ARR;
        m_liveIndices[idx] = n - m_unk3Arr;

        insertUnk3_Cell(n);

        return idx;
    }

    u16 SpatialPartitioner::insertEntity(PhysEntity* ent, const mat3x4f& transform) {
        u16 idx = allocateIndex();
        Node1* n = allocateArr1();

        if (!n) return 0xffff;
        
        n->inst.unk = ent;
        n->inst.transform = transform;
        n->instRef = &n->inst;
        n->cellIndices[1] = idx;

        m_elementTypes[idx] = SpatialPartitionerElementType::UNK1_ARR;
        m_liveIndices[idx] = n - m_unk1Arr;

        insertUnk1_Cell(n);

        return idx;
    }

    u16 SpatialPartitioner::insertBound(PhysEntity* ent, const mat3x4f& transform, undefined4 p3) {
        u16 idx = allocateIndex();
        Node3* n = allocateArr3();

        if (!n) return 0xffff;

        PhysEntityManager* pm = PhysEntityManager::get();

        n->unk0.someCollider = &n->unk0.collider;
        n->unk0.collider.setActive(true);
        pm->initEntity(ent, &n->ics);
        n->ics.resetTransform();
        n->ics.active = true;
        
        Node1* n2 = allocateArr2();
        n2->cellIndices[1] = idx;
        n2->instRef->unk = ent;
        n2->instRef->transform = transform;
        n->unk0.unk = n2;

        n->unk0.someCollider->init(n2->instRef, &n->ics, &n->sleep);
        n->unk0.someCollider->reset();
        n->sleep.init(&n->ics, n->unk0.someCollider);

        n->unk0.field0x8 = p3;
        n->unk0.field0xc = 0;
        n->unk0.field0x2 = idx;

        m_elementTypes[idx] = SpatialPartitionerElementType::UNK3_ARR;
        m_liveIndices[idx] = n - m_unk3Arr;

        insertUnk3_Cell(n);

        return idx;
    }

    u16 SpatialPartitioner::insertCollider(phColliderBase* collider) {
        u16 idx = allocateIndex();
        Node3* n = allocateArr3();

        if (!n) return 0xffff;

        n->unk0.someCollider = collider;
        n->unk0.field0xc = 0;
        n->unk0.field0x8 = 1;
        n->unk0.field0x2 = idx;

        Node1* n2 = allocateArr2();
        n2->cellIndices[1] = idx;
        n2->instRef = collider->getInstance();
        n->unk0.unk = n2;

        m_elementTypes[idx] = SpatialPartitionerElementType::UNK3_ARR;
        m_liveIndices[idx] = n - m_unk3Arr;

        insertUnk3_Cell(n);

        return idx;
    }
    


    void SpatialPartitioner::setsSleepToNull(u16 liveIndicesIdx) {
        u16 idx = m_liveIndices[liveIndicesIdx];
        m_unk3Arr[idx].unk0.field0x8 = 1;
        m_unk3Arr[idx].unk0.collider.setSleep(nullptr);
    }

    void SpatialPartitioner::removeElemAtIdx(u16 idx) {
        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK0_ARR: {
                Node2* n = &m_unk0Arr[m_liveIndices[idx]];
                if (n->cellIndex != 0xffff) removeFromUnk0Arr(n);

                insertIntoUnk0Arr(n);
                break;
            }
            case SpatialPartitionerElementType::UNK1_ARR: {
                Node1* n = &m_unk1Arr[m_liveIndices[idx]];
                if (n->cellIndices[0] != 0xffff) removeFromUnk1Arr(n);

                insertIntoUnk1Arr(n);
                break;
            }
            case SpatialPartitionerElementType::UNK2_ARR: {
                Node1* n = &m_unk2Arr[m_liveIndices[idx]];
                if (n->cellIndices[0] != 0xffff) removeFromUnk2Arr(n);

                insertIntoUnk2Arr(n);
                break;
            }
            case SpatialPartitionerElementType::UNK3_ARR: {
                Node3* n = &m_unk3Arr[m_liveIndices[idx]];
                if (n->unk0.cellIndex != 0xffff) removeFromUnk3Arr(n);

                insertIntoUnk3Arr(n);
                break;
            }
            case SpatialPartitionerElementType::UNK3_ARR_AGAIN: {
                Node3* n = &m_unk3Arr[m_liveIndices[idx]];
                if (n->unk0.cellIndex != 0xffff) removeFromUnk3Arr_(n);

                insertIntoUnk3Arr(n);
                break;
            }
            default: break;
        }

        m_liveIndices[idx] = 0xffff;
        m_elementTypes[idx] = SpatialPartitionerElementType::UNKNOWN_INTENT;
        freeIndex(idx);
    }

    SpatialPartitioner::Node2* SpatialPartitioner::allocateArr0() {
        return m_cells[0].allocate(SpatialPartitionerNodeArray::UNK0, m_unk0Arr);
    }

    SpatialPartitioner::Node1* SpatialPartitioner::allocateArr1() {
        return m_cells[0].allocate(SpatialPartitionerNodeArray::UNK1, m_unk1Arr);
    }

    SpatialPartitioner::Node1* SpatialPartitioner::allocateArr2() {
        return m_cells[0].allocate(SpatialPartitionerNodeArray::UNK2, m_unk2Arr);
    }

    SpatialPartitioner::Node3* SpatialPartitioner::allocateArr3() {
        return m_cells[0].allocate(SpatialPartitionerNodeArray::UNK3, m_unk3Arr);
    }

    u16 SpatialPartitioner::allocateIndex() {
        u32 freeIdx = m_totalCapacity - m_allocationCount;
        u16 idx = m_freeIndices[freeIdx - 1];
        m_freeIndices[freeIdx - 1] = 0xffff;
        m_allocationCount++;
        return idx;
    }

    void SpatialPartitioner::freeIndex(u16 idx) {
        u32 freeIdx = m_totalCapacity - m_allocationCount;
        m_freeIndices[freeIdx & 0xffff] = idx;
        m_allocationCount--;
    }

    void SpatialPartitioner::insertUnk0_Cell(Node2* node) {
        u16 cellIdx = calcCellIndex(node->inst->transform.w);
        if (cellIdx == 1) {
            m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK0, node, m_unk0Arr, 1);
            m_unk0Count++;
            return;
        }

        m_cells[cellIdx].insertNode(SpatialPartitionerNodeArray::UNK0, node, m_unk0Arr, cellIdx);
        m_elementTypes[node->cellIndex] = SpatialPartitionerElementType::UNK0_ARR;
        m_field0x26++;
    }

    void SpatialPartitioner::insertUnk1_Cell(Node1* node) {
        i32 cellIdx = calcCellIndex(node->instRef->transform.w);
        if (cellIdx == 1) {
            m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK1, node, m_unk1Arr, 1);
            m_unk1Count++;
            return;
        }

        i32 gridLength = (m_scaledMaxZ - m_scaledMinZ) + 1;
        if (gridLength == 0) exit(7);

        i32 gridZ = (cellIdx - 3) % gridLength;
        i32 gridU = (cellIdx - (gridZ + 3)) / gridLength;

        phInst* inst = node->instRef;

        f32 cellMinX = m_worldMin.x + (f32(gridU) - 0.5f) * m_cellSize;
        f32 cellMinZ = m_worldMin.z + (f32(gridZ) - 0.5f) * m_cellSize;
        f32 cellMaxX = cellMinX + m_cellSize + m_cellSize;
        f32 cellMaxZ = cellMinZ + m_cellSize + m_cellSize;

        phBound* bound = inst->unk->bound;
        switch (bound->type) {
            case phBoundType::BOUND_SPHERE:
            case phBoundType::BOUND_HOTDOG:
            case phBoundType::BOUND_BOX:
            case phBoundType::BOUND_GEOMETRY:
            case phBoundType::BOUND_UNK3: {
                phBoundPolygonal* geo = (phBoundPolygonal *)bound;
                for (u32 i = 0;i < geo->vertexCount;i++) {
                    vec3f v = inst->transform * geo->vertices[i];
                    if (v.x < cellMinX || v.x > cellMaxX || v.z < cellMinZ || v.z > cellMaxZ) {
                        // Vertex is outside of cell, so we need to insert into cell 2 for some reason
                        cellIdx = 2;
                        break;
                    }
                }

                break;
            }
            default: {
                vec3f& pos = inst->transform.w;

                bool longestExtentInBoundsX = pos.x - bound->longestExtentMag >= cellMinX && pos.x + bound->longestExtentMag <= cellMaxX;
                bool longestExtentInBoundsZ = pos.z - bound->longestExtentMag >= cellMinZ && pos.z + bound->longestExtentMag <= cellMaxZ;
                bool aabbInBoundsX = pos.x + bound->aabbMin.x >= cellMinX && pos.x + bound->aabbMax.x <= cellMaxX;
                bool aabbInBoundsZ = pos.z + bound->aabbMin.z >= cellMinZ && pos.z + bound->aabbMax.z <= cellMaxZ;
                
                if (!longestExtentInBoundsX || !longestExtentInBoundsZ || !aabbInBoundsX || !aabbInBoundsZ) {
                    // Node is/extends outside of the cell, so we need to insert into cell 2 for some reason
                    cellIdx = 2;
                }

                break;
            }
        }

        m_cells[cellIdx].insertNode(SpatialPartitionerNodeArray::UNK1, node, m_unk1Arr, cellIdx);
        m_someCount4++;
    }

    void SpatialPartitioner::insertUnk2_Cell(Node1* node) {
        u16 cellIdx = calcCellIndex(node->instRef->transform.w);
        if (cellIdx == 1) {
            m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK2, node, m_unk2Arr, 1);
            m_unk2Count++;
            return;
        }

        m_cells[cellIdx].insertNode(SpatialPartitionerNodeArray::UNK2, node, m_unk2Arr, cellIdx);
        m_elementTypes[node->cellIndices[1]] = SpatialPartitionerElementType::UNK2_ARR;
        m_liveIndices[node->cellIndices[1]] = node - m_unk2Arr;
        m_field0x1a++;
    }

    void SpatialPartitioner::insertUnk3_Cell(Node3* node) {
        u16 cellIdx = calcCellIndex(node->unk0.someCollider->getReadMatrix()->w);
        u16 nodeIdx = node - m_unk3Arr;

        if (cellIdx == 1) {
            m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK3, node, m_unk3Arr, 1);
            m_unk3Count++;
            return;
        }
        
        if (!m_useUnkColliderIndex1) {
            m_cells[cellIdx].insertNode(SpatialPartitionerNodeArray::UNK3, node, m_unk3Arr, cellIdx);
            m_elementTypes[node->unk0.field0x2] = SpatialPartitionerElementType::UNK3_ARR;
            m_activeObjectCount++;
        } else {
            m_cells[cellIdx].insertNode(SpatialPartitionerNodeArray::UNK3_AGAIN, node, m_unk3Arr, cellIdx);
            m_elementTypes[node->unk0.field0x2] = SpatialPartitionerElementType::UNK3_ARR_AGAIN;
            m_someCount2++;
        }

        m_liveIndices[node->unk0.field0x2] = nodeIdx;
        if (isIndexLive(nodeIdx)) {
            i16 prevIdx = m_unkColliderIndex0;
            m_unkColliderIndex0++;
            m_colliderIndices[prevIdx] = nodeIdx;
            raiseLivenessFlag(nodeIdx);
        }
    }

    void SpatialPartitioner::insertIntoUnk0Arr(Node2* node) {
        node->inst = nullptr;
        node->cellIndex = 0xffff;
        node->currentIndex = 0xffff;

        m_cells[0].insertNode(SpatialPartitionerNodeArray::UNK0, node, m_unk0Arr, 0);
    }

    void SpatialPartitioner::insertIntoUnk1Arr(Node1* node) {
        node->cellIndices[0] = 0xffff;
        node->cellIndices[1] = 0xffff;

        m_cells[0].insertNode(SpatialPartitionerNodeArray::UNK1, node, m_unk1Arr, 0);
    }

    void SpatialPartitioner::insertIntoUnk2Arr(Node1* node) {
        node->instRef = &node->inst;
        node->cellIndices[0] = 0xffff;
        node->cellIndices[1] = 0xffff;

        m_cells[0].insertNode(SpatialPartitionerNodeArray::UNK2, node, m_unk2Arr, 0);
    }

    void SpatialPartitioner::insertIntoUnk3Arr(Node3* node) {
        Node1* ele = node->unk0.unk;
        if (ele) {
            removeFromUnk2Arr(ele);
            insertIntoUnk2Arr(ele);
        }

        node->unk0.unk = nullptr;
        node->unk0.someCollider = &node->unk0.collider;
        node->unk0.field0x2 = 0xffff;
        node->unk0.field0x8 = 0;
        node->unk0.field0xc = 0;
        node->unk0.cellIndex = 0xffff;

        m_cells[0].insertNode(SpatialPartitionerNodeArray::UNK3, node, m_unk3Arr, 0);
    }

    void SpatialPartitioner::removeFromUnk0Arr(Node2* node) {
        if (node->cellIndex == 1) m_unk0Count--;
        else m_field0x26--;

        if (m_cells[node->cellIndex].indices[u32(SpatialPartitionerNodeArray::UNK0)] == node - m_unk0Arr) {
            m_cells[node->cellIndex].indices[u32(SpatialPartitionerNodeArray::UNK0)] = node->last;
        }
        
        u16 last = node->last;
        u16 next = node->next;

        if (last != 0xffff) m_unk1Arr[last].next = next;
        if (next != 0xffff) m_unk1Arr[next].last = last;

        node->cellIndex = node->last = node->next = 0xffff;
    }

    void SpatialPartitioner::removeFromUnk1Arr(Node1* node) {
        if (node->cellIndices[0] == 1) m_unk1Count--;
        else m_someCount4--;

        if (m_cells[node->cellIndices[0]].indices[u32(SpatialPartitionerNodeArray::UNK1)] == node - m_unk2Arr) {
            m_cells[node->cellIndices[0]].indices[u32(SpatialPartitionerNodeArray::UNK1)] = node->last;
        }
        
        u16 last = node->last;
        u16 next = node->next;

        if (last != 0xffff) m_unk1Arr[last].next = next;
        if (next != 0xffff) m_unk1Arr[next].last = last;

        node->cellIndices[0] = node->last = node->next = 0xffff;
    }

    void SpatialPartitioner::removeFromUnk2Arr(Node1* node) {
        if (node->cellIndices[0] == 1) m_unk2Count--;
        else m_field0x1a--;

        if (m_cells[node->cellIndices[0]].indices[u32(SpatialPartitionerNodeArray::UNK2)] == node - m_unk2Arr) {
            m_cells[node->cellIndices[0]].indices[u32(SpatialPartitionerNodeArray::UNK2)] = node->last;
        }
        
        u16 last = node->last;
        u16 next = node->next;

        if (last != 0xffff) m_unk2Arr[last].next = next;
        if (next != 0xffff) m_unk2Arr[next].last = last;

        node->cellIndices[0] = node->last = node->next = 0xffff;
    }

    void SpatialPartitioner::removeFromUnk3Arr(Node3* node) {
        if (node->unk0.cellIndex == 1) m_unk3Count--;
        else m_someCount2--;

        if (m_cells[node->unk0.cellIndex].indices[u32(SpatialPartitionerNodeArray::UNK3)] == node - m_unk3Arr) {
            m_cells[node->unk0.cellIndex].indices[u32(SpatialPartitionerNodeArray::UNK3)] = node->last;
        }
        
        u16 last = node->last;
        u16 next = node->next;

        if (last != 0xffff) m_unk3Arr[last].next = next;
        if (next != 0xffff) m_unk3Arr[next].last = last;

        node->unk0.cellIndex = node->last = node->next = 0xffff;
    }

    void SpatialPartitioner::removeFromUnk3Arr_(Node3* node) {
        if (node->unk0.cellIndex == 1) m_unk3Count--;
        else m_someCount2--;

        if (m_cells[node->unk0.cellIndex].indices[u32(SpatialPartitionerNodeArray::UNK3_AGAIN)] == node - m_unk3Arr) {
            m_cells[node->unk0.cellIndex].indices[u32(SpatialPartitionerNodeArray::UNK3_AGAIN)] = node->last;
        }

        u16 last = node->last;
        u16 next = node->next;

        if (last != 0xffff) m_unk3Arr[last].next = next;
        if (next != 0xffff) m_unk3Arr[next].last = last;

        node->unk0.cellIndex = node->last = node->next = 0xffff;
    }

    void SpatialPartitioner::removeFromUnk2Arr(u16 idx) {
        Node1* node = &m_unk2Arr[idx];
        u16 last = node->last;
        u16 next = node->next;

        if (m_field0x5c == idx) m_field0x5c = last;
        
        if (last != 0xffff) m_unk2Arr[last].next = next;
        if (next != 0xffff) m_unk2Arr[next].last = last;

        node->last = node->next = 0xffff;
    }

    void SpatialPartitioner::maybeReinsertUnk0Arr(Node2* node) {
        removeFromUnk0Arr(node);
        m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK0, node, m_unk0Arr, 1);
        m_unk0Count++;
    }

    void SpatialPartitioner::maybeReinsertUnk1Arr(Node1* node) {
        removeFromUnk1Arr(node);
        m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK1, node, m_unk1Arr, 1);
        m_unk1Count++;
    }

    void SpatialPartitioner::maybeReinsertUnk2Arr(Node1* node) {
        removeFromUnk2Arr(node);
        m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK2, node, m_unk2Arr, 1);
        m_unk2Count++;
    }

    void SpatialPartitioner::maybeReinsertUnk3Arr(Node3* node) {
        removeFromUnk3Arr(node);
        m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK3, node, m_unk3Arr, 1);
        m_unk3Count++;
    }

    void SpatialPartitioner::maybeReinsertUnk3Arr_(Node3* node) {
        removeFromUnk3Arr_(node);
        m_cells[1].insertNode(SpatialPartitionerNodeArray::UNK3_AGAIN, node, m_unk3Arr, 1);
        m_unk3Count++;
    }
    
    void SpatialPartitioner::maybeReinsertUnk0ArrByIndex(u16 idx) {
        maybeReinsertUnk0Arr(&m_unk0Arr[idx]);
    }    

    void SpatialPartitioner::maybeReinsertAny(u16 idx) {
        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK0_ARR:
                maybeReinsertUnk0Arr(&m_unk0Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK1_ARR:
                maybeReinsertUnk1Arr(&m_unk1Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK2_ARR:
                maybeReinsertUnk2Arr(&m_unk2Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK3_ARR:
                maybeReinsertUnk3Arr(&m_unk3Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK3_ARR_AGAIN:
                maybeReinsertUnk3Arr_(&m_unk3Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNKNOWN_INTENT:
                exit(1);
                break;
            default: break;
        }
    }

    void SpatialPartitioner::freeLive(u16 idx) {
        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK0_ARR:
                removeFromUnk0Arr(&m_unk0Arr[m_liveIndices[idx]]);
                insertUnk0_Cell(&m_unk0Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK1_ARR:
                removeFromUnk1Arr(&m_unk1Arr[m_liveIndices[idx]]);
                insertUnk1_Cell(&m_unk1Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK2_ARR:
                removeFromUnk2Arr(&m_unk2Arr[m_liveIndices[idx]]);
                insertUnk2_Cell(&m_unk2Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK3_ARR:
                removeFromUnk3Arr(&m_unk3Arr[m_liveIndices[idx]]);
                insertUnk3_Cell(&m_unk3Arr[m_liveIndices[idx]]);
                break;
            case SpatialPartitionerElementType::UNK3_ARR_AGAIN:
                removeFromUnk3Arr_(&m_unk3Arr[m_liveIndices[idx]]);
                insertUnk3_Cell(&m_unk3Arr[m_liveIndices[idx]]);
                break;
            default: break;
        }
    }

    void SpatialPartitioner::setTransform(i32 idx, const mat3x4f& transform) {
        if (idx < 0 || m_elementTypes[idx] != SpatialPartitionerElementType::UNK3_ARR) return;

        Node3* node = &m_unk3Arr[m_liveIndices[idx]];
        removeFromUnk3Arr(node);

        phColliderBase* collider = node->unk0.someCollider;
        phInertialCS* ics = collider->getICS();
        ics->resetTransform();
        ics->active = true;
        
        collider->getInstance()->transform = transform;
        collider->reset();

        phSleep* sleep = collider->getSleep();
        if (sleep) sleep->init(ics, collider);

        insertUnk3_Cell(node);
    }

    void SpatialPartitioner::updateCellIndex(Node3* node) {
        u16 cellIdx = calcCellIndex(node->unk0.someCollider->getReadMatrix()->w);
        if (cellIdx == node->unk0.cellIndex) return;

        removeFromUnk3Arr(node);
        insertUnk3_Cell(node);
    }

    void SpatialPartitioner::updateCellIndex(Node2* node) {
        u16 cellIdx = calcCellIndex(node->inst->transform.w);
        if (cellIdx == node->cellIndex) return;

        removeFromUnk0Arr(node);
        insertUnk0_Cell(node);
    }

    void SpatialPartitioner::updateCellIndex(i32 idx) {
        if (idx < 0) return;

        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK3_ARR: {
                Node3* node = &m_unk3Arr[m_liveIndices[idx]];
                updateCellIndex(node);
                break;
            }
            case SpatialPartitionerElementType::UNK0_ARR: {
                Node2* node = &m_unk0Arr[m_liveIndices[idx]];
                updateCellIndex(node);
                break;
            }
            default: break;
        }
    }

    void SpatialPartitioner::updateUnk3ArrCellIndex(i32 idx) {
        updateCellIndex(&m_unk3Arr[idx]);
    }

    void SpatialPartitioner::updateUnk0ArrCellIndex(i32 idx) {
        updateCellIndex(&m_unk0Arr[idx]);
    }

    void SpatialPartitioner::updateCellIndices() {
        for (u32 i = 2; i < m_cellCountPlus3; i++) {
            u16 idx = m_cells[i].indices[u32(SpatialPartitionerNodeArray::UNK3)];

            while (idx != 0xffff) {
                Node3* node = &m_unk3Arr[idx];

                updateCellIndex(node);

                idx = node->last;
            }
        }
    }

    static u16 nextColliderIndex = 0xffff;

    u16 SpatialPartitioner::getSomeColliderIndexFrom0() {
        nextColliderIndex = 0xffff;
        return getSomeColliderIndex();
    }

    u16 SpatialPartitioner::getSomeColliderIndex() {
        if (nextColliderIndex == 0xffff) nextColliderIndex = 0;
        else nextColliderIndex++;

        if (!m_useUnkColliderIndex1 && m_unkColliderIndex0 <= nextColliderIndex) return 0xffff;
        else if (m_useUnkColliderIndex1 && m_unkColliderIndex1 <= nextColliderIndex) return 0xffff;

        u16 colliderIdx = m_colliderIndices[nextColliderIndex];
        u16 cellIdx = m_unk3Arr[m_colliderIndices[nextColliderIndex]].unk0.cellIndex;

        while (cellIdx <= 1) {
            m_colliderFlags[colliderIdx >> 3] &= ~(1 << (colliderIdx & 0x7));

            m_unkColliderIndex0--;
            if (m_unkColliderIndex0 == nextColliderIndex) return 0xffff;

            u16 nextIndex = m_colliderIndices[m_unkColliderIndex0];
            m_colliderIndices[nextColliderIndex] = nextIndex;
            colliderIdx = nextIndex;
            cellIdx = m_unk3Arr[colliderIdx].unk0.cellIndex;
        }

        return colliderIdx;
    }

    mat3x4f* SpatialPartitioner::getTransform(i32 idx) {
        if (idx < 0) return nullptr;

        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK0_ARR: {
                return &m_unk0Arr[m_liveIndices[idx]].inst->transform;
            }
            case SpatialPartitionerElementType::UNK1_ARR: {
                return &m_unk1Arr[m_liveIndices[idx]].instRef->transform;
            }
            case SpatialPartitionerElementType::UNK2_ARR: {
                return &m_unk2Arr[m_liveIndices[idx]].instRef->transform;
            }
            case SpatialPartitionerElementType::UNK3_ARR: {
                return m_unk3Arr[m_liveIndices[idx]].unk0.someCollider->getReadMatrix();
            }
            default: break;
        }

        return nullptr;
    }

    bool SpatialPartitioner::isIndexNotInUnk0Arr(u16 idx) const {
        return getElementType(m_unk0Arr[idx].currentIndex) != SpatialPartitionerElementType::UNK0_ARR;
    }

    phColliderBase* SpatialPartitioner::getColliderAt(i32 idx) {
        if (idx < 0 || idx > m_colliderCapacity) return nullptr;
        return m_unk3Arr[idx].unk0.someCollider;
    }

    phColliderBase* SpatialPartitioner::getCollider(i32 idx) {
        if (idx < 0) return nullptr;

        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK3_ARR:
            case SpatialPartitionerElementType::UNK3_ARR_AGAIN: {
                return m_unk3Arr[m_liveIndices[idx]].unk0.someCollider;
            }
            default: break;
        }

        return nullptr;
    }

    phInst* SpatialPartitioner::getPhInstFromUnk2(u16 idx) {
        return m_unk2Arr[idx].instRef;
    }

    phInst* SpatialPartitioner::getPhInstFromUnk0(u16 idx) {
        return m_unk0Arr[idx].inst;
    }

    u32 SpatialPartitioner::getCellIndex(i32 idx) {
        if (idx < 0 || idx >= m_totalCapacity) return 0xffffffff;

        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK0_ARR: {
                return m_unk0Arr[m_liveIndices[idx]].cellIndex;
            }
            case SpatialPartitionerElementType::UNK1_ARR: {
                return m_unk1Arr[m_liveIndices[idx]].cellIndices[0];
            }
            case SpatialPartitionerElementType::UNK2_ARR: {
                return m_unk2Arr[m_liveIndices[idx]].cellIndices[0];
            }
            case SpatialPartitionerElementType::UNK3_ARR:
            case SpatialPartitionerElementType::UNK3_ARR_AGAIN: {
                return m_unk3Arr[m_liveIndices[idx]].unk0.cellIndex;
            }
            default: break;
        }

        return 0xffffffff;
    }

    phInst* SpatialPartitioner::getPhys(i32 idx) {
        if (idx < 0 || idx >= m_totalCapacity) return nullptr;

        switch (m_elementTypes[idx]) {
            case SpatialPartitionerElementType::UNK0_ARR: {
                return m_unk0Arr[m_liveIndices[idx]].inst;
            }
            case SpatialPartitionerElementType::UNK1_ARR: {
                return m_unk1Arr[m_liveIndices[idx]].instRef;
            }
            case SpatialPartitionerElementType::UNK2_ARR: {
                return m_unk2Arr[m_liveIndices[idx]].instRef;
            }
            case SpatialPartitionerElementType::UNK3_ARR:
            case SpatialPartitionerElementType::UNK3_ARR_AGAIN: {
                return m_unk3Arr[m_liveIndices[idx]].unk0.someCollider->getInstance();
            }
            default: break;
        }

        return nullptr;
    }
    
    phColliderBase* SpatialPartitioner::getSearch0ResultCollider(u32 idx) {
        return m_unk3Arr[m_search0.results[idx]].unk0.someCollider;
    }

    phInst* SpatialPartitioner::getSearch1ResultPhys(u32 idx) {
        return m_unk2Arr[m_search1.results[idx]].instRef;
    }

    phInst* SpatialPartitioner::getSearch2ResultPhys(u32 idx) {
        return m_unk0Arr[m_search2.results[idx]].inst;
    }
    
    phInst* SpatialPartitioner::getSearch3ResultPhys(u32 idx) {
        return m_unk1Arr[m_search3.results[idx]].instRef;
    }
    
    u16 SpatialPartitioner::getSearch0ResultCount() {
        return m_search0.resultCount;
    }

    u16 SpatialPartitioner::getSearch1ResultCount() {
        return m_search1.resultCount;
    }

    u16 SpatialPartitioner::getSearch2ResultCount() {
        return m_search2.resultCount;
    }

    u16 SpatialPartitioner::getSearch3ResultCount() {
        return m_search3.resultCount;
    }

    u16 SpatialPartitioner::getSearch0ResultIndex(u32 idx) {
        return m_search0.results[idx];
    }

    u16 SpatialPartitioner::getSearch1ResultIndex(u32 idx) {
        return m_search1.results[idx];
    }
    
    u16 SpatialPartitioner::getSearch2ResultIndex(u32 idx) {
        return m_search2.results[idx];
    }

    u16 SpatialPartitioner::getSearch3ResultIndex(u32 idx) {
        return m_search3.results[idx];
    }

    void SpatialPartitioner::searchStep(u16 cellIdx, f32 radius, const vec3f& pos, u32 matchFlags) {
        if (matchFlags & 2) {
            u16 idx = 0;
            
            idx = m_cells[cellIdx].indices[0];
            while (idx != 0xffff) {
                Node3* node = &m_unk3Arr[idx];
                PhysEntity* unk = node->unk0.someCollider->getInstance()->unk;

                if (matchFlags & unk->flags && unk->otherFlags & 2) {
                    vec3f delta = pos - unk->bound->getCenter(*node->unk0.someCollider->getReadMatrix());
                    f32 max_dist = radius + unk->bound->aabbRadius;
                    if (max_dist * max_dist > delta.lengthSq()) {
                        m_search0.results[m_search0.resultCount++] = (u16)(node - m_unk3Arr);
                    }
                }

                idx = node->last;
            }

            idx = m_cells[cellIdx].indices[3];
            while (idx != 0xffff) {
                Node3* node = &m_unk3Arr[idx];
                PhysEntity* unk = node->unk0.someCollider->getInstance()->unk;

                if (matchFlags & unk->flags && unk->otherFlags & 2) {
                    vec3f delta = pos - unk->bound->getCenter(*node->unk0.someCollider->getReadMatrix());
                    f32 max_dist = radius + unk->bound->aabbRadius;
                    if (max_dist * max_dist > delta.lengthSq()) {
                        m_search0.results[m_search0.resultCount++] = (u16)(node - m_unk3Arr);
                    }
                }

                idx = node->last;
            }
        }

        if (matchFlags & 4) {
            u16 idx = m_cells[cellIdx].indices[1];
            while (idx != 0xffff) {
                Node1* node = &m_unk2Arr[idx];
                PhysEntity* unk = node->instRef->unk;

                if (matchFlags & unk->flags && unk->otherFlags & 2) {
                    vec3f delta = pos - unk->bound->getCenter(node->instRef->transform);
                    f32 max_dist = radius + unk->bound->aabbRadius;
                    if (max_dist * max_dist > delta.lengthSq()) {
                        m_search1.results[m_search1.resultCount++] = (u16)(node - m_unk2Arr);
                    }
                }

                idx = node->last;
            }
        }

        if (matchFlags & 8) {
            u16 idx = m_cells[cellIdx].indices[2];
            while (idx != 0xffff) {
                Node2* node = &m_unk0Arr[idx];
                PhysEntity* unk = node->inst->unk;

                if (matchFlags & unk->flags && unk->otherFlags & 2) {
                    vec3f delta = pos - unk->bound->getCenter(node->inst->transform);
                    f32 max_dist = radius + unk->bound->aabbRadius;
                    if (max_dist * max_dist > delta.lengthSq()) {
                        m_search2.results[m_search2.resultCount++] = (u16)(node - m_unk0Arr);
                    }
                }

                idx = node->last;
            }
        }
    }

    bool SpatialPartitioner::searchForNearbyEntities(f32 radius, const vec3f& pos, undefined4 unused, u32 matchFlags) {
        i32 ny;
        i32 next_ny;
        
        m_search0.resultCount = 0;
        m_search1.resultCount = 0;
        m_search2.resultCount = 0;
        m_search3.resultCount = 0;

        u16 centerCellIdx = calcCellIndex(pos);

        for (i32 nx = -1; nx <= 1; nx++) {
            for (i32 nz = -1; nz <= 1; nz++) {
                u16 cellIdx = getNeighborCellIndex(centerCellIdx, nx, nz);
                if (cellIdx < 0) continue;
                
                searchStep(cellIdx, radius, pos, matchFlags);
            }
        }

        searchStep(2, radius, pos, matchFlags);

        if ((matchFlags & 1) != 0) {
            searchForNearbyUnk1(radius, pos);
        }

        return m_search0.resultCount + m_search1.resultCount + m_search2.resultCount + m_search3.resultCount > 0;
    }

    void SpatialPartitioner::searchForNearbyUnk1(f32 radius, const vec3f& pos) {
        u16 centerCellIdx = calcCellIndex(pos);

        for (i32 nx = -1; nx <= 1; nx++) {
            for (i32 nz = -1; nz <= 1; nz++) {
                u16 cellIdx = getNeighborCellIndex(centerCellIdx, nx, nz);
                if (cellIdx < 0) continue;
                
                u16 idx = m_cells[cellIdx].indices[4];
                while (idx != 0xffff) {
                    Node1* node = &m_unk1Arr[idx];
                    PhysEntity* unk = node->instRef->unk;

                    vec3f delta = pos - unk->bound->getCenter(node->instRef->transform);
                    f32 max_dist = radius + unk->bound->aabbRadius;
                    if (max_dist * max_dist > delta.lengthSq()) {
                        m_search3.results[m_search3.resultCount++] = (u16)(node - m_unk1Arr);
                    }

                    idx = node->last;
                }
            }
        }
    }

    undefined4 SpatialPartitioner::FUN_00273988() { 
        for (u32 i = 2;i < m_cellCountPlus3;i++) {
            u16 idx = 0;
            
            idx = m_cells[i].indices[(u32)SpatialPartitionerNodeArray::UNK0];
            while (idx != 0xffff) {
                Node2* node = &m_unk0Arr[idx];
                getNewColliderFromUnk0(node - m_unk0Arr);

                idx = node->last;
            }

            // Actual code for the loop below (including the asm???) describes a
            // loop that would never end...
            //
            // See:
            // 00273a94    beql    s0, zero, LAB_00273ad4 --.  (if s0 == 0 skip the loop)
            // 00273a98    _lhu    v0, 0xb8(s2)             |
            // 00273a9c    lui     s1, 0xaaaa               |
            // 00273aa0    ori     s1, s1, 0xaaab           |
            // 00273aa4    lw      a1, 0x70(s2)             |
            // LAB_00273aa8 <-----------------------------. |
            // 00273aa8    lw      v1, 0x3c(s2)           | |
            // 00273aac    subu    a1, s0, a1             | |
            // 00273ab0    mult    a1, a1, s1             | |
            // 00273ab4    lh      a0, 0x160(v1)          | |
            // 00273ab8    lw      v0, 0x164(v1)          | |
            // 00273abc    addu    a0, s2, a0             | |
            // 00273ac0    jalr    v0                     | |  (this call does not change s0, I checked)
            // 00273ac4    _sra    a1, a1, 0x5            | |  (notice: s0 has not been changed)
            // 00273ac8    bnel    s0, zero, LAB_00273aa8./ |  (if s0 != 0, repeat the loop)
            // 00273acc    _lw     a1, 0x70(s2)             |
            // 00273ad0    lhu     v0, 0xb8(s2)             |
            // LAB_00273ad4 <-------------------------------`
            // 00273ad4    ...and so on...
            //
            // I'm not sure what this loop is supposed to do, but I have to assume
            // this is correct because an infinite loop just doesn't make sense.
            // And this seems to be the pattern in the rest of the file.

            idx = m_cells[i].indices[(u32)SpatialPartitionerNodeArray::UNK2];
            while (idx != 0xffff) {
                Node1* node = &m_unk2Arr[idx];
                getNewColliderFromUnk2(node - m_unk2Arr);

                idx = node->last;
            }
        }
        
        return 1;
    }

    bool SpatialPartitioner::FUN_00273b20(u32 idx) {
        bool elemIsUnk3 = m_elementTypes[idx] == SpatialPartitionerElementType::UNK3_ARR;
        bool elemIsUnk3Again = m_elementTypes[idx] == SpatialPartitionerElementType::UNK3_ARR_AGAIN;
        if (elemIsUnk3 || elemIsUnk3Again) {
            return false;
        }

        FUN_00273b88(m_liveIndices[idx]);
        return true;
    }

    bool SpatialPartitioner::FUN_00273b88(u32 idx) {
        Node3* node = &m_unk3Arr[idx];

        if (!m_useUnkColliderIndex1) {
            removeFromUnk3Arr(node);
            Node1* n1 = node->unk0.unk;
            node->unk0.unk = nullptr;

            removeFromUnk2Arr(n1 - m_unk2Arr);
            insertUnk2_Cell(n1);

            m_elementTypes[n1->cellIndices[1]] = SpatialPartitionerElementType::UNK2_ARR;
            m_liveIndices[n1->cellIndices[1]] = n1 - m_unk2Arr;
            insertIntoUnk3Arr(node);
            return true;
        }

        if (node->unk0.field0x8) return false;

        node->unk0.field0xc = 1;
        return true;
    }

    void SpatialPartitioner::FUN_00273c88(u16 idx) {
        if (m_field0x5c == 0xffff) {
            m_field0x5c = idx;
            return;
        }

        Node1* node = &m_unk2Arr[idx];

        u16 last = node->last;
        node->next = node - m_unk2Arr;

        if (last != 0xffff) m_unk2Arr[last].next = node->next;
        node->last = node->next;
    }

    phColliderBase* SpatialPartitioner::getNewColliderFromUnk0(u32 idx) {
        Node3* node = allocateArr3();
        if (!node) return nullptr;

        Node1* n2 = &m_unk2Arr[idx];
        removeFromUnk2Arr(n2 - m_unk2Arr);
        FUN_00273c88(n2 - m_unk2Arr);

        node->unk0.unk = n2;
        node->unk0.someCollider = &node->unk0.collider;
        node->unk0.collider.setActive(true);

        PhysEntityManager* pm = PhysEntityManager::get();
        phInertialCS* ics = &node->ics;
        pm->initEntity(n2->instRef->unk, ics);
        ics->freeze();
        ics->active = true;

        node->unk0.someCollider->init(n2->instRef, ics, &node->sleep);
        node->unk0.someCollider->setMaxMovedDist(0.0f);
        node->unk0.someCollider->setBarelyMoved(true);

        node->sleep.init(ics, node->unk0.someCollider);
        node->unk0.field0x2 = n2->cellIndices[1];
        node->unk0.field0x8 = 0;
        node->unk0.field0xc = 0;

        insertUnk3_Cell(node);
        return node->unk0.someCollider;
    }

    phColliderBase* SpatialPartitioner::getNewColliderFromUnk2(u32 idx) {
        Node3* node = allocateArr3();
        if (!node) return nullptr;

        Node2* n2 = &m_unk0Arr[idx];
        maybeReinsertUnk0Arr(n2);

        Node1* n3 = allocateArr2();
        node->unk0.unk = n3;
        node->unk0.someCollider = &node->unk0.collider;
        node->unk0.collider.setActive(true);
        n3->instRef->transform = n2->inst->transform;
        n3->instRef->unk = n2->inst->unk;
        n3->cellIndices[1] = n2->currentIndex;

        PhysEntityManager* pm = PhysEntityManager::get();
        phInertialCS* ics = &node->ics;
        pm->initEntity(n3->instRef->unk, ics);
        ics->freeze();
        ics->active = true;

        node->unk0.someCollider->init(n3->instRef, ics, &node->sleep);
        node->unk0.someCollider->setMaxMovedDist(0.0f);
        node->unk0.someCollider->setBarelyMoved(true);

        node->sleep.init(ics, node->unk0.someCollider);
        node->unk0.field0x2 = n2->currentIndex;
        node->unk0.field0x8 = 0;
        node->unk0.field0xc = 0;

        insertUnk3_Cell(node);
        return node->unk0.someCollider;
    }

    phColliderBase* SpatialPartitioner::getNewCollider(u16 idx) {
        switch (getElementType(idx)) {
            case SpatialPartitionerElementType::UNK0_ARR: return getNewColliderFromUnk0(m_liveIndices[idx]);
            case SpatialPartitionerElementType::UNK2_ARR: return getNewColliderFromUnk2(m_liveIndices[idx]);
            default: break;
        }
        
        return nullptr;
    }

    void SpatialPartitioner::setUseUnkColliderIdx1() {
        m_useUnkColliderIndex1 = true;
        m_unkColliderIndex1 = m_activeObjectCount;
    }

    void SpatialPartitioner::FUN_00274100() {
        m_useUnkColliderIndex1 = false;

        for (u32 i = 0;i < m_unkColliderIndex0;i++) {
            Node3* node = &m_unk3Arr[m_colliderIndices[i]];
            if (m_elementTypes[node->unk0.field0x2] == SpatialPartitionerElementType::UNK3_ARR && node->unk0.field0xc) {
                FUN_00273b88(m_colliderIndices[i]);
            } else if (m_elementTypes[node->unk0.field0x2] == SpatialPartitionerElementType::UNK3_ARR_AGAIN) {
                removeFromUnk3Arr_(node);
                insertUnk3_Cell(node);
            }
        }

        m_unkColliderIndex1 = -1;
    }

    bool SpatialPartitioner::testSegment(phSegment& test, phIntersectionPoint& out, phInst* inst, u32 flags) {
        mat3x4f* outputTransform = nullptr;

        out.field10_0x30 = 0;
        out.field4_0x20 = 2.0f;
        out.instance = nullptr;

        configureSegmentQuery(test.a, test.b);

        for (u32 i = -1;i < SpatialPartitioner::SpatialQueryCellCount;i++) {
            u32 cellIndex = i == -1 ? 2 : SpatialPartitioner::SpatialQueryCellIndices[i];
            Cell* cell = &m_cells[cellIndex];

            if (flags & 1) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK1];
                while (idx != 0xffff) {
                    Node1* node = &m_unk1Arr[idx];
                    if (node->instRef == inst || (flags & node->instRef->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = node->instRef->unk->bound;
                    mat3x4f& T = node->instRef->transform;

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);
                    if (!prim.segmentSphereTest(test, prim.center, bound->aabbRadius)) {
                        idx = node->last;
                        continue;
                    }

                    phSegment seg;
                    seg.unk = 0;
                    seg.a = T.basis().multTransposed(test.a - T.w);
                    seg.b = T.basis().multTransposed(test.b - T.w);

                    if (bound->testSegmentPoint(seg, out.field4_0x20, &out)) {
                        out.instance = node->instRef;
                        out.someCellIdx = node->cellIndices[1];
                        out.field9_0x2c = nullptr;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }

            if (flags & 2) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK3];

                while (idx != 0xffff) {
                    Node3* node = &m_unk3Arr[idx];
                    phInst* nInst = node->unk0.someCollider->getInstance();
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = *node->unk0.someCollider->getReadMatrix();

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);
                    if (!prim.segmentSphereTest(test, prim.center, bound->aabbRadius)) {
                        idx = node->last;
                        continue;
                    }

                    phSegment seg;
                    seg.unk = test.unk;
                    seg.a = T.basis().multTransposed(test.a - T.w);
                    seg.b = T.basis().multTransposed(test.b - T.w);

                    if (bound->testSegmentPoint(seg, out.field4_0x20, &out)) {
                        out.instance = nInst;
                        out.someCellIdx = node->unk0.field0x2;
                        out.field9_0x2c = node->unk0.someCollider;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }

            if (flags & 4) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK2];

                while (idx != 0xffff) {
                    Node1* node = &m_unk2Arr[idx];
                    phInst* nInst = node->instRef;
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = node->instRef->transform;

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);
                    if (!prim.segmentSphereTest(test, prim.center, bound->aabbRadius)) {
                        idx = node->last;
                        continue;
                    }

                    phSegment seg;
                    seg.unk = test.unk;
                    seg.a = T.basis().multTransposed(test.a - T.w);
                    seg.b = T.basis().multTransposed(test.b - T.w);

                    if (bound->testSegmentPoint(seg, out.field4_0x20, &out)) {
                        out.instance = nInst;
                        out.someCellIdx = node->cellIndices[1];
                        out.field9_0x2c = nullptr;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }

            if (flags & 8) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK0];

                while (idx != 0xffff) {
                    Node2* node = &m_unk0Arr[idx];
                    phInst* nInst = node->inst;
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = node->inst->transform;

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);
                    if (!prim.segmentSphereTest(test, prim.center, bound->aabbRadius)) {
                        idx = node->last;
                        continue;
                    }

                    phSegment seg;
                    seg.unk = test.unk;
                    seg.a = T.basis().multTransposed(test.a - T.w);
                    seg.b = T.basis().multTransposed(test.b - T.w);

                    if (bound->testSegmentPoint(seg, out.field4_0x20, &out)) {
                        out.instance = nInst;
                        out.someCellIdx = node->currentIndex;
                        out.field9_0x2c = nullptr;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }
        }

        if (!outputTransform) return false;

        out.transform(*outputTransform);
        return true;
    }

    bool SpatialPartitioner::testManySegments(u32 count, phSegment* tests, phIntersectionPoint* out, phInst* inst, u32 flags) {
        mat3x4f* outputTransforms[64] = { nullptr };

        for (u32 i = 0;i < count;i++) {
            out[i].field10_0x30 = 0;
            out[i].field4_0x20 = 2.0f;
            out[i].instance = nullptr;
        }

        configureMultiSegmentQuery(count, tests);

        for (u32 i = -1;i < SpatialPartitioner::SpatialQueryCellCount;i++) {
            u32 cellIndex = i == -1 ? 2 : SpatialPartitioner::SpatialQueryCellIndices[i];
            Cell* cell = &m_cells[cellIndex];

            if (flags & 1) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK1];
                while (idx != 0xffff) {
                    Node1* node = &m_unk1Arr[idx];
                    if (node->instRef == inst || (flags & node->instRef->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = node->instRef->unk->bound;
                    mat3x4f& T = node->instRef->transform;

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);
                    for (u32 j = 0;j < count;j++) {
                        if (!prim.segmentSphereTest(tests[j], prim.center, bound->aabbRadius)) continue;

                        phSegment seg;
                        seg.unk = 0;
                        seg.a = T.basis().multTransposed(tests[j].a - T.w);
                        seg.b = T.basis().multTransposed(tests[j].b - T.w);

                        if (bound->testSegmentPoint(seg, out[j].field4_0x20, &out[j])) {
                            out[j].instance = node->instRef;
                            out[j].someCellIdx = node->cellIndices[1];
                            out[j].field9_0x2c = nullptr;
                            outputTransforms[j] = &T;
                        }
                    }

                    idx = node->last;
                }
            }

            if (flags & 2) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK3];

                while (idx != 0xffff) {
                    Node3* node = &m_unk3Arr[idx];
                    phInst* nInst = node->unk0.someCollider->getInstance();
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = *node->unk0.someCollider->getReadMatrix();

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);
                    for (u32 j = 0;j < count;j++) {
                        if (!prim.segmentSphereTest(tests[j], prim.center, bound->aabbRadius)) continue;

                        phSegment seg;
                        seg.unk = tests[j].unk;
                        seg.a = T.basis().multTransposed(tests[j].a - T.w);
                        seg.b = T.basis().multTransposed(tests[j].b - T.w);

                        if (bound->testSegmentPoint(seg, out[j].field4_0x20, &out[j])) {
                            out[j].instance = nInst;
                            out[j].someCellIdx = node->unk0.field0x2;
                            out[j].field9_0x2c = node->unk0.someCollider;
                            outputTransforms[j] = &T;
                        }
                    }

                    idx = node->last;
                }
            }

            if (flags & 4) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK2];

                while (idx != 0xffff) {
                    Node1* node = &m_unk2Arr[idx];
                    phInst* nInst = node->instRef;
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = node->instRef->transform;

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);

                    for (u32 j = 0;j < count;j++) {
                        if (!prim.segmentSphereTest(tests[j], prim.center, bound->aabbRadius)) continue;

                        phSegment seg;
                        seg.unk = tests[j].unk;
                        seg.a = T.basis().multTransposed(tests[j].a - T.w);
                        seg.b = T.basis().multTransposed(tests[j].b - T.w);

                        if (bound->testSegmentPoint(seg, out[j].field4_0x20, &out[j])) {
                            out[j].instance = nInst;
                            out[j].someCellIdx = node->cellIndices[1];
                            out[j].field9_0x2c = nullptr;
                            outputTransforms[j] = &T;
                        }
                    }

                    idx = node->last;
                }
            }

            if (flags & 8) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK0];

                while (idx != 0xffff) {
                    Node2* node = &m_unk0Arr[idx];
                    phInst* nInst = node->inst;
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = node->inst->transform;

                    phCollisionPrim prim;
                    prim.center = bound->getCenter(T);

                    for (u32 j = 0;j < count;j++) {
                        if (!prim.segmentSphereTest(tests[j], prim.center, bound->aabbRadius)) continue;

                        phSegment seg;
                        seg.unk = tests[j].unk;
                        seg.a = T.basis().multTransposed(tests[j].a - T.w);
                        seg.b = T.basis().multTransposed(tests[j].b - T.w);

                        if (bound->testSegmentPoint(seg, out[j].field4_0x20, &out[j])) {
                            out[j].instance = nInst;
                            out[j].someCellIdx = node->currentIndex;
                            out[j].field9_0x2c = nullptr;
                            outputTransforms[j] = &T;
                        }
                    }

                    idx = node->last;
                }
            }
        }

        bool found = false;
        for (u32 i = 0;i < count;i++) {
            if (outputTransforms[i]) {
                out[i].transform(*outputTransforms[i]);
                found = true;
            }
        }

        return found;
    }

    bool SpatialPartitioner::testSphere(f32 radius, const vec3f& center, phIntersectionPoint* out, phInst* inst, u32 flags) {
        configureSphereQuery(radius, center);

        mat3x4f* outputTransform = nullptr;
        out->penetration = -0.1f;
        out->instance = nullptr;

        for (i32 i = -1; i < SpatialPartitioner::SpatialQueryCellCount;i++) {
            u32 cellIndex = i == -1 ? 2 : SpatialPartitioner::SpatialQueryCellIndices[i];
            Cell* cell = &m_cells[cellIndex];

            if (flags & 1) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK1];
                while (idx != 0xffff) {
                    Node1* node = &m_unk1Arr[idx];
                    if (node->instRef == inst || (flags & node->instRef->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = node->instRef->unk->bound;
                    mat3x4f& T = node->instRef->transform;
                    vec3f transformedCenter = T.basis().multTransposed(center - T.w);

                    vec3f projection;
                    vec3f normal;
                    f32 penetration;
                    if (bound->testSphere(radius, transformedCenter, projection, normal, penetration) && penetration < out->penetration) {
                        out->instance = node->instRef;
                        out->someCellIdx = node->cellIndices[1];
                        out->contactPoint = projection;
                        out->normal = normal;
                        out->field6_0x28 = 0;
                        out->penetration = penetration;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }

            if (flags & 2) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK3];

                while (idx != 0xffff) {
                    Node3* node = &m_unk3Arr[idx];
                    phInst* nInst = node->unk0.someCollider->getInstance();
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = *node->unk0.someCollider->getReadMatrix();
                    vec3f transformedCenter = T.basis().multTransposed(center - T.w);

                    vec3f projection;
                    vec3f normal;
                    f32 penetration;
                    if (bound->testSphere(radius, transformedCenter, projection, normal, penetration) && penetration < out->penetration) {
                        out->instance = nInst;
                        out->someCellIdx = node->unk0.field0x2;
                        out->contactPoint = projection;
                        out->normal = normal;
                        out->field6_0x28 = 0;
                        out->penetration = penetration;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }

            if (flags & 4) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK2];

                while (idx != 0xffff) {
                    Node1* node = &m_unk2Arr[idx];
                    phInst* nInst = node->instRef;
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = node->instRef->transform;
                    vec3f transformedCenter = T.basis().multTransposed(center - T.w);

                    vec3f projection;
                    vec3f normal;
                    f32 penetration;
                    if (bound->testSphere(radius, transformedCenter, projection, normal, penetration) && penetration < out->penetration) {
                        out->instance = node->instRef;
                        out->someCellIdx = node->cellIndices[1];
                        out->contactPoint = projection;
                        out->normal = normal;
                        out->field6_0x28 = 0;
                        out->penetration = penetration;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }

            if (flags & 8) {
                u16 idx = cell->indices[(i32)SpatialPartitionerNodeArray::UNK0];

                while (idx != 0xffff) {
                    Node2* node = &m_unk0Arr[idx];
                    phInst* nInst = node->inst;
                    if (nInst == inst || (flags & nInst->unk->flags) == 0) {
                        idx = node->last;
                        continue;
                    }

                    phBound* bound = nInst->unk->bound;
                    mat3x4f& T = node->inst->transform;
                    vec3f transformedCenter = T.basis().multTransposed(center - T.w);

                    vec3f projection;
                    vec3f normal;
                    f32 penetration;
                    if (bound->testSphere(radius, transformedCenter, projection, normal, penetration) && penetration < out->penetration) {
                        out->instance = node->inst;
                        out->someCellIdx = node->currentIndex;
                        out->contactPoint = projection;
                        out->normal = normal;
                        out->field6_0x28 = 0;
                        out->penetration = penetration;
                        outputTransform = &T;
                    }

                    idx = node->last;
                }
            }
        }
        
        if (!outputTransform) return false;

        out->transform(*outputTransform);
        return true;
    }

    void SpatialPartitioner::configureSphereQuery(f32 radius, const vec3f& center) {
        f32 minX = floorf(((center.x - radius) - m_cellSize) * m_invCellSize);
        f32 minZ = floorf(((center.z - radius) - m_cellSize) * m_invCellSize);
        f32 maxX = floorf(((center.x + radius) + m_cellSize) * m_invCellSize);
        f32 maxZ = floorf(((center.z + radius) + m_cellSize) * m_invCellSize);

        i32 sx0 = math::max(i32(minX) - m_scaledMinX, 0);
        i32 sz0 = math::max(i32(minZ) - m_scaledMinZ, 0);
        i32 sx1 = math::min(i32(maxX) - m_scaledMinX, m_scaledMaxX - m_scaledMinX);
        i32 sz1 = math::min(i32(maxZ) - m_scaledMinZ, m_scaledMaxZ - m_scaledMinZ);

        SpatialPartitioner::SpatialQueryCellCount = 0;
        i32 span = (i32(m_scaledMaxZ) - i32(m_scaledMinZ)) + 1;

        for (i32 x = sx0; x <= sx1 && SpatialQueryCellCount < 75; x++) {
            for (i32 z = sz0; z <= sz1 && SpatialQueryCellCount < 75; z++) {
                SpatialQueryCellIndices[SpatialQueryCellCount++] = (x * span + z) + 3;
            }
        }
    }

    void SpatialPartitioner::configureSegmentQuery(const vec3f& a, const vec3f& b) {
        f32 halfCellSz = m_cellSize * 0.5f;

        f32 minX = floorf((math::min(a.x, b.x) - halfCellSz) * m_invCellSize);
        f32 minZ = floorf((math::min(a.z, b.z) - halfCellSz) * m_invCellSize);
        f32 minX = floorf((math::max(a.x, b.x) + halfCellSz + 1.0f) * m_invCellSize);
        f32 maxZ = floorf((math::max(a.z, b.z) + halfCellSz + 1.0f) * m_invCellSize);

        i32 sx0 = math::max(i32(minX) - m_scaledMinX, 0);
        i32 sz0 = math::max(i32(minZ) - m_scaledMinZ, 0);
        i32 sx1 = math::min(i32(minX) - m_scaledMinX, m_scaledMaxX - m_scaledMinX);
        i32 sz1 = math::min(i32(maxZ) - m_scaledMinZ, m_scaledMaxZ - m_scaledMinZ);

        SpatialQueryCellCount = 0;
        i32 span = (i32(m_scaledMaxZ) - i32(m_scaledMinZ)) + 1;

        for (i32 x = sx0; x <= sx1 && SpatialQueryCellCount < 75; x++) {
            for (i32 z = sz0; z <= sz1 && SpatialQueryCellCount < 75; z++) {
                SpatialQueryCellIndices[SpatialQueryCellCount++] = (x * span + z) + 3;
            }
        }
    }
    
    void SpatialPartitioner::configureMultiSegmentQuery(u32 count, phSegment* tests) {
        f32 halfCellSz = m_cellSize * 0.5f;
        f32 minX = FLT_MAX;
        f32 minZ = FLT_MAX;
        f32 maxX = FLT_MIN;
        f32 maxZ = FLT_MIN;

        for (u32 i = 0;i < count;i++) {
            phSegment& seg = tests[i];

            if (seg.a.x < seg.b.x) {
                minX = math::min(minX, seg.a.x);
                maxX = math::max(maxX, seg.b.x);
            } else {
                minX = math::min(minX, seg.b.x);
                maxX = math::max(maxX, seg.a.x);
            }

            if (seg.a.z < seg.b.z) {
                minZ = math::min(minZ, seg.a.z);
                maxZ = math::max(maxZ, seg.b.z);
            } else {
                minZ = math::min(minZ, seg.b.z);
                maxZ = math::max(maxZ, seg.a.z);
            }
        }

        i32 sx0 = math::max(i32(minX) - m_scaledMinX, 0);
        i32 sz0 = math::max(i32(minZ) - m_scaledMinZ, 0);
        i32 sx1 = math::min(i32(minX) - m_scaledMinX, m_scaledMaxX - m_scaledMinX);
        i32 sz1 = math::min(i32(maxZ) - m_scaledMinZ, m_scaledMaxZ - m_scaledMinZ);


        SpatialQueryCellCount = 0;
        i32 span = (i32(m_scaledMaxZ) - i32(m_scaledMinZ)) + 1;

        for (i32 x = sx0; x <= sx1 && SpatialQueryCellCount < 75; x++) {
            for (i32 z = sz0; z <= sz1 && SpatialQueryCellCount < 75; z++) {
                SpatialQueryCellIndices[SpatialQueryCellCount++] = (x * span + z) + 3;
            }
        }
    }

    i32 SpatialPartitioner::getNeighborCellIndex(i32 cellIndex, i32 offsetX, i32 offsetZ) {
        if (cellIndex <= 2) return -1;

        i32 width = (m_scaledMaxX - m_scaledMinX) + 1;
        i32 length = (m_scaledMaxZ - m_scaledMinZ) + 1;
        if (length == 0) exit(7);

        i32 cellX = (cellIndex - 3) / length + offsetX;
        if (cellX < 0 || cellX >= width) return -1;

        i32 cellZ = (cellIndex - 3) % length + offsetZ;
        if (cellZ < 0 || cellZ >= length) return -1;

        return length * offsetX + cellIndex + offsetZ;
    }
    
    void SpatialPartitioner::initSearch() {
        m_search0.results = new u16[m_search0.capacity];
        m_search1.results = new u16[m_search1.capacity];
        m_search2.results = new u16[m_search2.capacity];
        m_search3.results = new u16[m_search3.capacity];
    }

    u16 SpatialPartitioner::calcCellIndex(const vec3f& pos) const {
        if (pos.x > m_worldMax.x || pos.z > m_worldMax.z || pos.x < m_worldMin.x || pos.z < m_worldMin.z) return 1;

        i16 cx = i16(floorf((pos.x - m_worldMin.x) * m_invCellSize));
        i16 cz = i16(floorf((pos.z - m_worldMin.z) * m_invCellSize));

        return cx * ((m_scaledMaxZ - m_scaledMinZ) + 1) + cz + 3;
    }

    bool SpatialPartitioner::isIndexLive(u16 idx) const {
        return (m_colliderFlags[idx >> 3] & 1 << (idx & 0x7)) != 0;
    }

    void SpatialPartitioner::raiseLivenessFlag(u16 idx) {
        m_colliderFlags[idx >> 3] |= 1 << (idx & 0x7);
    }

    SpatialPartitionerElementType SpatialPartitioner::getElementType(u16 idx) const {
        return SpatialPartitionerElementType::UNK0_ARR;
    }

    phInst* SpatialPartitioner::getPhInst(u16 idx) const {
        return nullptr;
    }

    SpatialPartitioner::SpatialPartitioner() {
        m_someBool = false;
        
        m_unk0Capacity = 8192;
        m_unk1Capacity = 8192;
        m_unk2Capacity = 4096;

        m_colliderCapacity = 1024;
        m_useUnkColliderIndex1 = false;
        m_unkColliderIndex1 = -1;
        m_field0x5c = 0xffff;

        setWorldBounds(200.0f, vec3f(-999.0f, -999.0f, -999.0f), vec3f(999.0f, 999.0f, 999.0f));
    }

    SpatialPartitioner::~SpatialPartitioner() {
    }
};