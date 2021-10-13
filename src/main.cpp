#include <libsr2/libsr2.h>
#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/argParser.h>
#include <exception>
#include <stdio.h>

// 0x001001f8 
int main(int argc, char** argv) {
    // undefined auStack32 [16];
    try {
        // from a higher function, but here is good enough
        sr2::datArgParser::Init(argc, argv);

        // QuickTicks calls are likely a relic from profiling code which was
        // not compiled in the game's build. Function simply returns the value
        // of the Count register and is totally ineffectual.

        // Utils::Timer::QuickTicks();
        sr2::LoadArchives();
        // Utils::Timer::QuickTicks();
        
        // Utils::Timer::QuickTicks();
        // FUN_00101958((long)(int)auStack32); <- Constructs some weird object with a virtual function that creates the game engine, is totally unused
        sr2::GameEngine::Create(argc, argv);
        // Utils::Timer::QuickTicks();

        // Utils::Timer::QuickTicks();
        sr2::GameEngine::Execute();
        sr2::GameEngine::Terminate();
        sr2::datArgParser::Kill();
    } catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}