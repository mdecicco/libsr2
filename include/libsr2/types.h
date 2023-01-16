#pragma once
#include <limits.h>
#include <float.h>
#include <stdint.h>

#pragma pack(push, 1)

namespace sr2 {
	typedef intptr_t          word;

	typedef int8_t            i8;
	typedef int16_t           i16;
	typedef int32_t           i32;
	typedef int64_t           i64;

	typedef uint8_t           u8;
	typedef uint16_t          u16;
	typedef uint32_t          u32;
	typedef uint64_t          u64;

	typedef signed char       s8;
	typedef signed short      s16;
	typedef signed int        s32;
	typedef signed long long  s64;

	typedef float             f32;
	typedef double            f64;

	typedef u8                undefined;
	typedef u16               undefined2;
	typedef u32               undefined4;
	typedef undefined*        pointer;

	struct vec4f {
		f32 x, y, z, w;
	};
	struct vec3f {
		f32 x, y, z;
	};
	struct vec2f {
		f32 x, y;
	};
	struct vec2i {
		i32 x, y;
	};
	struct vec2s16 {
		s16 x, y;
	};
	struct mat3x3f {
		vec3f x, y, z;
	};
	struct mat3x4f {
		vec3f x, y, z, w;
	};
	struct mat4x4f {
		vec4f x, y, z, w;
	};

    enum VEHICLE_TYPE : u32 {
        vpsupbug    = 0x0,
        vphondo     = 0x1,
        vpatv       = 0x2,
        vprally2    = 0x3,
        vptrophy    = 0x4,
        vpsuv2      = 0x5,
        vpmil2      = 0x6,
        vphalftrack = 0x7,
        vpmass2     = 0x8,
        vpsnowcycle = 0x9
    };

	enum WEATHER_TYPE : u32 {
		weather_unk0 = 0x0,
		weather_unk1 = 0x1,
		weather_unk2 = 0x2,
		weather_unk3 = 0x3,
		weather_unk4 = 0x4,
		weather_unk5 = 0x5,
		weather_unk6 = 0x6,
	};

	enum MISSION_TYPE : u32 {
		mission_unk0       = 0x0,
		mission_contraband = 0x1,
		mission_unk2       = 0x2,
		mission_unk3       = 0x3,
		mission_unk4       = 0x4,
		mission_unk5       = 0x5,
		mission_unk6       = 0x6
	};
};

#pragma pack(pop)