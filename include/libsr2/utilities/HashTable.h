#pragma once
#include <unordered_map>
#include <string>

namespace sr2 {
    class HashTable {
        public:
            HashTable();
            ~HashTable();

            void* Access(const char* key);
            void Insert(const char* key, void* value);
            void Delete(const char* key);
            void MakePermanent();

        protected:
            // Original implementation is custom made and probably
            // more efficient. I'll just use an unordered_map until
            // something requires the original implementation
            std::unordered_map<std::string, void*> m_map;
    };
};

