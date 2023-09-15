#include <libsr2/sim/phMaterial.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/managers/phMaterialMgr.h>

#include <string.h>

namespace sr2 {
    phMaterial::phMaterial() {
        elasticity = 0.1f;
        friction = 0.5f;
        sound = -1;
        effect = -1;
        strcpy(name, "default");
    }

    phMaterial::~phMaterial() {
    }

    void phMaterial::parse(datAsciiTokenizer& tok) {
        tok.matchToken("mtl");
        tok.getToken(name, 32);
        if (tok.checkToken("{", false)) {
            tok.matchToken("{");
            tok.matchToken("elasticity:");
            elasticity = tok.readFloat();
            tok.matchToken("friction:");
            friction = tok.readFloat();
            tok.matchToken("effect:");
            effect = tok.readInt32();
            tok.matchToken("sound:");
            sound = tok.readInt32();
            char buf[256];
            while (!tok.checkIntToken("}", true)) tok.getToken(buf, 256);
        } else {
            // DAT_0036cb34 = 1
            phMaterialMgr* m = phMaterialMgr::get();
            if (!m) m = phMaterialMgr::create(100);

            phMaterial* mtrl = m->find(name);
            if (mtrl) copy(mtrl);
        }
    }

    void phMaterial::setName(const char* _name) {
        strncpy(name, _name, 32);
    }
    
    void phMaterial::copy(phMaterial* other) {
        elasticity = other->elasticity;
        friction = other->friction;
        effect = other->effect;
        sound = other->sound;
    }
};