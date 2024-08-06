#include <libsr2/managers/srAudMgr.h>
#include <stdio.h>

#include <al.h>

namespace sr2 {
    bool check_al_error() {
		ALenum err = alGetError();
		if (err != AL_NO_ERROR) {
			const char* e = "";
			if (err == AL_INVALID_NAME) e = "AL_INVLAID_NAME";
			if (err == AL_INVALID_ENUM) e = "AL_INVALID_ENUM";
			if (err == AL_INVALID_VALUE) e = "AL_INVALID_VALUE";
			if (err == AL_INVALID_OPERATION) e = "AL_INVALID_OPERATION";
			if (err == AL_OUT_OF_MEMORY) e = "AL_OUT_OF_MEMORY";

            printf("OpenAL error: %s\n", e);
            fflush(stdout);
			return true;
		}

		return false;
	}

	srAudMgr* srAudMgr::instance = nullptr;

	srAudMgr::srAudMgr(i32 p1, i32 p2) : Aud3DObjectManager(p1, p2, 1), m_device(nullptr), m_context(nullptr) {
		m_device = alcOpenDevice(NULL);
		if (!m_device) {
			return;
		}

		m_context = alcCreateContext(m_device, NULL);
		if (!m_context) {
			alcCloseDevice(m_device);
			m_device = nullptr;
			return;
		}

		if (!alcMakeContextCurrent(m_context)) {
			alcDestroyContext(m_context);
			m_context = nullptr;
			alcCloseDevice(m_device);
			m_device = nullptr;
			return;
		}

		setListener(mat4x4f({
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        }));

		srAudMgr::instance = this;
	}

	srAudMgr::~srAudMgr() {
		srAudMgr::instance = nullptr;

		if (!m_device) return;
		alcDestroyContext(m_context);
		m_context = nullptr;
		alcCloseDevice(m_device);
		m_device = nullptr;
	}

	void srAudMgr::setListener(const mat4x4f& transform, const vec3f& velocity) {
		vec3f position = { transform.w.x, transform.w.y, transform.w.z };
		vec3f up = { transform.y.x, transform.y.y, transform.y.z };
		vec3f forward = { transform.z.x, transform.z.y, transform.z.z };
		vec3f orientation[2] = { up, forward };
		
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		if (check_al_error()) m_listenerPosition = position;
		
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		if (check_al_error()) m_listenerVelocity = velocity;
		
		alListenerfv(AL_ORIENTATION, &orientation[0].x);
		if (check_al_error()) {
			m_listenerUp = up;
			m_listenerForward = forward;
		}
	}

	void srAudMgr::setListenerPosition(const vec3f& position) {
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		if (check_al_error()) m_listenerPosition = position;
	}

	void srAudMgr::setListenerVelocity(const vec3f& velocity) {
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		if (check_al_error()) m_listenerVelocity = velocity;
	}

	void srAudMgr::setListenerOrientation(const vec3f& up, const vec3f& forward) {
		vec3f orientation[2] = { up, forward };
		alListenerfv(AL_ORIENTATION, &orientation[0].x);
		if (check_al_error()) {
			m_listenerUp = up;
			m_listenerForward = forward;
		}
	}

    srAudMgr* srAudMgr::get() {
        return instance;
    }

    bool srAudMgr::isAlive() {
        return instance != nullptr;
    }
};