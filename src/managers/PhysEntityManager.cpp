#include <libsr2/managers/PhysEntityManager.h>
#include <libsr2/sim/PhysEntity.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phBoundSphere.h>
#include <libsr2/sim/phBoundHotdog.h>
#include <libsr2/sim/phBoundBox.h>
#include <libsr2/sim/phBoundGeometry.h>
#include <libsr2/sim/phBoundTerrain.h>
#include <libsr2/math/vec3f.h>

namespace sr2 {
    PhysEntityManager* PhysEntityManager::instance = nullptr;

    PhysEntityManager::PhysEntityManager(u32 _capacity) {
        entities = nullptr;
        freeCount = 0;
        usedCount = 0;
        capacity = 0;
        unk0 = 400.0f;

        init(_capacity);
    }

    PhysEntityManager::~PhysEntityManager() {
    }

    void PhysEntityManager::init(u32 _capacity) {
        deinit();
        usedCount = 0;
        freeCount = _capacity;
        capacity = _capacity;
        entities = new PhysEntity[_capacity];
    }

    void PhysEntityManager::deinit() {
        if (!entities) return;
        for (i32 i = usedCount - 1;i >= 0;i--) {
            if (entities[i].bound) delete entities[i].bound;
            delete entities[i].unk1;
        }

        delete [] entities;
        freeCount = usedCount = capacity = 0;

        entityIdxMap.clear();
    }

    void PhysEntityManager::addEntity(f32 u0, f32 mass, phBound* bound, phBoundType type, const char* name, const vec3f& u2, undefined* u3, const vec3f& angInertia, i32* outIdx) {
        std::string hash = PhysEntityManager::genHashStr(u0, type, name, u2, { 0.0f, 0.0f, 0.0f });
        auto& it = entityIdxMap.find(hash);
        if (it == entityIdxMap.end()) {
            u32 idx = insert(bound, hash);
            entities[idx].unk1 = u3;
            entities[idx].unk0 = u0;
            entities[idx].mass = mass;
            entities[idx].invMass = 1.0f / mass;
            entities[idx].angInertia = angInertia;
            entities[idx].invAngInertia = {
                1.0f / angInertia.x,
                1.0f / angInertia.y,
                1.0f / angInertia.z
            };
            entities[idx].unk2 = u2;
            if (outIdx) *outIdx = idx;
            return;
        }

        if (outIdx) *outIdx = it->second;
    }

