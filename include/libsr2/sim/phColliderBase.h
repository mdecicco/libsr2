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

    struct phColliderImpactCallbackData {
        const vec3f* field0_0x0;
        phImpact* impact;
        i32 impactIdx;
    };

    class phColliderBase {
        public:
            phColliderBase();
            virtual ~phColliderBase();

            virtual void reset();
            virtual void update();
            virtual void move();
            virtual void applyGravity();
            virtual void method_0x30();
            virtual void method_0x38();
            virtual void impact(phImpact* impact, i32 impactIdx, const vec3f& p3, const vec3f& p4, u64 p5);
            virtual void impact(phImpact* impact, i32 impactIdx, const vec3f& p3);
            virtual void contact(phImpactBase* impact, i32 p1, const vec3f& force, const vec3f& maybeContactPoint, const vec3f& p3, const mat3x4f& p4);
            virtual void contact(const vec3f& force, phContact* contact, const mat3x4f& p1);
            virtual void contact(const vec3f& force, phContact* contact);

            void updateMatrix();
            phInst* getInstance() const;
            undefined2 getUnk2() const;
            phInertialCS* getICS() const;
            phBound* getBound() const;
            mat3x4f* getReadMatrix() const;
            mat3x4f& getMatrix() const;
            phSleep* getSleep() const;
            void setSleep(phSleep* sleep);
            bool isActive() const;
            void setActive(bool active);
            void setMaxMovedDist(f32 maxMovedDist);
            void setBarelyMoved(bool barelyMoved);
            bool barelyMoved() const;
            void getSphereCenterAndRadius(vec3f& center, f32& radius);

            // possible candidates for moving to other classes
            void init(phBound* bnd, phInst* inst, phInertialCS* ics);
            void init(phInst* inst, phInertialCS* ics, phSleep* sleep);
            void reset2();

            f32 field23_0x74;

        private:
            phInst* m_instance;
            phInst* m_collidingWith;
            phInst* m_lastCollidingWith;
            phColliderBase* m_next;
            phColliderBase* m_last;
            datCallback* m_impactCb;
            datCallback* m_boundCb;
            phBound* m_bound;
            phInertialCS* m_ics;
            mat3x4f* m_readMatrix;
            phSleep* m_sleep;

            bool m_active;
            bool m_needsUpdate;
            mat3x4f m_matrix;
            bool m_justReset;
            bool m_barelyMoved;
            f32 m_maxMovedDist;
            undefined2 m_unk0;
    };
};