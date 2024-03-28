#pragma once
#include <libsr2/types.h>

namespace sr2 {
	enum ImageFormat : u8 {

	};

	class gfxTexture {
		public:
			gfxTexture();
			~gfxTexture();

			static gfxTexture* get(const char* name, i32 unk0, i32 unk1);
			static void getMovie(gfxTexture** out, const char* name, i32 unk0);
			static gfxTexture* None;

			void setTexEnv(u32 env);
			static void release(gfxTexture* tex);

			u16 width;
			u16 height;
			u16 bytesPerRow;
			// why are there two?
			ImageFormat type;
			ImageFormat format;
			u64 mipmap_bias;
			char* name;
			u32 tex_env;
			u32 ref_count;
			gfxTexture* next;
			void* data;
	};
};