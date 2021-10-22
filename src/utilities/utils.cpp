#include <libsr2/utilities/utils.h>
#include <libsr2/types.h>
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
};