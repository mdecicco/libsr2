#include <libsr2/utilities/utils.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/stream.h>
#include <stdarg.h>

namespace sr2 {
    std::string format(const char* fmt, ...) {
        static char buf[2048];
        va_list args;
        va_start(args, fmt);
        i32 count = vsnprintf(buf, 2048, fmt, args);
        buf[count] = 0;
        va_end(args);
        return buf;
    }

    char* duplicate_string(const char* str) {
        if (!str) return nullptr;

        size_t len = strlen(str);
        
        if (len > 0) {
            char* out = new char[len + 1];
            memcpy(out, str, len + 1);
            return out;
        }

        return nullptr;
    }

    bool IsPivotPrefix(const char* directory, const char* filename, const char* partname) {
        char buf[64] = { 0 };
        snprintf(buf, 64, "%s_%s", filename, partname);
        return datAssetManager::exists(directory, filename, "mtx");
    }

    bool IsPivot(const char* filename, const char* partname) {
        return IsPivotPrefix("geometry", filename, partname);
    }

    bool GetPivotPrefix(mat3x4f& out, const char* directory, const char* filename, const char* partname) {
        char fullname[64] = { 0 };
        snprintf(fullname, 64, "%s_%s", filename, partname);

        Stream* fp = datAssetManager::open(directory, fullname, "mtx", 1, true);
        if (!fp) return false;

        fp->read(&out, sizeof(mat3x4f));
        delete fp;
        
        return true;
    }
    
    bool GetPivot(mat3x4f& out, const char* filename, const char* partname) {
        return GetPivotPrefix(out, "geometry", filename, partname);
    }

    i32 GetPivotCount(const char* filename, const char* partBaseName, i32 maxCount) {
        char buf[64];

        i32 i = 0;
        for (;i < maxCount;i++) {
            snprintf(buf, 64, "%s%d", partBaseName, i);
            if (!IsPivot(filename, buf)) return i;
        }

        return i;
    }
};