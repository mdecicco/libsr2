#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundType.h>

#include <unordered_map>
#include <string>

namespace sr2 {
    class PhysEntity;
    class phBound;

    class PhysEntityManager {
        public:
            PhysEntityManager(u32 capacity);
            ~PhysEntityManager();

            void init(u32 capacity);
            void deinit();
            void addEntity(f32 u0, f32 mass, phBound* bound, phBoundType type, const char* name, const vec3f& u2, undefined* u3, const vec3f& angInertia, i32* outIdx);
            phBound* addEntity(f32 u0, phBoundType type, const char* name, const vec3f& u1, undefined* u2, i32* outIdx, const vec3f& center, bool u5);
            void addDuplicateEntity(f32 u0, f32 mass, phBound *bound, phBoundType type, const char *name, const vec3f& u2, undefined* u3, const vec3f& angInertia, i32* outIdx);
            u32 insert(phBound* bound, const std::string& hash);
            PhysEntity* getEntity(f32 u0, const char* name, i32 u1, i32* outIdx);
            void initEntity(f32 u0, u32 idx, phBound* bound, i32 u1);

            PhysEntity* entities;
            u32 freeCount;
            u32 usedCount;
            u32 capacity;
            f32 unk0;
            std::unordered_map<std::string, u32> entityIdxMap;

            static PhysEntityManager* create(u32 capacity);
            static PhysEntityManager* get(u32 capacity = 200);
            static void destroy();
            static std::string genHashStr(f32 u0, phBoundType type, const char* name, const vec3f& u3, const vec3f& u4);
        
        private:
            static PhysEntityManager* instance;
    };
};