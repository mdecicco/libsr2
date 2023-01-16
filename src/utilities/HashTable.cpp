#include <libsr2/utilities/HashTable.h>

namespace sr2 {
    HashTable::HashTable() {
    }

    HashTable::~HashTable() {
    }

    void* HashTable::Access(const char* key) {
        auto it = m_map.find(key);
        if (it != m_map.end()) return it->second;
        return nullptr;
    }

    void HashTable::Insert(const char* key, void* value) {
        m_map[key] = value;
    }

    void HashTable::Delete(const char* key) {
        m_map.erase(key);
    }

    void HashTable::MakePermanent() {
        // who knows
    }
};