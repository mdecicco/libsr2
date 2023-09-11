#include <libsr2/creature/ragdoll.h>
#include <libsr2/sim/dgLinkData.h>
#include <libsr2/math/math.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/quat.h>

#include <math.h>

namespace sr2 {
    /*
     * ragJoint
     */
    ragJoint::ragJoint() {
        unk0 = 1.0f;
        linkIdx = -1;
        parent = nullptr;
        unk1 = 0.0f;
        unk2 = 0.0f;
        boneIdx = -1;
    }

    ragJoint::~ragJoint() {
    }

    void ragJoint::reset(dgLinkData* data) {
        data->center.position = { 0.0f, 0.0f, 0.0f };
        data->velocity = { 0.0f, 0.0f, 0.0f };
        data->unk2.orient = data->rotated.orient;
        data->unk2.position = data->rotated.position;

        math::rotate(data->unk0->orient, data->init.position, data->rotated.position);
        math::add(data->rotated.position, data->unk0->position);
        math::mult(data->rotated.orient, data->unk0->orient, data->init.orient);
        data->unk2.orient = data->rotated.orient;
        data->unk2.position = data->rotated.position;
        data->unk5 = false;
    }

    void ragJoint::getTransform(mat3x4f& out) {
        out.w = position;
        if (!parent) return;

        mat3x4f t;
        parent->getTransform(t);

        math::mult_with_translation(out, t);
    }

