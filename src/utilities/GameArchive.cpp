#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/Data.h>
#include <libsr2/utilities/utils.h>

#include <zlib.h>
#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include <zip_file.hpp>

#include <string.h>

#include <exception>
#include <unordered_map>
#include <algorithm>

namespace sr2 {
	typedef std::unordered_map<std::string, u32> map_t;
	GameArchive* g_Archives = nullptr;

    GameArchive::GameArchive(const char* path) {
		next = g_Archives;
		g_Archives = this;

		m_files = nullptr;
		m_fileCount = 0;
		m_archive = nullptr;
		m_fileMap = nullptr;
        m_archive = new Data(path);

		try {
			char hdr[4] = { 0 };
			m_archive->read(hdr);
			if (!strncmp(hdr, "DAVE", 4)) {
				m_archive->read(m_fileCount);
				m_files = new entry[m_fileCount];

				u32 dirOffset = 0;
				m_archive->read(dirOffset);
				dirOffset += 2048;

				m_archive->position(2048);

				for (u64 i = 0;i < m_fileCount;i++) {
					entry& e = m_files[i];
					e.name_offset = m_archive->read<u32>() + dirOffset;
					e.offset = m_archive->read<u32>();
					e.decompressed_sz = m_archive->read<u32>();
					e.compressed_sz = m_archive->read<u32>();
				}

				m_fileMap = (void*)new map_t();
				map_t& map = *(map_t*)m_fileMap;
				char name_buf[128];
				for (u64 i = 0;i < m_fileCount;i++) {
					m_archive->position(m_files[i].name_offset);
					m_archive->read_str(name_buf, 128);
					std::string filename = name_buf;

					std::transform(filename.begin(), filename.end(), filename.begin(), std::tolower);
					map[filename] = i;
				}

				m_archive->position(0);
				//throw std::exception(format("'%s' is not a 'DAVE' archive", path).c_str());
			} else {
				m_archive->position(m_archive->size() - 22);
				u32 u0 = m_archive->read<u32>();
				if (u0 == 0x6054b50) {
					u16 u1 = m_archive->read<u16>();
					u16 u2 = m_archive->read<u16>();

					if (u1 == u2) {
						m_fileCount = m_archive->read<u16>();
						u16 u4 = m_archive->read<u16>();
						u32 u5 = m_archive->read<u32>();
						u32 u6 = m_archive->read<u32>();

						if (u6 & 0x7ff) {
							// File::FUN_0033cf08(fp); ?
						}

						m_files = new entry[m_fileCount];
						m_archive->position(u6);

						u32 u7 = m_archive->read<u32>();
						u8 buf0[32];
						char name[256];
						u8 buf2[256];
						u8 buf3[256];
						u32 i = 0;

						m_fileMap = (void*)new map_t();
						map_t& map = *(map_t*)m_fileMap;
						while (u7 == 0x2014b50) {
							m_archive->read(buf0, 6);
							u16 u8_ = m_archive->read<u16>();
							if (u8_ != 0 && u8_ != 8) {
								// some kind of error
							}

							m_archive->read(buf0, 8);
							u32 u9 = m_archive->read<u32>();
							u32 u10 = m_archive->read<u32>();
							u16 name_len = m_archive->read<u16>();
							u16 u12 = m_archive->read<u16>();
							u16 u13 = m_archive->read<u16>();
							m_archive->read(buf0, 8);
							u32 u14 = m_archive->read<u32>();
							m_archive->read(name, name_len);
							m_archive->read(buf2, u12);
							m_archive->read(buf3, u13);
							name[name_len] = 0;

							m_files[i].offset = u14 + name_len + 30;
							m_files[i].decompressed_sz = u10;
							m_files[i].compressed_sz = u9;
							map[name] = i;
							i++;

							u7 = m_archive->read<u32>();
						}
					}
				}
			}
		} catch (const std::exception& e) {
			if (m_files) {
				delete [] m_files;
				m_files = nullptr;
			}
			if (m_archive) {
				delete m_archive;
				m_archive = nullptr;
			}
			if (m_fileMap) {
				delete (map_t*)m_fileMap;
				m_fileMap = nullptr;
			}
			throw e;
		}
    }

    GameArchive::~GameArchive() {
		if (next) delete next;
		g_Archives = nullptr;

		if (m_files) {
			delete [] m_files;
			m_files = nullptr;
		}
		if (m_archive) {
			delete m_archive;
			m_archive = nullptr;
		}
		if (m_fileMap) {
			delete (map_t*)m_fileMap;
			m_fileMap = nullptr;
		}
    }

    Data* GameArchive::open(const char* path) {
		std::string filename = path;
		std::transform(filename.begin(), filename.end(), filename.begin(), std::tolower);
		map_t& map = *(map_t*)m_fileMap;

		auto it = map.find(filename);
		if (it == map.end()) return nullptr;
		entry& e = m_files[it->second];
		m_archive->position(e.offset);

		if (e.compressed_sz == e.decompressed_sz) {
			return m_archive->isolate(e.compressed_sz);
		}

		u8* compressed = new u8[e.compressed_sz];
		m_archive->read(compressed, e.compressed_sz);

		u8* decompressed = new u8[e.decompressed_sz];

		z_stream stream;
		stream.zalloc = [](voidpf opaque, uInt items, uInt size) {
			return (voidpf)new u8[size_t(items) * size_t(size)];
		};
		stream.zfree = [](voidpf opaque, voidpf address) {
			delete [] (u8*)address;
		};
		stream.opaque = Z_NULL;
		stream.avail_in = e.compressed_sz;
		stream.next_in = compressed;
		stream.avail_out = e.decompressed_sz;
		stream.next_out = decompressed;

		if (inflateInit2(&stream, -MAX_WBITS) != Z_OK) {
			delete[] compressed;
			delete[] decompressed;
			throw std::exception(format("Failed to initialize zlib stream for file '%s'", filename.c_str()).c_str());
			return nullptr;
		}
		i32 status = inflate(&stream, Z_NO_FLUSH);
		if (status != Z_OK && status != Z_STREAM_END) {
			delete[] compressed;
			delete[] decompressed;
			throw std::exception(format("Failed to inflate file '%s'", filename.c_str()).c_str());
			return nullptr;
		}

		if (inflateEnd(&stream) != Z_OK) {
			// r2Warn("Call to inflateEnd failed for file '%s'", filename.c_str());
		}

		delete [] compressed;

		Data* out = new Data(decompressed, e.decompressed_sz, false);
		out->take_ownership();
		return out;
    }

	Data* GameArchive::open(const char* directory, const char* filename, const char* extension) {
		std::string s = directory;
		s += "/";
		s += filename;
		s += "/";
		s += extension;
		return open(s.c_str());
	}

	void LoadArchives() {
		new GameArchive("./sr.zip");
		new GameArchive("./ramshop.zip");
	}
};
