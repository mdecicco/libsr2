#include <libsr2/creature/ragBallJoint.h>
#include <libsr2/creature/ragJoint.h>
#include <libsr2/sim/dgLinkData.h>

#include <libsr2/math/math.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/quat.h>

#include <math.h>

namespace sr2 {
        ragBallJoint::ragBallJoint() {
        math::zero(centerOrient.axis);
        centerOrient.angle = 1.0f;
        math::zero(initOrient.axis);
        initOrient.angle = 1.0f;

        setAngleLimit(0.5f);
    }

    ragBallJoint::~ragBallJoint() {
    }

    void ragBallJoint::reset(dgLinkData* data) {
        data->init.orient = initOrient;
        data->center.orient = centerOrient;

        math::mult(data->center.orient, initOrient);
        ragJoint::reset(data);
    }

    void ragBallJoint::getTransform(mat3x4f& out) {
        math::from_quaternion(out, initOrient);
        ragJoint::getTransform(out);
    }

    void ragBallJoint::setAngleLimit(f32 limit) {
        cosHalfAngleLimit = cosf(limit * 0.5f);

        // Apparently this is cheaper than sinf(limit * 0.5f)...
        sinHalfAngleLimit = sqrtf(1.0f - cosHalfAngleLimit * cosHalfAngleLimit);
        if (limit < 0.0f) sinHalfAngleLimit = -sinHalfAngleLimit;
    }

    void ragBallJoint::method_0x18(f32 dt, f32 gravity, dgLinkData* links, const vec3f& velocity, const vec3f& u0, const vec3f& u1) {
        dgLinkData* link = &links[linkIdx];
        if (unk1 <= 0.0f) {
            link->unk2 = link->rotated;
            math::rotate(link->unk0->orient, link->init.position, link->rotated.position);
            math::add(link->rotated.position, link->unk0->position);
            math::mult(link->rotated.orient, link->unk0->orient, link->init.orient);
            return;
        }

        vec3f vel = velocity;
        if (parent) {
            parent->method_0x58(links, link->rotated.position, u0, vel);
        }

        vec3f t0;
        math::sub(t0, vel, link->velocity);
        link->velocity = vel;
        t0.y -= gravity;

        vec3f t1;
        math::cross(t1, t0, u1);
        math::mult(t1, unk0 * math::min(unk1, 1.0f));
        math::add(link->center.position, t1);

        vec3f t2;
        math::sub(t2, u0, link->center.position);
        math::mult(t2, unk2 * dt);
        math::add(link->center.position, t2);

        link->center.position = {
            math::clamp(link->center.position.x, -9.0f, 9.0f),
            math::clamp(link->center.position.y, -9.0f, 9.0f),
            math::clamp(link->center.position.z, -9.0f, 9.0f)
        };

        vec3f t3;
        math::mult(t3, link->center.position, dt);
        math::FUN_0031b2f8(link->unk3, t3);
        link->unk2 = link->rotated;

        math::unrotate(link->rotated.orient, link->unk3, t3);

        quat t4;
        math::from_length_encoded(t4, t3);
        math::mult(link->rotated.orient, t4);

        math::zero(link->unk3);

        vec3f t5;
        math::rotate(link->unk0->orient, link->init.position, t5);
        math::add(link->rotated.position, link->unk0->position);

        link->init.orient = link->unk0->orient;
        math::negate(link->init.orient.axis);
        
        if (math::dot(link->unk0->orient, link->unk1->orient) < 0.0f) {
            math::negate(link->rotated.orient);
            math::negate(link->unk2.orient);
        }

        math::mult(link->init.orient, link->rotated.orient);
        link->center.orient = centerOrient;
        math::negate(link->center.orient.axis);

        math::mult(link->center.orient, link->init.orient);

        link->unk5 = false;
        if (fabsf(link->center.orient.angle) <= cosHalfAngleLimit) {
            link->unk5 = true;
            vec3f t6 = link->center.orient.axis;
            if (link->center.orient.angle < 0.0f) math::negate(t6);

            math::normalize(t6);
            math::rotate(centerOrient, t6, t2);

            vec3f t7;
            math::rotate(link->unk0->orient, t2, t7);
            
            f32 t8 = math::dot(link->center.position, t7);
            if (t8 > 0.0f) {
                vec3f t9;
                math::mult(t9, t7, t8);
                math::sub(link->center.position, t9);
            }

            math::mult(link->center.orient.axis, t6, sinHalfAngleLimit);
            link->center.orient.angle = cosHalfAngleLimit;
            
            math::mult(link->init.orient, centerOrient, link->center.orient);
            math::mult(link->rotated.orient, link->unk0->orient, link->init.orient);
        }
    }

    bool ragBallJoint::method_0x38(f32 u0, f32 u1, dgLinkData* link) {
        bool b = fabsf(math::dot(link->init.orient, initOrient)) < cosf(0.05f);
        if (b) {
            vec3f t0, t1, t2;
            math::unrotate(link->unk0->orient, link->center.position, t0);
            math::unrotate(link->init.orient, t0, t1);

            math::FUN_002c3720(u0, 1.0f / u1, t2, link->init.orient, t1, initOrient);

            math::rotate(link->init.orient, t2, t1);
            math::rotate(link->unk0->orient, t1, t0);

            vec3f t3;
            math::mult(t0, u1);
            math::add(link->center.position, t3);
        }

        return b;
    }

    void ragBallJoint::method_0x48(quat_transform& out) {
        out.orient = initOrient;
        out.position = position;
    }

