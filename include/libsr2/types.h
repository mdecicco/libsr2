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
	typedef u64               undefined8;
	typedef undefined*        pointer;

	struct vec2f {
		f32 x, y;
	};
	struct vec3f {
		f32 x, y, z;

		vec3f();
		vec3f(f32 x, f32 y, f32 z);

		vec3f operator-() const;
		vec3f operator-(const vec3f& rhs) const;
		vec3f operator+(const vec3f& rhs) const;
		vec3f operator*(const vec3f& rhs) const;
		vec3f operator/(const vec3f& rhs) const;
		vec3f operator-(f32 rhs) const;
		vec3f operator+(f32 rhs) const;
		vec3f operator*(f32 rhs) const;
		vec3f operator/(f32 rhs) const;
		vec3f operator-=(const vec3f& rhs);
		vec3f operator+=(const vec3f& rhs);
		vec3f operator*=(const vec3f& rhs);
		vec3f operator/=(const vec3f& rhs);
		vec3f operator-=(f32 rhs);
		vec3f operator+=(f32 rhs);
		vec3f operator*=(f32 rhs);
		vec3f operator/=(f32 rhs);
		f32 operator[](u8 axis) const;
		f32& operator[](u8 axis);

		vec3f cross(const vec3f& rhs) const;
		vec3f normalized() const;
		void normalize();
		f32 dot(const vec3f& rhs) const;
		f32 length() const;
		f32 lengthSq() const;
		void zero();
	};
	struct vec4f {
		vec4f();
		vec4f(f32 x, f32 y, f32 z, f32 w);
		vec4f(const vec3f& rhs);

		vec4f operator-() const;
		vec4f operator-(const vec4f& rhs) const;
		vec4f operator+(const vec4f& rhs) const;
		vec4f operator*(const vec4f& rhs) const;
		vec4f operator/(const vec4f& rhs) const;
		vec4f operator-(f32 rhs) const;
		vec4f operator+(f32 rhs) const;
		vec4f operator*(f32 rhs) const;
		vec4f operator/(f32 rhs) const;
		vec4f operator-=(const vec4f& rhs);
		vec4f operator+=(const vec4f& rhs);
		vec4f operator*=(const vec4f& rhs);
		vec4f operator/=(const vec4f& rhs);
		vec4f operator-=(f32 rhs);
		vec4f operator+=(f32 rhs);
		vec4f operator*=(f32 rhs);
		vec4f operator/=(f32 rhs);
		f32 operator[](u8 axis) const;
		f32& operator[](u8 axis);
		
		void zero();

		f32 x, y, z, w;
	};

	vec3f operator*(f32 lhs, const vec3f& rhs);

	struct vec2b {
		i8 x, y;
	};

	struct vec2ub {
		u8 x, y;
	};

	struct vec2i {
		i32 x, y;
	};

	struct vec2s16 {
		s16 x, y;
	};

	struct quat {
		vec3f axis;
		f32 angle;
	};

	struct quat_transform {
		quat orient;
		vec3f position;
	};

	struct mat3x3f {
		vec3f x, y, z;

		vec3f operator*(const vec3f& rhs) const;
		vec3f multTransposed(const vec3f& rhs) const;
		mat3x3f inverse() const;
		mat3x3f transposed() const;
		
		vec3f operator[](u8 axis) const;
		vec3f& operator[](u8 axis);
	};

	struct mat3x4f {
		vec3f x, y, z, w;

		vec3f operator*(const vec3f& rhs) const;
		mat3x4f operator*(const mat3x4f& rhs) const;
		mat3x4f transposed() const;

		vec3f operator[](u8 axis) const;
		vec3f& operator[](u8 axis);

		const mat3x3f& basis() const;
		mat3x3f& basis();

		static const mat3x4f identity;
	};

	struct mat4x4f {
		vec3f operator*(const vec3f& rhs) const;
		mat4x4f operator*(const mat3x4f& rhs) const;
		mat4x4f operator*(const mat4x4f& rhs) const;
		mat4x4f operator*=(const mat3x4f& rhs);
		mat4x4f operator*=(const mat4x4f& rhs);
		mat3x4f to3x4() const;
		mat3x3f basis() const;
		mat4x4f transposed() const;

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

	enum PLAYER_TYPE : u32 {
		Human,
		AI,
		Parked
	};

	enum GAME_MODE : u32 {
		Cruise = 0x0,
		Checkpoint = 0x1,
		CrooksAndSmugglers = 0x2,
		LootGrab = 0x3,
		BombTag = 0x4,
		Career = 0x5
	};

	enum MAP_SELECTION : u32 {
		Afghan = 0x0,
		Vietnam = 0x1,
		AfghanSnow = 0x2,
		UnknownMap = 0x3
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

    enum LANGUAGE {
        English = 0,
        Spanish,
        French,
        German,
        Italian,
        Portugese,
        Japanese,
        Chinese,
        Korean,
        Norwegian
    };

    enum class ImageFormat : u8 {
        UNK0 = 0,
        RGBA8888,
        RGB888,
        UNK3,
        RGBA5551,
        RGBA_8888_P8,
        RGBA_8888_P4,
        UNK8,
        UNK9,
        UNK10,
        UNK11
    };

	enum Button : u32 {
		None = 0x0,
		L2 = 0x1,
		R2 = 0x2,
		L1 = 0x4,
		R1 = 0x8,
		TRIANGLE = 0x10,
		CIRCLE = 0x20,
		CROSS = 0x40,
		SQUARE = 0x80,
		SELECT = 0x100,
		L3 = 0x200,
		R3 = 0x400,
		START = 0x800,
		UP = 0x1000,
		RIGHT = 0x2000,
		DOWN = 0x4000,
		LEFT = 0x8000
	};

	enum KEY_CODE : u32 {
		UNK0 = 0,
		UNK1 = 1,
		UNK2 = 2,
		UNK3 = 3,
		UNK4 = 4,
		UNK5 = 5,
		UNK6 = 6,
		UNK7 = 7,
		UNK8 = 8,
		UNK9 = 9,
		UNK10 = 10,
		UNK11 = 11,
		UNK12 = 12,
		UNK13 = 13,
		UNK14 = 14,
		UNK15 = 15,
		UNK16 = 16,
		UNK17 = 17,
		UNK18 = 18,
		UNK19 = 19,
		UNK20 = 20,
		UNK21 = 21,
		UNK22 = 22,
		UNK23 = 23,
		UNK24 = 24,
		UNK25 = 25,
		UNK26 = 26,
		UNK27 = 27,
		UNK28 = 28,
		UNK29 = 29,
		UNK30 = 30,
		UNK31 = 31,
		UNK32 = 32,
		UNK33 = 33,
		UNK34 = 34,
		UNK35 = 35,
		UNK36 = 36,
		UNK37 = 37,
		UNK38 = 38,
		UNK39 = 39,
		UNK40 = 40,
		UNK41 = 41,
		UNK42 = 42,
		UNK43 = 43,
		UNK44 = 44,
		UNK45 = 45,
		UNK46 = 46,
		UNK47 = 47,
		UNK48 = 48,
		UNK49 = 49,
		UNK50 = 50,
		UNK51 = 51,
		UNK52 = 52,
		UNK53 = 53,
		UNK54 = 54,
		UNK55 = 55,
		UNK56 = 56,
		UNK57 = 57,
		UNK58 = 58,
		UNK59 = 59,
		UNK60 = 60,
		UNK61 = 61,
		UNK62 = 62,
		UNK63 = 63,
		UNK64 = 64,
		UNK65 = 65,
		UNK66 = 66,
		UNK67 = 67,
		UNK68 = 68,
		UNK69 = 69,
		UNK70 = 70,
		UNK71 = 71,
		UNK72 = 72,
		UNK73 = 73,
		UNK74 = 74,
		UNK75 = 75,
		UNK76 = 76,
		UNK77 = 77,
		UNK78 = 78,
		UNK79 = 79,
		UNK80 = 80,
		UNK81 = 81,
		UNK82 = 82,
		UNK83 = 83,
		UNK84 = 84,
		UNK85 = 85,
		UNK86 = 86,
		UNK87 = 87,
		UNK88 = 88,
		UNK89 = 89,
		UNK90 = 90,
		UNK91 = 91,
		UNK92 = 92,
		UNK93 = 93,
		UNK94 = 94,
		UNK95 = 95,
		UNK96 = 96,
		UNK97 = 97,
		UNK98 = 98,
		UNK99 = 99,
		UNK100 = 100,
		UNK101 = 101,
		UNK102 = 102,
		UNK103 = 103,
		UNK104 = 104,
		UNK105 = 105,
		UNK106 = 106,
		UNK107 = 107,
		UNK108 = 108,
		UNK109 = 109,
		UNK110 = 110,
		UNK111 = 111,
		UNK112 = 112,
		UNK113 = 113,
		UNK114 = 114,
		UNK115 = 115,
		UNK116 = 116,
		UNK117 = 117,
		UNK118 = 118,
		UNK119 = 119,
		UNK120 = 120,
		UNK121 = 121,
		UNK122 = 122,
		UNK123 = 123,
		UNK124 = 124,
		UNK125 = 125,
		UNK126 = 126,
		UNK127 = 127,
		UNK128 = 128,
		UNK129 = 129,
		UNK130 = 130,
		UNK131 = 131,
		UNK132 = 132,
		UNK133 = 133,
		UNK134 = 134,
		UNK135 = 135,
		UNK136 = 136,
		UNK137 = 137,
		UNK138 = 138,
		UNK139 = 139,
		UNK140 = 140,
		UNK141 = 141,
		UNK142 = 142,
		UNK143 = 143,
		UNK144 = 144,
		UNK145 = 145,
		UNK146 = 146,
		UNK147 = 147,
		UNK148 = 148,
		UNK149 = 149,
		UNK150 = 150,
		UNK151 = 151,
		UNK152 = 152,
		UNK153 = 153,
		UNK154 = 154,
		UNK155 = 155,
		UNK156 = 156,
		UNK157 = 157,
		UNK158 = 158,
		UNK159 = 159,
		UNK160 = 160,
		UNK161 = 161,
		UNK162 = 162,
		UNK163 = 163,
		UNK164 = 164,
		UNK165 = 165,
		UNK166 = 166,
		UNK167 = 167,
		UNK168 = 168,
		UNK169 = 169,
		UNK170 = 170,
		UNK171 = 171,
		UNK172 = 172,
		UNK173 = 173,
		UNK174 = 174,
		UNK175 = 175,
		UNK176 = 176,
		UNK177 = 177,
		UNK178 = 178,
		UNK179 = 179,
		UNK180 = 180,
		UNK181 = 181,
		UNK182 = 182,
		UNK183 = 183,
		UNK184 = 184,
		UNK185 = 185,
		UNK186 = 186,
		UNK187 = 187,
		UNK188 = 188,
		UNK189 = 189,
		UNK190 = 190,
		UNK191 = 191,
		UNK192 = 192,
		UNK193 = 193,
		UNK194 = 194,
		UNK195 = 195,
		UNK196 = 196,
		UNK197 = 197,
		UNK198 = 198,
		UNK199 = 199,
		UNK200 = 200,
		UNK201 = 201,
		UNK202 = 202,
		UNK203 = 203,
		UNK204 = 204,
		UNK205 = 205,
		UNK206 = 206,
		UNK207 = 207,
		UNK208 = 208,
		UNK209 = 209,
		UNK210 = 210,
		UNK211 = 211,
		UNK212 = 212,
		UNK213 = 213,
		UNK214 = 214,
		UNK215 = 215,
		UNK216 = 216,
		UNK217 = 217,
		UNK218 = 218,
		UNK219 = 219,
		UNK220 = 220,
		UNK221 = 221,
		UNK222 = 222,
		UNK223 = 223,
		UNK224 = 224,
		UNK225 = 225,
		UNK226 = 226,
		UNK227 = 227,
		UNK228 = 228,
		UNK229 = 229,
		UNK230 = 230,
		UNK231 = 231,
		UNK232 = 232,
		UNK233 = 233,
		UNK234 = 234,
		UNK235 = 235,
		UNK236 = 236,
		UNK237 = 237,
		UNK238 = 238,
		UNK239 = 239,
		UNK240 = 240,
		UNK241 = 241,
		UNK242 = 242,
		UNK243 = 243,
		UNK244 = 244,
		UNK245 = 245,
		UNK246 = 246,
		UNK247 = 247,
		UNK248 = 248,
		UNK249 = 249,
		UNK250 = 250,
		UNK251 = 251,
		UNK252 = 252,
		UNK253 = 253,
		UNK254 = 254,
		UNK255 = 255,
		UNK256 = 256
	};
};

#pragma pack(pop)