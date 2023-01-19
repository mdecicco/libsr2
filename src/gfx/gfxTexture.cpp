#include <libsr2/gfx/gfxTexture.h>

namespace sr2 {
	gfxTexture* gfxTexture::None = nullptr;
	gfxTexture::gfxTexture() {

	}

	gfxTexture::~gfxTexture() {

	}

	gfxTexture* gfxTexture::get(const char* name, i32 unk0, i32 unk1) {
		return gfxTexture::None;
	}

	void gfxTexture::getMovie(gfxTexture** out, const char* name, i32 unk0) {
		*out = gfxTexture::None;
	}

	void gfxTexture::setTexEnv(u32 env) {
		tex_env = env;
		// this->UpdateRegs();
	}
};