    void ragBallJoint::method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4) {
        dgLinkData* link = &links[linkIdx];
        if (unk1 <= 0.0f) {
            ragJoint::method_0x50(links, link->rotated.position, param_3, param_4);
            return;
        }

        bool bVar1 = true;
        if (link->unk5) {
            vec3f t0;
            math::sub(t0, param_2, link->rotated.position);
            
            vec3f t1;
            math::cross(t1, t0, param_4);

            vec3f t2;
            math::unrotate(link->unk0->orient, t1, t2);
            math::unrotate(centerOrient, t2, t1);

            vec3f t3 = link->center.orient.axis;
            math::normalize(t3);

            f32 t4 = math::dot(t1, t3);
            if (t4 > 0.0f) {
                vec3f t5;
                math::mult(t3, t4);
                math::sub(t1, t3);

                math::rotate(centerOrient, t1, t2);
                math::rotate(link->unk0->orient, t2, t1);

                vec3f t6;
                math::sub(t6, param_2, link->rotated.position);
                
                vec3f t7;
                math::cross(t7, t1, t6);

                vec3f t8;
                math::mult(t8, t7, t7.x * unk1);
                math::add(param_3.x, t8);

                math::mult(t8, t7, t7.y * unk1);
                math::add(param_3.y, t8);

                math::mult(t8, t7, t7.z * unk1);
                math::add(param_3.z, t8);

                bVar1 = false;
            }
        }
        
        if (bVar1) {
            vec3f t9;
            math::sub(t9, param_2, link->rotated.position);

            vec3f t10;
            math::mult(t10, t9, unk1);
            math::mult(t10, t9);
            
            param_3.x.x += t10.y + t10.z;
            param_3.x.y -= (unk1 * t9.x * t9.y);
            param_3.x.z -= (unk1 * t9.x * t9.z);

            param_3.y.x -= (unk1 * t9.x * t9.y);
            param_3.y.y += t10.x + t10.z;
            param_3.y.z -= (unk1 * t9.y * t9.z);
            
            param_3.z.x -= (unk1 * t9.x * t9.z);
            param_3.z.y -= (unk1 * t9.y * t9.z);
            param_3.z.z += t10.x + t10.y;
        }
        
        ragJoint::method_0x50(links, link->rotated.position, param_3, param_4);
    }

    void ragBallJoint::method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4) {
        if (unk1 <= 0.0f) {
            ragJoint::method_0x58(links, param_2, param_3, param_4);
            return;
        }

        vec3f t0;
        math::sub(t0, links[linkIdx].center.position, param_3);

        if (t0.x != 0.0f || t0.y != 0.0f || t0.z != 0.0f) {
            vec3f t1;
            math::sub(t1, param_2, links[linkIdx].rotated.position);
            math::cross(t1, t1, t0);
            math::sub(param_4, t1);
        }

        ragJoint::method_0x58(links, param_2, param_3, param_4);
    }

    void ragBallJoint::method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2) {
        if (unk1 <= 0.0f) {
            ragJoint::method_0x20(links, param_2, links[linkIdx].rotated.position);
            return;
        }

        vec3f t0;
        math::sub(t0, param_2, links[linkIdx].rotated.position);
        
        vec3f t1;
        math::cross(t1, t0, param_1);
        math::mult(t1, unk1);
        math::add(links[linkIdx].center.position, t1);

        ragJoint::method_0x20(links, param_2, links[linkIdx].rotated.position);
    }

    void ragBallJoint::method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2) {
        if (unk1 <= 0.0f) {
            ragJoint::method_0x28(links, param_1, param_2);
            return;
        }

        dgLinkData* link = &links[linkIdx];

        f32 magSq = math::magnitudeSq(param_1);
        if (magSq <= 0.0f) {
            ragJoint::method_0x28(links, param_1, param_2);
            return;
        }

        vec3f t0;
        math::sub(t0, param_2, link->rotated.position);

        f32 t0MagSq = math::magnitudeSq(t0);
        if (t0MagSq <= 0.0f) {
            ragJoint::method_0x28(links, param_1, param_2);
            return;
        }

        vec3f t1;
        math::cross(t1, t0, param_1);
        f32 t1MagSq = math::magnitudeSq(t1);
        if (magSq * t0MagSq * 0.03f >= t1MagSq) {
            ragJoint::method_0x28(links, param_1, param_2);
            return;
        }

        if (link->unk5) {
            vec3f t2, t3;
            math::unrotate(link->unk0->orient, t1, t2);
            math::unrotate(centerOrient, t2, t3);
            vec3f t4 = link->center.orient.axis;
            math::normalize(t4);

            f32 t5 = math::dot(t3, t4);
            if (t5 > 0.0f) {
                vec3f t6;
                math::mult(t6, t4, t5);
                math::sub(t6, t3, t6);
                math::rotate(centerOrient, t6, t2);
                math::rotate(link->unk0->orient, t2, t1);
                t1MagSq = math::magnitudeSq(t1);
            }
        }

        vec3f t7;
        math::cross(t7, t1, t0);
        f32 t8 = math::dot(t7, param_1);
        if (t8 <= 0.0f) {
            ragJoint::method_0x28(links, param_1, param_2);
            return;
        }

        f32 t9 = math::min(magSq * sqrtf(math::magnitudeSq(t7) / (t0MagSq * t8 * t8)), 0.1884956f) / sqrtf(t1MagSq);
        vec3f t10;
        math::mult(t10, t1, t9);

        math::FUN_0031b2f8(link->unk3, t10);

        vec3f t11;
        math::cross(t11, t10, t0);

        f32 t12 = 1.0f - math::dot(t11, param_1) / magSq;
        math::mult(param_1, t12);

        vec3f t13;
        math::add(t13, param_2, t11);
        ragJoint::method_0x28(links, param_1, t13);
    }
};