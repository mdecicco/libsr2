#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class HashTable;
    class phMaterial;
    class datAsciiTokenizer;
    class Stream;
    class phMaterialMgr {
        public:
            static phMaterialMgr* create(u32 materialCapacity);
            static phMaterialMgr* get(); 
            static phMaterial* getDefaultMaterial();

            phMaterial* find(const char* name);
            void addToTable(phMaterial* mtrl);

            virtual phMaterial* addMaterial(datAsciiTokenizer& file);
            virtual phMaterial* addMaterial(Stream* file);

        protected:
            static phMaterialMgr* instance;
            static phMaterial* defaultMaterial;

            phMaterialMgr(u32 materialCapacity);
            virtual ~phMaterialMgr();

            HashTable* m_table;
            phMaterial** m_materials;
            u32 m_effectCount;
            u32 m_materialCount;
            u32 m_materialCapacity;
            char (*m_effects)[32];
    };
};