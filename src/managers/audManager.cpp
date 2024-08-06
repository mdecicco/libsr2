#include <libsr2/managers/audManager.h>
#include <libsr2/managers/Aud3DObjectManager.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/audio/audSound.h>
#include <libsr2/audio/audSoundInfo.h>
#include <libsr2/audio/audPlayer.h>
#include <libsr2/io/stream.h>
#include <libsr2/math/math.h>
#include <libsr2/math/mat3x4f.h>

#include <utils/Array.hpp>

#include <string.h>
#include <stdlib.h>

// VAG-Pack, VAG-Depack, hacked by bITmASTER@bigfoot.com
// v0.1
double f[5][2] = { 
	{ 0.0, 0.0 },
	{  60.0 / 64.0,  0.0 },
	{  115.0 / 64.0, -52.0 / 64.0 },
	{  98.0 / 64.0, -55.0 / 64.0 },
	{  122.0 / 64.0, -60.0 / 64.0 } 
};

double samples[28];

void VAG_Unpack(uint8_t *data, int offset, int databytes, uint8_t **bufferptr, int *outsize)  {
    int predict_nr, shift_factor, flags, datasize;
    int i, d, s, size;
    static double s_1 = 0.0;
    static double s_2 = 0.0;
	uint8_t *in, *out;

	// determine amount if data that should be written
	// fixme: OPTIMIZE
	in = data + offset; // skip VAG header
	datasize = databytes - offset;
	size = 0;
	#define inc() if(datasize < 0) break;in++;datasize--;
	while(datasize > 0)
	{
		predict_nr = in[0]; inc()
		shift_factor = predict_nr & 0xf;
		predict_nr >>= 4;
		flags = in[0]; inc() // flags
		if (flags == 7 || flags == 5)
			break;    
		for (i = 0; i < 28; i += 2) 
		{
			d = in[0]; inc()
			s = (d & 0xf) << 12;
			if (s & 0x8000)
				s |= 0xffff0000;
			samples[i] = (double)(s >> shift_factor);
			s = (d & 0xf0) << 8;
			if (s & 0x8000)
			s |= 0xffff0000;
			samples[i+1] = (double)(s >> shift_factor);
		}
		for ( i = 0; i < 28; i++ ) 
		{
			samples[i] = samples[i] + s_1 * f[predict_nr][0] + s_2 * f[predict_nr][1];
			s_2 = s_1;
			s_1 = samples[i];
			d = (int)(samples[i] + 0.5);
			size += 2;
		}
	}
      
	// decode
	in = data + offset;
	datasize = databytes - offset;
	*bufferptr = new uint8_t[size];
	out = *bufferptr;
	while(1)
	{
		predict_nr = in[0]; inc()
		shift_factor = predict_nr & 0xf;
		predict_nr >>= 4;
		flags = in[0]; inc() // flags
		if (flags == 7 || flags == 5)
			break;   
		for (i = 0; i < 28; i += 2) 
		{
			d = in[0]; inc()
			s = (d & 0xf) << 12;
			if (s & 0x8000)
				s |= 0xffff0000;
			samples[i] = (double)(s >> shift_factor);
			s = (d & 0xf0) << 8;
			if (s & 0x8000)
			s |= 0xffff0000;
			samples[i+1] = (double)(s >> shift_factor);
		}
		for ( i = 0; i < 28; i++ ) 
		{
			samples[i] = samples[i] + s_1 * f[predict_nr][0] + s_2 * f[predict_nr][1];
			s_2 = s_1;
			s_1 = samples[i];
			d = (int)(samples[i] + 0.5);
			out[0] = d & 0xff;
			out[1] = d >> 8;
			out += 2;
		}
	}
	*outsize = size;
	#undef inc
}



namespace sr2 {
    audManager* audManager::instance = nullptr;
    
    i32 audManager::DAT_00362eb0 = 0;
    Aud3DObjectManager* audManager::SomeManager = nullptr;
    i32 audManager::Count = 0;
    i32 audManager::DAT_00362ea8 = 0;
    f32 audManager::FLOAT_00362ee8 = 0;
    i32 audManager::DAT_00362eec = 0;
    u32 audManager::DAT_00362eac = 0;
    i32 audManager::DAT_00362eb4 = 0;
    i32 audManager::DAT_00362eb8[4] = { 0 };
    i32 audManager::DAT_00362ec8[4] = { 0 };
    i32 audManager::DAT_00362ed8[4] = { 0 };

