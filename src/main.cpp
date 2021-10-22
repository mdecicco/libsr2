#include <libsr2/libsr2.h>
#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/argParser.h>
#include <exception>
#include <stdio.h>

void debug_testing();

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

        debug_testing();

        // Utils::Timer::QuickTicks();
        sr2::GameEngine::Execute();
        sr2::GameEngine::Terminate();
        sr2::datArgParser::Kill();
    } catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}


#include <libsr2/io/datParser.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/stream.h>
#include <libsr2/vehicle/vehWheel.h>
void debug_testing() {
    sr2::datParser p("vehCarSim");
    sr2::vec3f ibox;
    sr2::vehWheel wheel;
    p.add(sr2::PARSE_TYPE::VEC3F, "InertiaBox", &ibox, 1, 0);

    sr2::datParser* wh_p = p.add_parser("WheelFront", nullptr);
    wheel.prepare_parser(wh_p);
    p.load("C:\\Users\\Computer\\RevEng\\sr2\\sr\\tune\\vehicle\\", "vpsuv2", "vehcarsim");

    return;
}