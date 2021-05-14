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
};