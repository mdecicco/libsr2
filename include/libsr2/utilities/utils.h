#include <libsr2/types.h>
#include <string>

namespace sr2 {
    std::string format(const char* fmt, ...);
    char* duplicate_string(const char* str);
};