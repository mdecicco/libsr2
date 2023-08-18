#include <libsr2/creature/ragdoll.h>
#include <libsr2/sim/dgLinkData.h>
#include <libsr2/math/math.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/quat.h>

#include <math.h>

namespace sr2 {
    /*
     * ragUnk2
     */
    ragUnk2::ragUnk2() {
    }

    ragUnk2::~ragUnk2() {
    }

    void ragUnk2::reset() {
    }

    void ragUnk2::method_0x18(f32 dt, f32 gravity, dgLinkData* links, const vec3f& velocity, const vec3f& unk, const vec3f& unk1) {
    }

    void ragUnk2::method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2) {
    }

    void ragUnk2::method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2) {
    }

    void ragUnk2::setAngleLimit(f32 limit) {
    }

    bool ragUnk2::method_0x38() {
        return false;
    }

    void ragUnk2::getTransform(mat3x4f& out) {
    }

    void ragUnk2::method_0x48(quat_transform& out) {
    }

    void ragUnk2::method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4) {
    }

    void ragUnk2::method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4) {
    }



    /*
     * ragLink
     */
    ragLink::ragLink() {
        setAngleLimit(0.5f);
        centerAngle = angleLimit;
    }

    ragLink::~ragLink() {
    }

    void ragLink::reset(dgLinkData* link) {
        link->center.orient.axis.x = initAngle;
        math::from_length_encoded(link->init.orient, {
            initAngle, 0.0f, 0.0f
        });

        ragUnk2::reset();
    }

    void ragLink::method_0x18(f32 dt, f32 gravity, dgLinkData* links, const vec3f& vel, const vec3f& param_5, const vec3f& param_6) {
        ragUnk2 *parent;
        
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
    
    void ragLink::method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2) {
        if (unk1 <= 0.0f) {
            ragUnk2::method_0x20(links, param_1, links[linkIdx].rotated.position);
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

        ragUnk2::method_0x20(links, param_1, links[linkIdx].rotated.position);
    }
    
    void ragLink::method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2) {
        if (unk1 <= 0.0f) {
            ragUnk2::method_0x28(links, param_1, param_2);
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
                            FUN_00285810(u3, &links[linkIdx]);

                            vec3f tmp3;
                            math::mult(tmp3, linkX, u3);

                            vec3f tmp4;
                            math::cross(tmp4, tmp3, tmp0);

                            math::mult(param_1, 1.0f - math::dot(tmp4, param_1) / p1MagSq);

                            vec3f tmp5;
                            math::add(tmp5, param_2, tmp4);
                            ragUnk2::method_0x28(links, param_1, tmp5);
                            return;
                        }
                    }
                }
            }
        }
        
        ragUnk2::method_0x28(links, param_1, param_2);
    }
    
    void ragLink::setAngleLimit(f32 limit) {
        angleLimit = limit;
        initAngle = limit * 0.1f;
    }
    
    bool ragLink::method_0x38(f32 unk0, f32 unk1, dgLinkData* link) {
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
    
    void ragLink::getTransform(mat3x4f& out) {
        math::rotation_axis_angle(out, { 1.0f, 0.0f, 0.0f }, initAngle);
        ragUnk2::getTransform(out);
    }
    
    void ragLink::method_0x48(quat_transform& out) {
        math::from_axis_angle(out.orient, { 1.0f, 0.0f, 0.0f }, initAngle);
        math::copy(out.position, position);
    }
    
    void ragLink::method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4) {
        dgLinkData& link = links[linkIdx];
        if (unk1 <= 0.0f) {
            ragUnk2::method_0x50(links, link.rotated.position, param_3, param_4);
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
        
        ragUnk2::method_0x50(links, link.rotated.position, param_3, param_4);
    }
    
    void ragLink::method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4) { 
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

        ragUnk2::method_0x58(links, param_2, param_3, param_4);
    }

    void ragLink::FUN_00285810(dgLinkData* link, f32 param_1) {
        bool unkCond = i32(reinterpret_cast<u32&>(link->unk3.x) ^ reinterpret_cast<u32&>(param_1)) < 0;
        if (link->unk3.x == 0.0f || unkCond) {
            link->unk3.x += param_1;
        } else if (fabsf(link->unk3.x) < fabsf(param_1)) {
            link->unk3.x = param_1;
        }
    }

    /*
     * ragUnk0
     */
    ragUnk0::ragUnk0() {
    }

    ragUnk0::~ragUnk0() {
    }
};