    audManager::audManager(i32 someCount, i32 p2, i32 p3) {
        audManager::instance = this;
        m_streamMemoryEnd = 0x5010;
        field_0x2e0 = p3;
        m_someArrayContainer = nullptr;
        field_0x8 = 0;
        m_streamsRemaining = 0;
        field_0x13b5 = 0;
        m_pathPrefix[0] = 0;
        m_useHostFS = true;
        m_maxStreams = 0;
        m_someCount = 0;
        m_hasSomeCount = false;
        // m_archive = nullptr;
        field_0x2ec = 0;
        m_hasAudioMem = false;
        m_sfxCapacity = someCount;
        field_0x14bc = math::max(p2, i32(0x1e));
        field_0x1370 = 0;
        m_reservedSfxCount = 0;
        field_0x2ac = 0;
        field_0x294 = 1;
        m_someSampleRate = 22050;
        field_0x2d0 = 8;
        field_0x2d4 = 4;
        field_0x2b8 = 2;
        field_0x298 = 4;
        m_sounds = new audSound[m_sfxCapacity];
        m_currentListenerTransform = new mat3x4f;
        math::zero(*m_currentListenerTransform);
        
        init();
    }

    audManager::~audManager() {
    }



    void audManager::init() {
        m_nextSampleRateCode = SampleRateCode::srZero;
        m_nextIndex = 0;
        m_lastLoaded = nullptr;
        field_0x370 = 0;
        m_somePath[0] = 0;
        m_pathBase[0] = 0;
        m_sfxCount = 0;
        m_totalCtrlCount = 0;
        m_totalUnk1Count = 0;
        m_totalAudioMem = 0;
        m_totalAlignedAudioMem = 0;
        field_0x14dc = 0;
        math::zero(*m_currentListenerTransform);

        for (u32 i = 0;i < 16;i++) {
            m_audioTypeCtrls[i].count = 0;
            m_audioTypeCtrls[i].volume = 1.0f;
            m_audioTypeCtrls[i].field_0x8 = 0;
            m_audioTypeCtrls[i].field_0xc = 0.0f;
            m_audioTypeCtrls[i].field_0x10 = 1.0f;

            m_unk[i].count = 0;
            m_unk[i].balance = 0.0f;
            m_unk[i].field_0x8 = 0.0f;
            m_unk[i].index = 0;
            m_unk[i].field_0x10 = 1.0f;
        }

        FUN_0032ce58();

        field_0xf40 = 0;
        field_0xf50 = 0xffffffff;
        field_0xf44 = 0;
        field_0xf48 = 0;
        field_0xf4c = 0;
        field_0x1314 = 0;
        field_0x1318 = 0;
        field_0x131c = 0;
        field_0x1350 = 0;
        field_0x1354 = 0;
        field_0x1358 = 0;
        field_0x135c = 0;
        field_0x1360 = 0;
        field_0x13c0 = 0;
        field_0x136c = 0;
        field_0x1364 = 0;
        m_filenameCount = 0;
        m_totalFileCount = 0;
    }

    void audManager::shutdown() {
    }

    u32 audManager::getFreeIndex() {
        for (u32 i = m_reservedSfxCount + m_sfxCount;i < m_sfxCapacity;i++) {
            if (!m_sounds[i].isUsed()) return i;
        }

        return 0xFFFFFFFF;
    }

    audSound* audManager::findSFX(const char* filePath, u32 startIndex, u32 notThisIndex) {
        u32 len = strlen(filePath);
        const char* filename = filePath + len - 1;
        while (*filename != '/' && *filename != '\\') filename--;
        if (*filename == '/' || *filename == '\\') filename++;

        for (u32 i = startIndex;i < m_sfxCapacity;i++) {
            audSound* snd = &m_sounds[i];
            if (!snd->m_info || snd->m_index == notThisIndex) continue;
            if (strstr(snd->m_info->m_leftChannelPath, filename)) return snd;
        }

        return nullptr;
    }
    
