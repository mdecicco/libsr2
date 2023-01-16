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
#include <libsr2/managers/missManager.h>
#include <libsr2/utilities/argParser.h>
void mission_test() {
    const char* missions[] = {
        "Afghan_AAAcruise.sp",
        "Afghan_AirConvoy02.sp",
        "Afghan_AirDrop01.sp",
        "Afghan_BombTag01.mp",
        "Afghan_BombTag02.mp",
        "Afghan_BombTag03.mp",
        "Afghan_BombTag04.mp",
        "Afghan_BombTag05.mp",
        "Afghan_CNS05.mp",
        "Afghan_CNS05Free.mp",
        "Afghan_CNS07.mp",
        "Afghan_CNS08.mp",
        "Afghan_CNS09.mp",
        "Afghan_CNS09_SP.sp",
        "Afghan_Checkpoint01.mp",
        "Afghan_Checkpoint02.mp",
        "Afghan_Checkpoint03.mp",
        "Afghan_Checkpoint04.mp",
        "Afghan_Checkpoint05.mp",
        "Afghan_Cruise.mp",
        "Afghan_FollowLeader01.sp",
        "Afghan_LootGrab01.mp",
        "Afghan_LootGrab02.mp",
        "Afghan_LootGrab03.mp",
        "Afghan_LootGrab04.mp",
        "Afghan_LootGrab05.mp",
        "Afghan_LootGrab05SP.sp",
        "Afghan_PuDo03.sp",
        "Afghan_PuDo05.sp",
        "Afghan_PuDo10.sp",
        "Afghan_PuDo12.sp",
        "Afghan_PuDo13.sp",
        "Afghan_Unordered03.sp",
        "Afghan_VehicleAssault02a.sp",
        "Afghan_bonus_pudo01.mp",
        "Afghan_snow_AAACruise.mp",
        "Afghan_snow_AirConvoy03.sp",
        "Afghan_snow_AirDrop01.sp",
        "Afghan_snow_BombRun01.sp",
        "Afghan_snow_BombTag01.mp",
        "Afghan_snow_BombTag02.mp",
        "Afghan_snow_BombTag03.mp",
        "Afghan_snow_BombTag03a.mp",
        "Afghan_snow_BombTag04.mp",
        "Afghan_snow_BombTag05.mp",
        "Afghan_snow_CNS05_SP.sp",
        "Afghan_snow_CNS06.mp",
        "Afghan_snow_CNS07.mp",
        "Afghan_snow_CNS07_backup.mp",
        "Afghan_snow_CNS07_old.mp",
        "Afghan_snow_CNS08_SP.sp",
        "Afghan_snow_CNS09.mp",
        "Afghan_snow_CNS10.mp",
        "Afghan_snow_CNS10_backup.mp",
        "Afghan_snow_CNS11.mp",
        "Afghan_snow_Checkpoint01.mp",
        "Afghan_snow_Checkpoint02.mp",
        "Afghan_snow_Checkpoint03.mp",
        "Afghan_snow_Checkpoint04.mp",
        "Afghan_snow_Checkpoint05.mp",
        "Afghan_snow_FollowLeader02.sp",
        "Afghan_snow_LootGrab01.mp",
        "Afghan_snow_LootGrab02.mp",
        "Afghan_snow_LootGrab02_SP.sp",
        "Afghan_snow_LootGrab03.mp",
        "Afghan_snow_LootGrab04.mp",
        "Afghan_snow_LootGrab05.mp",
        "Afghan_snow_PuDo01.sp",
        "Afghan_snow_PuDo07.sp",
        "Afghan_snow_PuDo09.sp",
        "Afghan_snow_PuDo14.sp",
        "Afghan_snow_Unordered01.sp",
        "Afghan_snow_VehicleAssault01a.sp",
        "Afghan_snow_VehicleAssault03.sp",
        "Afghan_snow_bonus_pudo01.mp",
        "Afghan_snow_testtest.sp",
        "Vietnam_AAAcruise.sp",
        "Vietnam_AirConvoy01.sp",
        "Vietnam_AirConvoy01b.sp",
        "Vietnam_AirDrop01.sp",
        "Vietnam_AirDrop02.sp",
        "Vietnam_BombRun01.sp",
        "Vietnam_BombTag01.mp",
        "Vietnam_BombTag02.mp",
        "Vietnam_BombTag03.mp",
        "Vietnam_BombTag04.mp",
        "Vietnam_BombTag05.mp",
        "Vietnam_CNS01.mp",
        "Vietnam_CNS01_AI_test.mp",
        "Vietnam_CNS01t.mp",
        "Vietnam_CNS02.mp",
        "Vietnam_CNS02t.mp",
        "Vietnam_CNS03.mp",
        "Vietnam_CNS03t.mp",
        "Vietnam_CNS03t_SP.sp",
        "Vietnam_CNS04.mp",
        "Vietnam_CNS04t.mp",
        "Vietnam_CNS05.mp",
        "Vietnam_CNS05t.mp",
        "Vietnam_CNSaFree.mp",
        "Vietnam_Checkpoint01.mp",
        "Vietnam_Checkpoint02.mp",
        "Vietnam_Checkpoint03.mp",
        "Vietnam_Checkpoint04.mp",
        "Vietnam_Checkpoint05.mp",
        "Vietnam_Convoy01.sp",
        "Vietnam_Cruise.mp",
        "Vietnam_CruiseMarc.sp",
        "Vietnam_FollowLeader01.sp",
        "Vietnam_LTaTeam.mp",
        "Vietnam_LootGrab01.mp",
        "Vietnam_LootGrab02.mp",
        "Vietnam_LootGrab03.mp",
        "Vietnam_LootGrab04.mp",
        "Vietnam_LootGrab04_SP.sp",
        "Vietnam_LootGrab05.mp",
        "Vietnam_MissionA.sp",
        "Vietnam_MissionB.sp",
        "Vietnam_MissionC.sp",
        "Vietnam_MissionCNS01.sp",
        "Vietnam_MissionCNS02.sp",
        "Vietnam_MissionCNS03.sp",
        "Vietnam_MissionCNS04.sp",
        "Vietnam_MissionCNS05.sp",
        "Vietnam_MissionD.sp",
        "Vietnam_MissionE.sp",
        "Vietnam_MissionF.sp",
        "Vietnam_MissionG.sp",
        "Vietnam_MissionTest01.sp",
        "Vietnam_MissionTest02.sp",
        "Vietnam_MissionTest03.sp",
        "Vietnam_MissionTest04.sp",
        "Vietnam_MissionTest05.sp",
        "Vietnam_MissionTest06.sp",
        "Vietnam_MissionTest07.sp",
        "Vietnam_MissionTest08.sp",
        "Vietnam_PUDO01.sp",
        "Vietnam_PUDO02.sp",
        "Vietnam_PUDO03.sp",
        "Vietnam_PUDO03CS.sp",
        "Vietnam_PUDO03CStemp.sp",
        "Vietnam_PUDO04.sp",
        "Vietnam_ScrnShtTemple.sp",
        "Vietnam_ScrnShtTemple02.sp",
        "Vietnam_ScrnShtTemple03.sp",
        "Vietnam_ScrnShtTown.sp",
        "Vietnam_ScrnShtVillage.sp",
        "Vietnam_Test.sp",
        "Vietnam_TestCheckpoint.mp",
        "Vietnam_TestConvoy.sp",
        "Vietnam_TestConvoy01.sp",
        "Vietnam_TestConvoy02.sp",
        "Vietnam_TestConvoy02a.sp",
        "Vietnam_TestGetaway.sp",
        "Vietnam_TestHill.sp",
        "Vietnam_TestProximity.mp",
        "Vietnam_TestTimed.sp",
        "Vietnam_Unordered01.sp",
        "Vietnam_VehicleAssault01.sp",
        "Vietnam_bonus_pudo01.mp",
        "afghan_snow_Cruise.mp",
        nullptr
    };

    const char** n = missions;
    while (*n) {
        std::string str = *n;
        std::string map, mission;
        sr2::u32 splitIdx = str.find_first_of('_');
        map = str.substr(0, splitIdx);
        mission = str.substr(splitIdx + 1);

        printf("Loading mission: %s.\n", *n);
        sr2::missManager::create();
        bool result = sr2::missManager::get()->load(map, mission);
        if (!result) printf("    Apparently it failed.\n");
        result = sr2::missManager::get()->loadScoring();
        if (!result) printf("    Scoring table failed to load.\n");
        sr2::missManager::get()->debugPrintState(false);
        sr2::missManager::destroy();
        n++;
    }
}

