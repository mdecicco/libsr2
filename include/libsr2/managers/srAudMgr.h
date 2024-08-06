#pragma once
#include <libsr2/types.h>
#include <libsr2/managers/Aud3DObjectManager.h>

#include <alc.h>

namespace sr2 {
    class srAudMgr : public Aud3DObjectManager {
        public:
            srAudMgr(i32 p1, i32 p2);
            virtual ~srAudMgr();

            void setListener(const mat4x4f& transform, const vec3f& velocity = vec3f(0, 0, 0));
            void setListenerPosition(const vec3f& position);
            void setListenerVelocity(const vec3f& velocity);
            void setListenerOrientation(const vec3f& up, const vec3f& forward);

            inline const vec3f listenerPosition() const { return m_listenerPosition; }
            inline const vec3f listenerVelocity() const { return m_listenerVelocity; }
            inline const vec3f listenerUp() const { return m_listenerUp; }
            inline const vec3f listenerForward() const { return m_listenerForward; }

            static srAudMgr* get();
            static bool isAlive();
        
        protected:
            static srAudMgr* instance;
            vec3f m_listenerPosition;
            vec3f m_listenerVelocity;
            vec3f m_listenerUp;
            vec3f m_listenerForward;

            ALCdevice* m_device;
            ALCcontext* m_context;
    };
};