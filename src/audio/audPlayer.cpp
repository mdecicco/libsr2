#include <libsr2/audio/audPlayer.h>

namespace sr2 {
    bool check_al_error();
    
    audPlayer::audPlayer() {
        m_source = UINT_MAX;
        alGenSources(1, &m_source);
        check_al_error();

        m_buffer = UINT_MAX;

        m_duration = 0.0f;
        setPosition(vec3f(0, 0, 0));
        setVelocity(vec3f(0, 0, 0));
        setPitch(1.0f);
        setGain(1.0f);
        setDoesLoop(false);

        m_maxDistance = 0.0f;
        m_rolloffFactor = 1.0f;
        m_referenceDistance = 0.0f;
        m_minGain = 0.0f;
        m_maxGain = 0.0f;
        m_coneOuterGain = 0.0f;
        m_coneInnerAngle = 0.0f;
        m_coneOuterAngle = 360.0f;
		m_positionRelative = false;
        m_direction = vec3f(0, 0, 0);
    }

    audPlayer::~audPlayer() {
        if (m_buffer != UINT_MAX) alDeleteBuffers(1, &m_buffer);
        if (m_source != UINT_MAX) alDeleteSources(1, &m_source);
        check_al_error();
    }

    void audPlayer::setPitch(f32 pitch) {
        alSourcef(m_source, AL_PITCH, pitch);
        if (!check_al_error()) m_pitch = pitch;
    }

    void audPlayer::setGain(f32 gain) {
        alSourcef(m_source, AL_GAIN, gain);
        if (!check_al_error()) m_gain = gain;
    }

    void audPlayer::setMinGain(f32 min) {
        alSourcef(m_source, AL_MIN_GAIN, min);
        if (!check_al_error()) m_minGain = min;
    }

    void audPlayer::setMaxGain(f32 max) {
        alSourcef(m_source, AL_MIN_GAIN, max);
        if (!check_al_error()) m_maxGain = max;
    }

    void audPlayer::setMaxDistance(f32 distance) {
        alSourcef(m_source, AL_MAX_DISTANCE, distance);
        if (!check_al_error()) m_maxDistance = distance;
    }

    void audPlayer::setRolloffFactor(f32 factor) {
        alSourcef(m_source, AL_ROLLOFF_FACTOR, factor);
        if (!check_al_error()) m_rolloffFactor = factor;
    }

    void audPlayer::setReferenceDistance(f32 distance) {
        alSourcef(m_source, AL_REFERENCE_DISTANCE, distance);
        if (!check_al_error()) m_referenceDistance = distance;
    }

    void audPlayer::setPositionRelative(bool isRelative) {
        alSourcef(m_source, AL_SOURCE_RELATIVE, isRelative ? AL_TRUE : AL_FALSE);
        if (!check_al_error()) m_positionRelative = isRelative;
    }

    void audPlayer::setConeOuterGain(f32 gain) {
        alSourcef(m_source, AL_CONE_OUTER_GAIN, gain);
        if (!check_al_error()) m_coneOuterGain = gain;
    }

    void audPlayer::setConeInnerAngle(f32 angle) {
        alSourcef(m_source, AL_CONE_INNER_ANGLE, angle);
        if (!check_al_error()) m_coneInnerAngle = angle;
    }

    void audPlayer::setConeOuterAngle(f32 angle) {
        alSourcef(m_source, AL_CONE_OUTER_ANGLE, angle);
        if (!check_al_error()) m_coneOuterAngle = angle;
    }

    void audPlayer::setPosition(const vec3f& pos) {
        alSource3f(m_source, AL_POSITION, pos.x, pos.y, pos.z);
        if (!check_al_error()) m_position = pos;
    }

    void audPlayer::setVelocity(const vec3f& vel) {
        alSource3f(m_source, AL_VELOCITY, vel.x, vel.y, vel.z);
        if (!check_al_error()) m_velocity = vel;
    }

    void audPlayer::setDirection(const vec3f& dir) {
        alSource3f(m_source, AL_DIRECTION, dir.x, dir.y, dir.z);
        if (!check_al_error()) m_direction = dir;
    }

    void audPlayer::setDoesLoop(bool loops) {
        alSourcei(m_source, AL_LOOPING, loops ? AL_TRUE : AL_FALSE);
        if (!check_al_error()) m_loops = loops;
    }

    bool audPlayer::buffer(audio_source_format format, void* data, size_t data_size, size_t sampleRate) {
        if (m_source == UINT_MAX) {
            return false;
        }

        if (m_buffer != UINT_MAX) {
            alDeleteBuffers(1, &m_buffer);
            m_buffer = UINT_MAX;
        }

        alGenBuffers(1, &m_buffer);
        if (check_al_error()) {
            return false;
        }

        alBufferData(m_buffer, format, data, data_size, sampleRate);
        bool ret = check_al_error();
        if (!ret) {
            size_t sample_size = 1;
            size_t channels = 1;
            if (format == asf_stereo_8bit) channels = 2;
            else if (format == asf_mono_16bit) sample_size = 2;
            else if (format == asf_stereo_16bit) {
                sample_size = 2;
                channels = 2;
            }
            m_duration = f32((data_size / channels) / sample_size) / f32(sampleRate);
            alSourcei(m_source, AL_BUFFER, m_buffer);
        } else {
            alDeleteBuffers(1, &m_buffer);
            m_buffer = UINT_MAX;
        }

        return !ret;
    }

    void audPlayer::play() {
        alSourcePlay(m_source);
        check_al_error();
    }

    void audPlayer::pause() {
        alSourcePause(m_source);
        check_al_error();
    }

    void audPlayer::stop() {
        alSourceStop(m_source);
        check_al_error();
    }

    f32 audPlayer::playPosition() {
        f32 r = 0.0f;
        alGetSourcef(m_source, AL_SEC_OFFSET, &r);
        return r;
    }

    bool audPlayer::isPlaying() {
        ALint v = AL_INITIAL;
        alGetSourcei(m_source, AL_SOURCE_STATE, &v);
        return v == AL_PLAYING;
    }

    bool audPlayer::isPaused() {
        ALint v = AL_INITIAL;
        alGetSourcei(m_source, AL_SOURCE_STATE, &v);
        return v == AL_PAUSED;
    }

    bool audPlayer::isStopped() {
        ALint v = AL_INITIAL;
        alGetSourcei(m_source, AL_SOURCE_STATE, &v);
        return v == AL_STOPPED;
    }
};