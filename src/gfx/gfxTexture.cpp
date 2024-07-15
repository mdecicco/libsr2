#include <libsr2/gfx/gfxTexture.h>
#include <libsr2/gfx/gfxImage.h>

namespace sr2 {
	void NormalizeFilename(char *out, const char *in) {
		long tch;
		char ch;
		
		do {
			ch = *in;
			in = in + 1;
			if ((int)ch - 0x41U < 0x1a) {
				tch = (long)(ch + 0x20);
			}
			else {
				tch = (long)ch;
				if ((long)ch == 0x5c) {
					tch = 0x2f;
				}
			}
			*out = (char)tch;
			out = out + 1;
		} while (tch != 0);
	}

	gfxTexture* gfxTexture::None = nullptr;
	gfxTexture::gfxTexture() {
		m_refCount = 1;
	}

	gfxTexture::~gfxTexture() {
	}

	gfxTexture* gfxTexture::create(u16 width, u16 height, ImageFormat fmt, u32 mipCount) {
		gfxTexture* ret = new gfxTexture();
		ret->m_width = width;
		ret->m_height = height;

		// todo...

		return ret;
	}

	gfxTexture* gfxTexture::create(gfxImage* img, bool p2) {
		u32 mipCount = 0;
		if (p2) {
			gfxImage* mip = img;
			while (mip->getNextMipMap()) {
				mipCount++;
				mip = mip->getNextMipMap();
			}
		}

		vec2i dims = img->getDimensions();
		gfxTexture* tex = create(dims.x, dims.y, img->getFormat(), mipCount);
		if (!tex) return nullptr;

		tex->load(img);
		tex->setTexEnv(tex->m_texEnv | img->getTexEnv());

		return tex;
	}

	gfxTexture* gfxTexture::get(const char* name, i32 unk0, i32 unk1) {
		// todo: this is not at all how this works
		
		char normalizedFilename[128] = { 0 };
		NormalizeFilename(normalizedFilename, name);

		gfxImage* img = gfxImage::LoadTEX(normalizedFilename, unk0);
		if (img) {
			return gfxTexture::create(img, unk0);
		}

		return gfxTexture::None;
	}

	void gfxTexture::getMovie(gfxTexture** out, const char* name, i32 unk0) {
		*out = gfxTexture::None;
	}

	void gfxTexture::release(gfxTexture* tex) {
		if (!tex) return;
		tex->m_refCount--;
		if (tex->m_refCount == 0) delete tex;
	}


	void gfxTexture::addRef() {
		m_refCount++;
	}

	void gfxTexture::setTexEnv(u32 env) {
		m_texEnv = env;
	}

	u32 gfxTexture::getTexEnv() {
		return m_texEnv;
	}
	
	void gfxTexture::load(gfxImage* src) {
		// todo
	}

	vec2i gfxTexture::getDimensions() {
		return { m_width, m_height };
	}
};