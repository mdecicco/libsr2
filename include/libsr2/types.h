#pragma once
#include <limits.h>
#include <float.h>
#include <stdint.h>

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
	typedef void*             pointer;

	struct vec4f {
		f32 x, y, z, w;
	};
	struct vec3f {
		f32 x, y, z;
	};
	struct vec2f {
		f32 x, y;
	};
	struct vec2s16 {
		s16 x, y;
	};
	struct mat3x4f {
		vec3f x, y, z, w;
	};
};