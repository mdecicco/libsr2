#pragma once
#include <libsr2/sim/vehBound.h>
#include <libsr2/sim/lvlMaterial.h>

namespace sr2 {
    vehBound::vehBound() : phBoundGeometry(BOUND_GEOMETRY) {
        material = new lvlMaterial();
        penetration = 0.0f;
        field_0x9c = nullptr;
    }

    vehBound::~vehBound() {
        delete material;
        if (field_0x9c) {
            delete [] field_0x9c;
            field_0x9c = nullptr;
        }
    }
};

