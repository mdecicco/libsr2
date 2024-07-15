#pragma once
#include <libsr2/types.h>
#include <libsr2/utilities/HashTable.h>

namespace sr2 {
    class datArgParser {
        public:
            datArgParser();
            ~datArgParser();

            static void Init(int argc, char** argv);
            static void Kill();
            static void InitFromMem(char* args);
            static bool GetBooleanArgument(const char* arg);
            static bool GetIntegerArgument(const char* arg, i32 index, i32* dest);
            static bool GetFloatArgument(const char* arg, i32 index, f32* dest);
            static bool GetStringArgument(const char* arg, i32 index, const char** dest);
            static void AddReplace(const char* arg, const char* value);
            static void Remove(const char* arg);

            static i32 argc;
            static char** argv;
            static HashTable argTable;
    };
};