    audManager::AudioTypeCtrl* audManager::getAudioTypeCtrl(AudioTypeIndex index) {
        return &m_audioTypeCtrls[(u32)index];
    }

    void audManager::allocateAudioHeap() {
        if (m_hasAudioMem) return;
        m_hasAudioMem = true;
        m_totalAlignedAudioMem = 0x5010;
        field_0x14c8 = 0;
    }

    undefined4 audManager::initClient() {
        allocateAudioHeap();
        return initStreams();
    }

    undefined4 audManager::initStreams() {
        for (u32 i = 0;i < m_maxStreams;i++) {
            m_totalAlignedAudioMem += 0xd700;
        }

        m_streamMemoryEnd = m_totalAlignedAudioMem;
        return 0;
    }

    void audManager::setMasterVolume(f32 p1) {
        m_masterVolume = f32(i32(math::clamp(p1, 0.0f, 1.0f) * 16383.0f));
    }

    void audManager::FUN_00329f60(i32 p1, i32 p2, i32 p3, i32 maxStreams) {
        m_maxStreams = maxStreams;
        m_masterVolume = 16383.0f;
        m_someOffset = p1 * 0x100 + 0x40;
        field_0x2bc = p1;
        m_useHostFS = true;
        if (p2 > 0) {
            m_hasSomeCount = true;
            m_someCount = p2;
        }

        if (!m_useHostFS) {
            // setPathPrefix("cdrom0:");
        } else {
            // setPathPrefix("host0:");
        }

        if (m_maxStreams == 0) m_maxStreams = 1;

        initClient();

        // todo:
    }

    void audManager::FUN_0032a0c8(i32 p1) {
    }

    void audManager::FUN_0032a328() {
    }

    audSound* audManager::getSoundEffect(u32 idx) {
        return &m_sounds[idx];
    }
    
    const char* audManager::getPathPrefix() {
        return m_pathPrefix;
    }

    const char* audManager::getPathBase() {
        return m_pathBase;
    }

    const char* audManager::getSomePath() {
        return m_somePath;
    }

    void audManager::setPathPrefix(const char* pathPrefix) {
        strncpy(m_pathPrefix, pathPrefix, 128);
    }

    void audManager::setPathBase(const char* pathBase) {
        if (strlen(datAssetManager::getPath()) == 0) {
            snprintf(m_pathBase, 32, "%s\\", pathBase);
        } else {
            snprintf(m_pathBase, 32, "%s%s\\", datAssetManager::getPath(), pathBase);
        }

        u32 len = strlen(m_pathBase);
        for (u32 i = 0;i < len;i++) {
            if (m_pathBase[i] == '/') m_pathBase[i] = '\\';
        }
    }

    void audManager::FUN_0032a720() {
    }

    void audManager::FUN_0032a850() {
    }

    void audManager::FUN_0032a880(i32 p1, i32 p2) {
    }

    undefined4* audManager::FUN_0032a958(i32 p1, i32 p2) {
        return nullptr;
    }

    void audManager::FUN_0032aa18(i32 p1) {
    }

    audSound* audManager::loadSoundAtIndex(const char* filename, i32 someIdx) {
        m_nextIndex = someIdx;
        i32 result = m_sounds[someIdx].load(filename);
        m_nextIndex = 0;

        if (result < 0) return nullptr;
        return &m_sounds[someIdx];
    }

    void audManager::FUN_0032aaa0(f32 p1, f32 p2, f32 p3, i32 p4) {
    }

    void audManager::FUN_0032abf8(f32 p1, i32 p2) {
    }

    void audManager::FUN_0032ace0() {
    }

    u32 audManager::FUN_0032adb8(f32 p1, f32 p2, f32 p3, i32 p4) {
        return 0;
    }

    void audManager::FUN_0032af10(f32 p1, i32 p2) {
    }

    void audManager::FUN_0032aff8() {
    }

    void audManager::FUN_0032b0b8(mat3x4f* p1) {
    }

    mat3x4f* audManager::FUN_0032b0d8() {
        return nullptr;
    }

    undefined4 audManager::FUN_0032b0e0(i32 p1) {
        return 0;
    }

