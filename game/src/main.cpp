#include <libsr2/libsr2.h>
#include <libsr2/utilities/datArgParser.h>
#include <libsr2/utilities/GameArchive.h>

#include <utils/Allocator.hpp>
#include <utils/Singleton.hpp>

#include <exception>
#include <stdio.h>

using namespace sr2;

int main(int argc, char** argv) {
    utils::Mem::Create();

    try {
        datArgParser::Init(argc, argv);

        LoadArchives();

        GameEngine::Create(argc, argv);
        GameEngine::Execute();
        GameEngine::Terminate();
        datArgParser::Kill();
    } catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    utils::Mem::Destroy();
    return 0;
}