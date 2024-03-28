#include <libsr2/types.h>
#include <string>

namespace sr2 {
    std::string format(const char* fmt, ...);
    char* duplicate_string(const char* str);
    bool IsPivotPrefix(const char* directory, const char* filename, const char* partname);
    bool IsPivot(const char* filename, const char* partname);
    bool GetPivotPrefix(mat3x4f& out, const char* directory, const char* filename, const char* partname);
    bool GetPivot(mat3x4f& out, const char* filename, const char* partname);
    i32 GetPivotCount(const char* filename, const char* partBaseName, i32 count);
};