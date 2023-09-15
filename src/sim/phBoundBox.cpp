#include <libsr2/sim/phBoundBox.h>
#include <libsr2/math/vec3f.h>

namespace sr2 {
    vec3f phBoundBox::unitCorners[8] = {
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f }
    };
    vec3f phBoundBox::faceNormals[6] = {
        {  1.0f,  0.0f,  0.0f },
        { -1.0f,  0.0f,  0.0f },
        {  0.0f,  1.0f,  0.0f },
        {  0.0f, -1.0f,  0.0f },
        {  0.0f,  0.0f,  1.0f },
        {  0.0f,  0.0f, -1.0f }
    };
    vec3f phBoundBox::edgeNormals[12] = {
        {  0.0f       ,  0.70710677f,  0.70710677f },
        { -0.70710677f,  0.0f       ,  0.70710677f },
        {  0.0f       , -0.70710677f,  0.70710677f },
        {  0.70710677f,  0.0f       ,  0.70710677f },
        {  0.0f       ,  0.70710677f, -0.70710677f },
        { -0.70710677f,  0.0f       , -0.70710677f },
        {  0.0f       , -0.70710677f, -0.70710677f },
        {  0.70710677f,  0.0f       , -0.70710677f },
        {  0.70710677f,  0.70710677f,  0.0f        },
        { -0.70710677f,  0.70710677f,  0.0f        },
        { -0.70710677f, -0.70710677f,  0.0f        },
        {  0.70710677f, -0.70710677f,  0.0f        }
    };
    vec3f phBoundBox::cornerNormals[8] = {
        {  1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        { -1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f }
    };
    u16 phBoundBox::boxEdges[24] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7
    };

    phBoundBox::phBoundBox() : phBoundPolygonal(BOUND_BOX) {
    }

    phBoundBox::~phBoundBox() {
    }

    void phBoundBox::setSize(const vec3f& size) {
        dimensions = size;
        for (u32 i = 0;i < 8;i++) {
            math::mult(verts[i], phBoundBox::unitCorners[i], size);
            math::add(verts[i], centroid);
        }

        for (u32 i = 0;i < 6;i++) {
            sides[i].calculateNormal(verts);
        }

        calculateBoundingBox();
    }
};