void single_mission_test() {
    sr2::missManager::create();
    // bool result = sr2::missManager::get()->load("Vietnam", "Unordered01.sp"); // <- the tower one
    bool result = sr2::missManager::get()->load("Afghan", "VehicleAssault02a.sp");
    if (!result) printf("    Apparently it failed.\n");
    result = sr2::missManager::get()->loadScoring();
    if (!result) printf("    Scoring table failed to load.\n");
    sr2::missManager::get()->debugPrintState(false);
    sr2::missManager::destroy();
}

void vehicle_test() {
    sr2::datParser p("vehCarSim");
    sr2::vec3f ibox;
    sr2::vehWheel wheel;
    p.add(sr2::PARSE_TYPE::VEC3F, "InertiaBox", &ibox, 1, 0);
    sr2::datParser* wh_p = p.add_parser("WheelFront", nullptr);
    wheel.prepare_parser(wh_p);
    p.load("C:\\Users\\miguel\\reverse_engineering\\sr2\\sr\\tune\\vehicle\\", "vpsuv2", "vehcarsim");

}

void debug_testing() {
    sr2::datArgParser::AddReplace("level", "Afghan");
    sr2::datArgParser::AddReplace("humans", "1");
    sr2::datArgParser::AddReplace("mission", "Cruise.mp");

    // mission_test();
    // vehicle_test();
    return;
}