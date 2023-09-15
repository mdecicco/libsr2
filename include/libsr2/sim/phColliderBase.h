#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phInst;
    class phBound;
    class phInertialCS;
    class phSleep;
    class phImpact;
    class phImpactBase;
    class phContact;
    class datCallback;

    class phColliderBase {
        public:
            phColliderBase();
            virtual ~phColliderBase();

            void init(phBound* bnd, phInst* inst, phInertialCS* ics);
            void init(phInst* inst, phInertialCS* ics, phSleep* sleep);
            virtual void reset();
            virtual void reset2();
            virtual void update();
            virtual void move();
            virtual void applyGravity();
            virtual void method_0x30();
            virtual void method_0x38();
            virtual void impact(phImpact* impact, i32 p1, const vec3f& p2, u64 p3);
            virtual void impact(i32 p1, i32 p2, u64 p3);
            virtual void contact(phImpactBase* impact, i32 p1, const vec3f& force, const vec3f& p2, const vec3f& p3, const mat3x4f& p4);
            virtual void contact(const vec3f& force, phContact* contact, const mat3x4f& p1);
            virtual void contact(const vec3f& force, phContact* contact);

            phInst* instance;
            phColliderBase* next;
            phColliderBase* last;
            bool active;
            undefined2 unk0;
            datCallback* impact_cb;
            datCallback* bound_cb;
            phBound* bound;
            phInertialCS* ics;
            mat3x4f* read_matrix;
            mat3x4f matrix;
            bool just_reset;
            bool barely_moved;
            f32 max_moved_dist;
            phSleep* sleep;

            undefined4 field20_0x68;
            f32 field23_0x74;
            undefined4 field24_0x78;
            undefined4 field25_0x80;
    };
};