    void audManager::setReference(audSound* original, audSound* referencedBy) {
        if (!referencedBy->isUsed()) {
            if (referencedBy->m_info) delete referencedBy->m_info;
            referencedBy->m_info = new audSoundInfo();
        }

        referencedBy->m_info->m_owner = original->m_info->m_owner;
        referencedBy->m_info->m_flags = original->m_info->m_flags;
        referencedBy->m_info->m_memOffset = original->m_info->m_memOffset;
        referencedBy->m_info->m_channelSize = original->m_info->m_channelSize;
        referencedBy->m_info->m_sampleRate = original->m_info->m_sampleRate;
    }

    i32 audManager::FUN_0032b410(i32 p1, f32* p2) {
        return 0;
    }

    i32 audManager::FUN_0032b4a0(i32 p1, f32* p2) {
        return 0;
    }

    i32 audManager::FUN_0032b530(i32 p1, f32* p2, i32 p3, i32 p4) {
        return 0;
    }

    void audManager::FUN_0032b5d8(mat3x4f* p1) {
    }

    i32 audManager::FUN_0032b620() {
        return 0;
    }

    void audManager::FUN_0032b760() {
    }
    
    undefined4 audManager::FUN_0032b8e8() {
        return 0;
    }

    void audManager::FUN_0032b958(f32 p1, f32* p2, f32* p3) {
    }

    i32 audManager::FUN_0032ba10(i32 p1) {
        return 0;
    }

    void audManager::setNextSampleRateCode(SampleRateCode p1) {
        if (i32(p1) != 0) m_nextSampleRateCode = p1;
    }

    audSound* audManager::FUN_0032bb60() {
        return nullptr;
    }

    f32 audManager::FUN_0032bc10(f32* p1) {
        return 0.0f;
    }

    f32 audManager::FUN_0032c138(vec3f* p1) {
        return 0.0f;
    }

    void audManager::FUN_0032c180(u8 p1) {
    }

    void audManager::FUN_0032c1f0(f32 p1, i32 p2) {
    }

    void audManager::FUN_0032c2c8(i32 p1, undefined2 p2) {
    }

    void audManager::FUN_0032c340(f32 p1, i32 p2) {
    }

    void audManager::FUN_0032c458(i32 p1) {
    }

    i32 audManager::FUN_0032c580(f32 p1, f32 p2, f32 p3, i32 p4, i32 p5, undefined4 p6, undefined4 p7, undefined4 p8, i32 p9) {
        return 0;
    }

    i32 audManager::addFile(i32 p1, const char* p2) {
        m_filenames.push(p2);
        return m_filenames.size() - 1;
    }

    undefined4 audManager::FUN_0032c958(const char* p2) {
        return 0;
    }

    undefined4 audManager::FUN_0032c9a0(undefined4 p1, undefined4 p2) {
        return 0;
    }

    undefined4 audManager::FUN_0032c9d0() {
        return 0;
    }

    undefined4 audManager::FUN_0032c9d8(const char* p1) {
        return 0;
    }