    void ragJoint::method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4) {
        if (!parent) return;
        parent->method_0x50(links, param_2, param_3, param_4);
    }

    void ragJoint::method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4) {
        if (!parent) return;
        parent->method_0x58(links, param_2, param_3, param_4);
    }

    void ragJoint::method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2) {
        if (!parent) return;
        parent->method_0x20(links, param_1, param_2);
    }

    void ragJoint::method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2) {
        if (!parent) return;
        parent->method_0x28(links, param_1, param_2);
    }
    
    void ragJoint::FUN_00284238(f32 length, f32 inertia) {
        unk0 = 1.0f / (length * (inertia + 1.0f));
    }

    void ragJoint::resetSomething(dgLinkData* data) {
        math::zero(data->center.position);
        math::zero(data->velocity);
    }



    /*
     * ragElbowJoint
     */
    ragElbowJoint::ragElbowJoint() {
        setAngleLimit(0.5f);
        centerAngle = angleLimit;
    }

    ragElbowJoint::~ragElbowJoint() {
    }

    void ragElbowJoint::reset(dgLinkData* link) {
        link->center.orient.axis.x = initAngle;
        math::from_length_encoded(link->init.orient, {
            initAngle, 0.0f, 0.0f
        });

        ragJoint::reset(link);
    }

    void ragElbowJoint::method_0x18(f32 dt, f32 gravity, dgLinkData* links, const vec3f& vel, const vec3f& param_5, const vec3f& param_6) {
        if (unk1 > 0.0f) {
            if (parent) {
                vec3f dummy;
                parent->method_0x58(links, links[linkIdx].rotated.position, param_5, dummy);
            }

            vec3f dv;
            math::sub(dv, vel, links[linkIdx].velocity);
            dv.y -= gravity;

            links[linkIdx].velocity = vel;

            vec3f linkX;
            links[linkIdx].getXAxis(linkX);

            vec3f tmp;
            math::cross(tmp, dv, param_6);
            math::mult(tmp, unk0);
            math::mult(tmp, linkX);

            f32 cx = links[linkIdx].center.position.x;

            cx += (tmp.x + tmp.y + tmp.z) * math::min(unk1, 1.0f);
            cx += (math::dot(param_5, linkX) - cx) * unk2 * dt;
            cx = math::max(math::min(cx, 9.0f), -9.0f);

            links[linkIdx].center.position.x = cx;
            links[linkIdx].center.orient.axis.x += cx * dt + links[linkIdx].unk3.x;
            links[linkIdx].unk3.x = 0.0f;

            f32 fVar2 = links[linkIdx].center.orient.axis.x - centerAngle;

            if (-angleLimit <= fVar2) {
                if (fVar2 <= angleLimit) {
                    links[linkIdx].unk5 = false;
                } else {
                    links[linkIdx].unk5 = true;
                    links[linkIdx].center.orient.axis.x = centerAngle + angleLimit;
                    links[linkIdx].center.position.x = math::min(links[linkIdx].center.position.x, 0.0f);
                }
            } else {
                links[linkIdx].unk5 = true;
                links[linkIdx].center.orient.axis.x = centerAngle - angleLimit;
                links[linkIdx].center.position.x = math::max(links[linkIdx].center.position.x, 0.0f);
            }

            math::from_length_encoded(links[linkIdx].init.orient, {
                links[linkIdx].center.orient.axis.x,
                0.0f,
                0.0f
            });
        }

        links[linkIdx].unk2 = links[linkIdx].rotated;

        math::rotate(links[linkIdx].unk0->orient, links[linkIdx].init.position, links[linkIdx].rotated.position);
        math::add(links[linkIdx].rotated.position, links[linkIdx].unk0->position);
        math::mult(links[linkIdx].rotated.orient, links[linkIdx].unk0->orient, links[linkIdx].init.orient);
    }
    
    void ragElbowJoint::setAngleLimit(f32 limit) {
        angleLimit = limit;
        initAngle = limit * 0.1f;
    }
    
    bool ragElbowJoint::method_0x38(f32 unk0, f32 unk1, dgLinkData* link) {
        if (fabsf(link->center.orient.axis.x - initAngle) <= 0.1f) return false;

        link->center.position.x += math::findHomingAccel(
            unk0,
            link->center.orient.axis.x,
            link->center.position.x,
            initAngle,
            0.0f
        ) * unk1;

        return true;
    }
    
    void ragElbowJoint::getTransform(mat3x4f& out) {
        math::rotation_axis_angle(out, { 1.0f, 0.0f, 0.0f }, initAngle);
        ragJoint::getTransform(out);
    }
    
    void ragElbowJoint::method_0x48(quat_transform& out) {
        math::from_axis_angle(out.orient, { 1.0f, 0.0f, 0.0f }, initAngle);
        math::copy(out.position, position);
    }
    
    void ragElbowJoint::method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4) {
        dgLinkData& link = links[linkIdx];
        if (unk1 <= 0.0f) {
            ragJoint::method_0x50(links, link.rotated.position, param_3, param_4);
            return;
        }

        vec3f linkX;
        link.getXAxis(linkX);

        bool somethingInvolvingSigns = !link.unk5;
        if (link.unk5) {
            vec3f tmp;
            math::sub(tmp, param_2, link.rotated.position);
            math::cross(tmp, tmp, param_4);

            f32 tmpDotX = math::dot(tmp, linkX);
            f32 angleDiff = link.center.orient.axis.x - centerAngle;

            // Same thing as signbit(tmpDotX) != signBit(angleDiff) but with fewer instructions            
            somethingInvolvingSigns = (reinterpret_cast<u32&>(tmpDotX) ^ reinterpret_cast<u32&>(angleDiff)) >> 0x1f;
        }

        if (somethingInvolvingSigns) {
            vec3f tmp0;
            math::sub(tmp0, param_2, link.rotated.position);

            vec3f tmp1;
            math::cross(tmp1, linkX, tmp0);

            vec3f tmp2;
            math::mult(tmp2, tmp1, unk1);

            vec3f tmp3;
            math::mult(tmp3, tmp1, tmp2.x); math::add(param_3.x, tmp3);
            math::mult(tmp3, tmp1, tmp2.y); math::add(param_3.y, tmp3);
            math::mult(tmp3, tmp1, tmp2.z); math::add(param_3.z, tmp3);
        }
        
        ragJoint::method_0x50(links, link.rotated.position, param_3, param_4);
    }
    
    void ragElbowJoint::method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4) { 
        if (unk1 > 0.0f && links[linkIdx].center.position.x != 0.0f) {
            vec3f dp;
            math::sub(dp, param_2, links[linkIdx].rotated.position);

            vec3f& c = links[linkIdx].center.position;
            vec3f linkX;
            links[linkIdx].getXAxis(linkX);

            vec3f tmp0;
            math::mult(tmp0, linkX, c.x);

            vec3f tmp1;
            math::cross(tmp1, tmp0, dp);
            math::add(param_4, tmp1);
        }

        ragJoint::method_0x58(links, param_2, param_3, param_4);
    }

    void ragElbowJoint::method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2) {
        if (unk1 <= 0.0f) {
            ragJoint::method_0x20(links, param_1, links[linkIdx].rotated.position);
            return;
        }

        vec3f dp;
        math::sub(dp, param_2, links[linkIdx].rotated.position);

        vec3f linkX;
        links[linkIdx].getXAxis(linkX);

        vec3f tmp0;
        math::cross(tmp0, dp, param_1);

        f32 unk = math::dot(tmp0, linkX);
        f32 da = links[linkIdx].center.orient.axis.x - centerAngle;

        // Who knows what on earth this is doing
        bool unkCond = i32(reinterpret_cast<u32&>(unk) ^ reinterpret_cast<u32&>(da)) < 0;
        if (!links[linkIdx].unk5 || unkCond) {
            links[linkIdx].center.position.x += unk * unk1;
        }

        ragJoint::method_0x20(links, param_1, links[linkIdx].rotated.position);
    }
    
    void ragElbowJoint::method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2) {
        if (unk1 <= 0.0f) {
            ragJoint::method_0x28(links, param_1, param_2);
            return;
        }     
        
        f32 p1MagSq = math::magnitudeSq(param_1);
        if (0.0 < p1MagSq) {
            vec3f tmp0;
            math::sub(tmp0, param_2, links[linkIdx].rotated.position);

            f32 t0MagSq = math::magnitudeSq(tmp0);
            if (0.0 < t0MagSq) {
                vec3f tmp1;
                math::cross(tmp1, tmp0, param_1);

                f32 t1MagSq = math::magnitudeSq(tmp1);
                if (t1MagSq > p1MagSq * t0MagSq * 0.03f) {
                    vec3f linkX;
                    links[linkIdx].getXAxis(linkX);

                    f32 u0 = math::dot(param_1, linkX);
                    f32 u1 = math::dot(tmp1, linkX);
                    f32 da = links[linkIdx].center.orient.axis.x - centerAngle;

                    // Who knows what on earth this is doing
                    bool unkCond = i32(reinterpret_cast<u32&>(da) ^ reinterpret_cast<u32&>(u1)) < 0;
                    if ((p1MagSq * 0.03 < u0 * u0) && (!links[linkIdx].unk5 || unkCond)) {
                        vec3f tmp2;
                        math::cross(tmp2, linkX, tmp0);
                        f32 u2 = math::dot(tmp2, param_1);
                        
                        if (u2 != 0.0f) {
                            f32 u3 = math::max(math::min(p1MagSq / u2, 0.1884956f), -0.1884956f);
                            FUN_00285810(&links[linkIdx], u3);

                            vec3f tmp3;
                            math::mult(tmp3, linkX, u3);

                            vec3f tmp4;
                            math::cross(tmp4, tmp3, tmp0);

                            math::mult(param_1, 1.0f - math::dot(tmp4, param_1) / p1MagSq);

                            vec3f tmp5;
                            math::add(tmp5, param_2, tmp4);
                            ragJoint::method_0x28(links, param_1, tmp5);
                            return;
                        }
                    }
                }
            }
        }
        
        ragJoint::method_0x28(links, param_1, param_2);
    }
    
    void ragElbowJoint::FUN_00285810(dgLinkData* link, f32 param_1) {
        bool unkCond = i32(reinterpret_cast<u32&>(link->unk3.x) ^ reinterpret_cast<u32&>(param_1)) < 0;
        if (link->unk3.x == 0.0f || unkCond) {
            link->unk3.x += param_1;
        } else if (fabsf(link->unk3.x) < fabsf(param_1)) {
            link->unk3.x = param_1;
        }
    }



    /*
     * ragBallJoint
     */
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



    /*
     * ragUnk0
     */
    ragUnk0::ragUnk0() {
        links = nullptr;
        boneLinkIndices = nullptr;
        unk0 = 0.0f;
        unk1 = 0.0f;
        count = 0;
    }

    ragUnk0::~ragUnk0() {
        if (links) {
            for (u32 i = 0;i < count;i++) delete links[i];
            delete [] links;
            links = nullptr;
        }

        if (boneLinkIndices) {
            delete [] boneLinkIndices;
            boneLinkIndices = nullptr;
        }
    }
    
    void ragUnk0::init(i32 linkCount, i32 boneCount) {
        count = linkCount;
        links = new ragJoint*[linkCount];
        for (u32 i = 0;i < linkCount;i++) links[i] = nullptr;

        boneLinkIndices = new i32[boneCount];
        for (u32 i = 0;i < boneCount;i++) boneLinkIndices[i] = -1;

        unk0 = 1.0f;
        unk1 = 1.0f;
    }

    void ragUnk0::initBallJoint(f32 angleLimit, f32 length, f32 inertia, f32 unk1, f32 unk2, i32 linkIdx, i32 boundIdx, const vec3f& centerOrient, const vec3f &initOrient, const vec3f& pos, i32 parentIdx) {
        ragBallJoint* joint = new ragBallJoint();
        links[linkIdx] = joint;
        boneLinkIndices[boundIdx] = linkIdx;
        joint->boneIdx = boundIdx;
        joint->linkIdx = linkIdx;
        
        math::from_length_encoded(joint->centerOrient, centerOrient);
        math::from_length_encoded(joint->initOrient, initOrient);

        FUN_00282b78(angleLimit, length, inertia, unk1, unk2, joint, pos, parentIdx);
    }

    void ragUnk0::initElbowJoint(f32 angleLimit, f32 centerAngle, f32 initAngle, f32 length, f32 inertia, f32 unk1, f32 unk2, i32 linkIdx, i32 boundIdx, const vec3f& pos, i32 parentIdx) {
        ragElbowJoint* joint = new ragElbowJoint();
        links[linkIdx] = joint;
        boneLinkIndices[boundIdx] = linkIdx;
        joint->boneIdx = boundIdx;
        joint->linkIdx = linkIdx;
        joint->centerAngle = centerAngle;
        joint->initAngle = initAngle;

        FUN_00282b78(angleLimit, length, inertia, unk1, unk2, joint, pos, parentIdx);
    }

    void ragUnk0::FUN_00282b78(f32 angleLimit, f32 length, f32 inertia, f32 unk1, f32 unk2, ragJoint* joint, const vec3f& pos, i32 parentIdx) {
        joint->setAngleLimit(angleLimit);
        joint->FUN_00284238(length, inertia);
        joint->position = pos;
        joint->unk1 = unk1;
        joint->unk2 = unk2;
        if (parentIdx >= 0) joint->parent = links[parentIdx];
    }
};