    phBound* PhysEntityManager::addEntity(f32 u0, phBoundType type, const char* name, const vec3f& u1, undefined* u2, i32* outIdx, const vec3f& center, bool u5) {
        std::string hash = PhysEntityManager::genHashStr(u0, type, name, u1, center);
        auto& it = entityIdxMap.find(hash);
        if (it != entityIdxMap.end()) {
            if (outIdx) *outIdx = it->second;
            return entities[it->second].bound;
        }

        vec3f unk;
        phInertialCS ics;
        phBound* out = nullptr;

        // Note: It appears as though the results from the `phBound::load` calls
        // are memory leaks

        switch (type) {
            case BOUND_SPHERE: {
                phBoundSphere* bnd = new phBoundSphere(1.0f);
                bnd->setRadius(u1.x);

                unk.x = u1.x;
                if (u5) {
                    phBound* b = phBound::load(name);
                    if (b) unk.x = bnd->aabbMax.x - bnd->aabbMin.x;
                }

                bnd->centerBound(center);
                ics.initSphereDensity(u0, unk.x * 0.5f);

                out = bnd;
                break;
            }
            case BOUND_HOTDOG: {
                phBoundHotdog* bnd = new phBoundHotdog();
                if (u5) {
                    phBound* b = phBound::load(name);
                    if (b) {
                        bnd->scaleBoundingBox(u1.x, u1.y);
                        unk.x = bnd->aabbMax.x - bnd->aabbMin.x;
                        unk.y = bnd->aabbMax.y - bnd->aabbMin.y;
                    }
                } else {
                    bnd->setBoundingBox(u1.x, u1.y);
                    unk.x = u1.x;
                    unk.y = u1.y;
                }

                bnd->centerBound(center);
                ics.initHotdogDensity(u0, unk.x * 0.5f, unk.y - unk.x, 'y');

                out = bnd;
                break;
            }
            case BOUND_BOX: {
                phBoundBox* bnd = new phBoundBox();
                if (u5) {
                    phBound* b = phBound::load(name);
                    if (b) {
                        bnd->setSize(u1);
                        math::sub(unk, bnd->aabbMax, bnd->aabbMin);
                    } else {
                        bnd->setSize(u1);
                        unk = u1;
                    }
                } else {
                    bnd->setSize(u1);
                    unk = u1;
                }

                bnd->centerBound(center);
                ics.initBoxDensity(u0, unk.x, unk.y, unk.z);

                out = bnd;
                break;
            }
            case BOUND_GEOMETRY: {
                phBoundGeometry* bnd = new phBoundGeometry(BOUND_GEOMETRY);
                bnd->load(name, nullptr);
                bnd->scaleSize(u1.x, u1.y, u1.z);
                ics.initFromGeometry(u0, bnd->vertices, bnd->polygons, bnd->polyCount, nullptr);
                bnd->centerBound(center);
                out = bnd;
                break;
            }
            case BOUND_TERRAIN: {
                phBoundTerrain* bnd = new phBoundTerrain();
                if (!bnd->load(name)) {
                    // Physics::UnkTerrainRelated::UnkTerrainRelated(&UStack_d0);
                    // Physics::UnkTerrainRelated::FUN_002aa368(&UStack_d0,this->unk0,(phBoundTerrain *)bnd);
                    // Physics::UnkTerrainRelated::~UnkTerrainRelated(&UStack_d0,2);
                }
                out = bnd;
                break;
            }
            case BOUND_UNK0: {
                if (u5) {
                    phBoundGeometry* bnd = new phBoundGeometry(BOUND_GEOMETRY);
                    bnd->load(name, nullptr);
                    out = bnd;
                    break;
                }
                
                break;
            }
            case BOUND_UNK1: {
                if (u5) {
                    phBoundGeometry* bnd = new phBoundGeometry(BOUND_GEOMETRY);
                    bnd->load(name, nullptr);
                    out = bnd;
                    break;
                }

                break;
            }
            case BOUND_UNK2: {
                if (u5) {
                    phBoundGeometry* bnd = new phBoundGeometry(BOUND_GEOMETRY);
                    bnd->load(name, nullptr);
                    out = bnd;
                    break;
                }

                break;
            }
            default: {

            }
        }

        if (!out) abort();

        u32 idx = insert(out, hash);
        entities[idx].unk0 = u0;
        entities[idx].unk1 = u2;
        entities[idx].unk2 = u1;
        entities[idx].mass = ics.mass;
        entities[idx].invMass = ics.inv_mass;
        entities[idx].angInertia = ics.ang_inertia;
        entities[idx].invAngInertia = ics.inv_ang_inertia;
        if (outIdx) *outIdx = idx;

        return out;
    }

    void PhysEntityManager::addDuplicateEntity(f32 u0, f32 mass, phBound* bound, phBoundType type, const char* name, const vec3f& u2, undefined* u3, const vec3f& angInertia, i32* outIdx) {
        std::string hash = PhysEntityManager::genHashStr(u0, type, name, u2, { 0.0f, 0.0f, 0.0f });
        u32 i = 0;
        while (true) {
            auto& it = entityIdxMap.find(hash);
            if (it == entityIdxMap.end()) break;

            char nameBuf[64];
            snprintf(nameBuf, 64, "%s%i", name, i++);
            hash = PhysEntityManager::genHashStr(u0, type, nameBuf, u2, { 0.0f, 0.0f, 0.0f });
        }

        u32 idx = insert(bound, hash);
        entities[idx].unk1 = u3;
        entities[idx].unk0 = u0;
        entities[idx].mass = mass;
        entities[idx].invMass = 1.0f / mass;
        entities[idx].angInertia = angInertia;
        entities[idx].invAngInertia = {
            1.0f / angInertia.x,
            1.0f / angInertia.y,
            1.0f / angInertia.z
        };
        entities[idx].unk2 = u2;
        if (outIdx) *outIdx = idx;
    }
    