    bool audManager::loadVAG(audSound* sfx, const char* path) {
        Stream* fp = datAssetManager::open(path, "VAG", 0, true);
        if (!fp) return false;

        #pragma pack(push, 1)
        struct VAG_HEADER {
            char magic[4];
            u32 version;
            undefined reserved0[4];
            u32 channelSize;
            u32 sampleRate;
            undefined reserved1[12];
            char optionalName[16];
        } header;
        #pragma pack(pop)

        fp->read(&header, sizeof(VAG_HEADER));

        u32 channelSz = _byteswap_ulong(header.channelSize);
        u32 sampleRate = _byteswap_ulong(header.sampleRate);
        u8* data = new u8[channelSz];
        fp->read(data, channelSz);
        u8 someFlag = data[0x11];
        delete fp;

        u8* decoded = nullptr;
        i32 decodedSize = 0;
        VAG_Unpack(data, 0, channelSz, &decoded, &decodedSize);
        delete [] data;
        
        sfx->m_player = new audPlayer();
        audio_source_format fmt = asf_mono_16bit;
        sfx->m_player->buffer(fmt, decoded, decodedSize, sampleRate);
        delete [] decoded;

        if (!sampleRate) return false;

        sfx->m_sampleRateCode = SampleRateCode::srUnk0;
        sfx->m_rightChannelFilenameIdx = m_totalFileCount + m_filenameCount;
        sfx->m_leftChannelFilenameIdx = 0;
        sfx->m_sampleRate = sampleRate;
        sfx->field_0x18 = 0;
        sfx->field_0x19 = 0;
        sfx->m_info->m_memOffset = m_totalAlignedAudioMem;
        sfx->m_info->m_sampleRate = sampleRate;
        sfx->m_info->m_channelSize = channelSz;
        sfx->m_info->m_owner = sfx;
        sfx->m_info->m_flags = someFlag;

        if (sfx->hasFlag(1)) sfx->m_player->setDoesLoop(true);

        u32 len = strlen(path);
        const char* filename = path + len - 1;
        while (*filename != '/' && *filename != '\\') filename--;
        if (*filename == '/' || *filename == '\\') filename++;

        strcpy(sfx->m_info->m_leftChannelPath, filename);
        strcpy(sfx->m_info->m_rightChannelPath, path);

        m_totalFileCount++;
        m_totalAudioMem += channelSz;
        m_totalAlignedAudioMem += (channelSz + 0x3f) & 0xFFFFFFC0;
        return true;
    }

    i32 audManager::FUN_0032cd08(i32 p1) {
        return 0;
    }

    void audManager::FUN_0032ce58() {
        for (u32 i = 0;i < 0x30 - m_maxStreams;i++) {
            field_0x13d8[i] = 0xFF;
            field_0x1408[i] = 0;
        }

        field_0x13cc = 0;
        field_0x13c0 = 1;
        field_0x13c8 = 0;
    }

    void audManager::FUN_0032ceb0(i32 p1) {
    }

    undefined4 audManager::FUN_0032cec0(i32 p1) {
        return 0;
    }

    i32 audManager::FUN_0032ced0(i32 p1) {
        return 0;
    }

    bool audManager::FUN_0032cf28(i32 p1) {
        return false;
    }

    u32 audManager::FUN_0032cf58(i32 p1) {
        return 0;
    }

    i32 audManager::FUN_0032cf80() {
        return 0;
    }

    void audManager::FUN_0032d010(undefined p1) {
    }

    i32 audManager::FUN_0032d070(i32 p1, i32 p2, i32 p3) {
        return 0;
    }

    undefined4 audManager::FUN_0032d108(u32 p1, i32 p2) {
        return 0;
    }

    undefined4 audManager::FUN_0032d198(u32 p1, i32 p2, i32 p3) {
        return 0;
    }

    void audManager::FUN_0032d1f0(f32 p1, f32 p2, f32 p3, f32 p4, i32 p5) {
    }

    void audManager::FUN_0032d220(f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6, i32 p7) {
    }

    void audManager::FUN_0032d2e0(f32 p1) {
    }

    void audManager::FUN_0032d328(f32 p1) {
    }

    void audManager::FUN_0032d370(f32 p1, f32 p2) {
    }

    void audManager::FUN_0032d3f0(f32 p1, f32 p2) {
    }

    void audManager::FUN_0032d440(i32 p1, undefined2 p2) {
    }

    void audManager::FUN_0032d478(undefined2 p2) {
    }

    void audManager::FUN_0032d4a0(u16 p1, u16 p2) {
    }

    void audManager::FUN_0032d4d0(u16 p1, i32 p2, i32 p3, u16 p4, undefined2 p6, undefined2 p7, i16 p8, i32 p9) {
    }

    void audManager::FUN_0032d538() {
    }

    void audManager::FUN_0032d598(u16 p1, i32 p2, u16 p3, u16 p4, u16 p5, i16 p6, i32 p7) {
    }

    void audManager::FUN_0032d5f0(u16 p1, u16 p2, u16 p3) {
    }

    void audManager::FUN_0032d620(u16 p1, u16 p2) {
    }

    void audManager::FUN_0032d650(u16 p1) {
    }

    void audManager::FUN_0032d678(u16 p1) {
    }

