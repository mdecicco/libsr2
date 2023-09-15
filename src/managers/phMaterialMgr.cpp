#include <libsr2/managers/phMaterialMgr.h>
#include <libsr2/sim/phMaterial.h>
#include <libsr2/utilities/HashTable.h>

namespace sr2 {
    phMaterialMgr* phMaterialMgr::instance = nullptr;
    phMaterial* phMaterialMgr::defaultMaterial = nullptr;

    phMaterialMgr* phMaterialMgr::create(u32 materialCapacity) {
        if (phMaterialMgr::instance) return phMaterialMgr::instance;

        phMaterialMgr::defaultMaterial = new phMaterial();
        phMaterialMgr::instance = new phMaterialMgr(materialCapacity);
        return phMaterialMgr::instance;
    }

    phMaterialMgr* phMaterialMgr::get() {
        return phMaterialMgr::instance;
    }
    
    phMaterial* phMaterialMgr::getDefaultMaterial() {
        return phMaterialMgr::defaultMaterial;
    }

    phMaterialMgr::phMaterialMgr(u32 materialCapacity) {
        m_effectCount = 0;
        m_materialCount = 0;
        m_materialCapacity = materialCapacity;

        m_effects = new char[materialCapacity][32];
        memset(m_effects, 0, 32 * materialCapacity);

        m_materials = new phMaterial*[materialCapacity];
        memset(m_materials, 0, sizeof(void*) * materialCapacity);

        m_table = new HashTable();
        addToTable(phMaterialMgr::defaultMaterial);
        m_materials[m_materialCount++] = phMaterialMgr::defaultMaterial;
    }

    phMaterialMgr::~phMaterialMgr() {
        delete [] m_effects;
        m_effects = nullptr;

        delete [] m_materials;
        m_materials = nullptr;

        delete m_table;
        m_table = nullptr;
    }
    
    phMaterial* phMaterialMgr::find(const char* name) {
        return (phMaterial*)m_table->Access(name);
    }

    void phMaterialMgr::addToTable(phMaterial* mtrl) {
        m_table->Insert(mtrl->name, mtrl);
    }

    phMaterial* phMaterialMgr::addMaterial(datAsciiTokenizer& tok) {
        phMaterial mtrl;
        mtrl.parse(tok);
        phMaterial* out = find(mtrl.name);
        if (!out) {
            out = new phMaterial();
            out->setName(mtrl.name);
            out->copy(&mtrl);
            addToTable(out);
            m_materials[m_materialCount++] = out;
        }

        return out;
    }

    phMaterial* phMaterialMgr::addMaterial(Stream* file) {
        return nullptr;
    }
};