    u32 PhysEntityManager::insert(phBound* bound, const std::string& hash) {
        entities[usedCount].bound = bound;
        entityIdxMap[hash] = usedCount;
        return usedCount++;
    }

    PhysEntity* PhysEntityManager::getEntity(f32 u0, const char* name, i32 u1, i32* outIdx) {
        std::string hash = PhysEntityManager::genHashStr(u0, INVALID_BOUND, name, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });
        auto& it = entityIdxMap.find(hash);
        if (it != entityIdxMap.end()) {
            u32 idx = it->second;
            if (outIdx) *outIdx = idx;
            return &entities[idx];
        }

        phBound* bnd = phBound::load(name);
        u32 idx = insert(bnd, hash);
        initEntity(u0, idx, bnd, u1);
        if (bnd) {
            if (outIdx) *outIdx = idx;
            return &entities[idx];
        }

        return nullptr;
    }
    
    void PhysEntityManager::initEntity(f32 u0, u32 idx, phBound* bound, i32 u1) {
        if (!bound) {
            entities[idx].mass = 0.0f;
            entities[idx].invMass = 0.0f;
            entities[idx].angInertia = { 0.0f, 0.0f, 0.0f };
            entities[idx].invAngInertia = { 0.0f, 0.0f, 0.0f };
            entities[idx].unk0 = 0.0f;
            entities[idx].unk1 = nullptr;
            entities[idx].unk2 = { 0.0f, 0.0f, 0.0f };
            return;
        }

        vec3f unk2 = { 1.0f, 1.0f, 1.0f };

        if (!u1) {
            entities[idx].mass = 1.0f;
            entities[idx].invMass = 1.0f;
            entities[idx].angInertia = { 1.0f, 1.0f, 1.0f };
            entities[idx].invAngInertia = { 1.0f, 1.0f, 1.0f };
            entities[idx].unk0 = 1.0f;
            entities[idx].unk2 = unk2;
            return;
        }

        phInertialCS ics;
        switch (bound->type) {
            case BOUND_SPHERE: {
                ics.initSphereDensity(u0, bound->aabbRadius);
                unk2 = { bound->aabbRadius, bound->aabbRadius, bound->aabbRadius };
                break;
            }
            case BOUND_HOTDOG: {
                phBoundHotdog* hd = (phBoundHotdog*)bound;
                ics.initHotdogDensity(u0, hd->radius, hd->length, 'y');
                break;
            }
            case BOUND_BOX: {
                math::sub(unk2, bound->aabbMax, bound->aabbMin);
                ics.initBoxDensity(u0, unk2.x, unk2.y, unk2.z);
                break;
            }
            case BOUND_GEOMETRY: {
                phBoundGeometry* geo = (phBoundGeometry*)bound;
                ics.initFromGeometry(u0, geo->vertices, geo->polygons, geo->polyCount, nullptr);
                break;
            }
            default: break;
        }

        entities[idx].unk0 = u0;
        entities[idx].unk1 = nullptr;
        entities[idx].unk2 = unk2;
        entities[idx].mass = ics.mass;
        entities[idx].invMass = ics.inv_mass;
        entities[idx].angInertia = ics.ang_inertia;
        entities[idx].invAngInertia = ics.inv_ang_inertia;
    }

    PhysEntityManager* PhysEntityManager::create(u32 capacity) {
        if (instance) return instance;
        instance = new PhysEntityManager(capacity);
        return instance;
    }

    PhysEntityManager* PhysEntityManager::get(u32 capacity) {
        if (!instance) return PhysEntityManager::create(capacity);
        return instance;
    }

    void PhysEntityManager::destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }
    
    std::string PhysEntityManager::genHashStr(f32 u0, phBoundType type, const char* name, const vec3f& u3, const vec3f& u4) {
        char buf[128] = { 0 };

        snprintf(
            buf, 128,
            "%d %s %2.2f %2.2f %2.2f%2.2f %2.2f %2.2f %2.2f",
            type, name, u3.x, u3.y, u3.z, u0, u4.x, u4.y, u4.z
        );

        return buf;
    }
};