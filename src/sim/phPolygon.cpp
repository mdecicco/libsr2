#include <libsr2/sim/phPolygon.h>
#include <libsr2/math/vec3f.h>

namespace sr2 {
    phPolygon::phPolygon() {
    }
    
    phPolygon::~phPolygon() {
    }

    void phPolygon::calculateNormal(vec3f* vertices) {
        u8* magPtr = (u8*)&normalMag;
        u8 initialFirstByte = *magPtr;

        vec3f& v0 = vertices[indices[0]];
        vec3f& v1 = vertices[indices[1]];
        vec3f& v2 = vertices[indices[2]];

        vec3f d21, d01;
        math::sub(d21, v2, v1);
        math::sub(d01, v0, v1);
        math::cross(normal, d21, d01);
        normalMag = math::magnitude(normal);
        if (normalMag != 0.0f) {
            math::mult(normal, 1.0f / normalMag);
            normalMag *= 0.5f;
        }

        if (indices[3] != 0) {
            vec3f& v3 = vertices[indices[3]];
            vec3f d03, d23;
            math::sub(d03, v0, v3);
            math::sub(d23, v2, v3);

            vec3f n;
            math::cross(n, d03, d23);
            
            // ???
            normalMag += math::magnitude(n) * 0.5f;
        }

        // I _assume_ that what the code intends to do is
        // preserve whatever the sign was... And this is
        // how it did it (exactly), but I don't think that
        // is the actual behavior that this will have unless
        // the first 7 exponent bits would remain unchanged
        // after the above code is executed.
        *magPtr = initialFirstByte;
    }
};