    void audManager::FUN_0032d6a0(u16 p1, u16 p2) {
    }

    void audManager::FUN_0032d6e0() {
    }

    void audManager::FUN_0032d700() {
    }

    void audManager::FUN_0032d720(u16 p1) {
    }

    void audManager::FUN_0032d748(u16 p1) {
    }

    void audManager::FUN_0032d770(u16 p1) {
    }

    void audManager::FUN_0032d798(u16 p1, u16 p2, u16 p3) {
    }

    void audManager::FUN_0032d7d8(u16 p1) {
    }

    void audManager::FUN_0032d808() {
    }

    void audManager::FUN_0032d830() {
    }

    void audManager::FUN_0032d858(i32 p1) {
    }

    void audManager::FUN_0032d8a8(u16 p1, u16 p2) {
    }

    void audManager::FUN_0032d8d8(u16 p1, u16 p2) {
    }

    void audManager::FUN_0032d908(u16 p1, u16 p2) {
    }

    void audManager::FUN_0032d938(u16 p1, u8 p2) {
    }

    void audManager::FUN_0032d9c0(u16 p1, u16 p2, u16* p3) {
    }

    void audManager::FUN_0032da38(u16 p1, u16 p2, u16 p3, u16 p4, undefined4 p5) {
    }

    void audManager::FUN_0032da78(undefined2 p1, i16 p2) {
    }

    void audManager::FUN_0032db50() {
    }

    void audManager::FUN_0032db78(i32 p1) {
    }

    void audManager::FUN_0032dc00() {
    }

    u32 audManager::FUN_0032de88(i32 p1) {
        return 0;
    }

    i32 audManager::transferToIOP(void* src, void* dst, i32 size) {
        return 0;
    }

    void audManager::initStatics() { 
        audManager::DAT_00362eb0 = 1;
        audManager::DAT_00362eb8[3] = -1;
        audManager::DAT_00362ec8[0] = 0;
        audManager::SomeManager = nullptr;
        audManager::Count = 0;
        audManager::DAT_00362ea8 = 0;
        audManager::DAT_00362eac = 0;
        audManager::DAT_00362eb4 = 0;
        audManager::DAT_00362eb8[0] = -1;
        audManager::DAT_00362ec8[3] = 0;
        audManager::DAT_00362ed8[0] = 0;
        audManager::DAT_00362ed8[3] = 0;
        audManager::FLOAT_00362ee8 = 0.0;
        audManager::DAT_00362eec = 0;
        audManager::DAT_00362eb8[1] = -1;
        audManager::DAT_00362eb8[2] = -1;
        audManager::DAT_00362ec8[1] = 0;
        audManager::DAT_00362ec8[2] = 0;
        audManager::DAT_00362ed8[1] = 0;
        audManager::DAT_00362ed8[2] = 0;
    }

    void audManager::loadSoundBanks(const char* path, undefined4 p2) {
        audManager::initStatics();

        if (Aud3DObjectManager::isAlive()) {
            Aud3DObjectManager::get()->method_0x130();
            audManager::SomeManager = Aud3DObjectManager::get();
        }

        if (audManager::SomeManager) {
            audManager::get()->field_0x298 = 4;
            audManager::get()->field_0x294 = 1;
            audManager::DAT_00362eac = audManager::SomeManager->loadSoundBanksAtPath(path, p2);
        }

        audManager::DAT_00362eb0 = 0;
    }

    f32 audManager::FUN_0032b9f8(f32 p1) {
        return 0.0f;
    }

    audSound* audManager::getOffsetSFX(u32 idx) {
        return instance->getSoundEffect(idx + instance->m_reservedSfxCount);
    }

    void audManager::playSound(i32 soundIdx) {
        if (!audManager::instance) return;

        audSound* snd = audManager::getOffsetSFX(soundIdx);
        snd->play();
    }

    audManager* audManager::get() {
        return instance;
    }

    void audManager::create(i32 someCount, i32 p2, i32 p3) {
        if (audManager::instance) return;
        audManager::instance = new audManager(someCount, p2, p3);
    }

    void audManager::destroy() {
        if (!audManager::instance) return;
        delete audManager::instance;
        audManager::instance = nullptr;
    }
};