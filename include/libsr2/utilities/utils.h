#include <libsr2/types.h>
#include <string>

namespace sr2 {
    std::string format(const char* fmt, ...);
    char* duplicate_string(const char* str);
    bool IsPivotPrefix(char* directory, char* filename, char* partname);
    bool IsPivot(char* filename, char* partname);
    bool GetPivotPrefix(mat3x4f& out, char* directory, char* filename, char* partname);
    bool GetPivot(mat3x4f& out, char* filename, char* partname);
    i32 GetPivotCount(char* filename, char* partBaseName, i32 count);
};