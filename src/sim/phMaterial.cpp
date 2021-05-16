#include <libsr2/sim/phMaterial.h>
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
};