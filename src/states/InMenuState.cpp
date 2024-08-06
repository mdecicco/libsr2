#include <libsr2/states/InMenuState.h>
#include <libsr2/states/gameFSM.h>
#include <libsr2/ui/srui2Bitmap.h>
#include <libsr2/gfx/gfx.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/managers/Aud3DObjectManager.h>
#include <libsr2/managers/audManager.h>
#include <libsr2/utilities/datArgParser.h>
#include <libsr2/utilities/msgMsgSource.h>
#include <libsr2/globals.h>
#include <libsr2/libsr2.h>

#include <libsr2/frontend/srfeMainMenu.h>
#include <libsr2/frontend/srfeBombTag.h>
#include <libsr2/frontend/srfeCheckpointRace.h>
#include <libsr2/frontend/srfeCNS.h>
#include <libsr2/frontend/srfeJoyriding.h>
#include <libsr2/frontend/srfeLootGrab.h>
#include <libsr2/frontend/srfeTurfWars.h>
#include <libsr2/frontend/srfeLoadOrNew.h>
#include <libsr2/frontend/srfeMissionSelect.h>
#include <libsr2/frontend/srfeNameInput.h>
#include <libsr2/frontend/srfeSavegameSelect.h>
#include <libsr2/frontend/srfePlayerSel.h>
#include <libsr2/frontend/srfeVehicleSel.h>
#include <libsr2/frontend/srfeVehicleSel2Pl.h>
#include <libsr2/frontend/srfeOptions.h>
#include <libsr2/frontend/srfeControllerSetup.h>
#include <libsr2/frontend/srfeMemcardManager.h>
#include <libsr2/frontend/srfeLanguageInGame.h>
#include <libsr2/frontend/srfeExtras.h>
#include <libsr2/frontend/srfeCredits.h>
#include <libsr2/frontend/srfeTheater.h>
#include <libsr2/frontend/srfeConceptArt.h>
#include <libsr2/frontend/srfeBonusMissions.h>
#include <libsr2/frontend/srfeHiScoresMain.h>
#include <libsr2/frontend/srfeHiScoresSmuggler.h>
#include <libsr2/frontend/srfeHiScoresCNS.h>
#include <libsr2/frontend/srfeHiScoresLootGrab.h>
#include <libsr2/frontend/srfeHiScoresBombTag.h>
#include <libsr2/frontend/srfeHiScoresCheckpointRace.h>
#include <libsr2/frontend/srfeHiScoresBonus.h>
#include <libsr2/frontend/srfeLoading.h>
#include <libsr2/frontend/srfeTitlescreen.h>
#include <libsr2/frontend/srfeRockstarLogoMovie.h>
#include <libsr2/frontend/srfeAngelLogoMovie.h>
#include <libsr2/frontend/srfeIntroMovie.h>
#include <libsr2/frontend/srfeUIIntroMovie.h>
#include <libsr2/frontend/srfeCareer1Movie.h>
#include <libsr2/frontend/srfeCareer2Movie.h>
#include <libsr2/frontend/srfeCareer3Movie.h>
#include <libsr2/frontend/srfeCareer4Movie.h>
#include <libsr2/frontend/srfeCareer5Movie.h>
#include <libsr2/frontend/srfeCareer6Movie.h>
#include <libsr2/frontend/srfeCareer7Movie.h>
#include <libsr2/frontend/srfeCareer8Movie.h>
#include <libsr2/frontend/srfeCareer9Movie.h>
#include <libsr2/frontend/srfeSRNewsMovie.h>
#include <libsr2/frontend/srfeMemcardCheck.h>
#include <libsr2/frontend/srfeLanguageSelectAtBoot.h>

#include <render/utils/SimpleDebugDraw.h>
#include <render/utils/ImGui.h>

#include <assert.h>

namespace sr2 {
    MenuUnk0::MenuUnk0() {
        gameMode = GAME_MODE::Cruise;
        mapId = MAP_SELECTION::Afghan;
        missionIdx = 0;
    }

    bool MenuUnk0::isMapUnknown() {
        return mapId == MAP_SELECTION::UnknownMap;
    }

    const char* MenuUnk0::getMapName() {
        switch (mapId) {
            case MAP_SELECTION::Afghan: return "Afghan";
            case MAP_SELECTION::Vietnam: return "Vietnam";
            case MAP_SELECTION::AfghanSnow: return "afghan_snow";
            default: return nullptr;
        }
    }

    const char* MenuUnk0::getGameModeName() {
        switch (gameMode) {
            case GAME_MODE::Cruise: return "Cruise";
            case GAME_MODE::Checkpoint: return "Checkpoint";
            case GAME_MODE::CrooksAndSmugglers: return "CNS";
            case GAME_MODE::LootGrab: return "LootGrab";
            case GAME_MODE::BombTag: return "BombTag";
            case GAME_MODE::Career: return "Career";
            default: return "";
        }
    }

    const char* MenuUnk0::getMissionName() {
        const char* mapName = getMapName();

        switch (gameMode) {
            case GAME_MODE::Checkpoint: return getCheckpointMissionName(mapName, missionIdx);
            case GAME_MODE::CrooksAndSmugglers: return getCNSMissionName(mapName, missionIdx);
            case GAME_MODE::LootGrab: return getLootgrabMissionName(mapName, missionIdx);
            case GAME_MODE::BombTag: return getBombTagMissionName(mapName, missionIdx);
            case GAME_MODE::Career: return getCareerMissionName(mapName, missionIdx);
            default: return "Cruise.mp";
        }
    }

    u32 MenuUnk0::getMissionCountForMode() {
        const char* mapName = getMapName();
        switch (gameMode) {
            case GAME_MODE::Checkpoint: return getCheckpointMissionCount(mapName);
            case GAME_MODE::CrooksAndSmugglers: return getCNSMissionCount(mapName);
            case GAME_MODE::LootGrab: return getLootgrabMissionCount(mapName);
            case GAME_MODE::BombTag: return getBombTagMissionCount(mapName);
            case GAME_MODE::Career: return getCareerMissionCount(mapName);
            default: return 1;
        }
    }

    bool MenuUnk0::advanceToNext() {
        if (isMapUnknown()) return false;

        missionIdx++;
        if (missionIdx < getMissionCountForMode()) return true;
        
        missionIdx = 0;
        gameMode = GAME_MODE(u32(gameMode) + 1);
        if (gameMode <= u32(GAME_MODE::Career)) return true;
        
        gameMode = GAME_MODE::Cruise;
        mapId = MAP_SELECTION(u32(mapId) + 1);

        return !isMapUnknown();
    }

    const char* MenuUnk0::getCheckpointMissionName(const char* mapName, u32 missionIdx) {
        switch (missionIdx) {
            case 0: return "checkpoint01.mp";
            case 1: return "checkpoint02.mp";
            case 2: return "checkpoint03.mp";
            case 3: return "checkpoint04.mp";
            case 4: return "checkpoint05.mp";
            default: return nullptr;
        }
    }
    
    const char* MenuUnk0::getCNSMissionName(const char* mapName, u32 missionIdx) {
        if (strcmp(mapName, "Afghan") == 0) {
            switch (missionIdx) {
                case 0: return "cns09.mp";
                case 1: return "cns05.mp";
                case 2: return "cns07.mp";
                case 3: return "cns05free.mp";
                case 4: return "cns08.mp";
                default: return nullptr;
            }
        }

        if (strcmp(mapName, "Vietnam") == 0) {
            switch (missionIdx) {
                case 0: return "cns01.mp";
                case 1: return "cns05t.mp";
                case 2: return "cns03.mp";
                case 3: return "cns02.mp";
                case 4: return "cns03t.mp";
                default: return nullptr;
            }
        }

        if (strcmp(mapName, "afghan_snow") == 0) {
            switch (missionIdx) {
                case 0: return "cns06.mp";
                case 1: return "cns09.mp";
                case 2: return "cns11.mp";
                case 3: return "cns10.mp";
                case 4: {
                    // Weird jump table, might actually be "cns09.mp"... See switch statement at0x0014dc0c
                    return "cns07.mp";
                }
                default: return nullptr;
            }
        }

        assert(false);
        return "";
    }

    const char* MenuUnk0::getLootgrabMissionName(const char* mapName, u32 missionIdx) {
        switch (missionIdx) {
            case 0: return "lootgrab01.mp";
            case 1: return "lootgrab02.mp";
            case 2: return "lootgrab03.mp";
            case 3: return "lootgrab04.mp";
            case 4: return "lootgrab05.mp";
            default: return nullptr;
        }
    }

    const char* MenuUnk0::getBombTagMissionName(const char* mapName, u32 missionIdx) {
        switch (missionIdx) {
            case 0: return "bombtag01.mp";
            case 1: return "bombtag02.mp";
            case 2: return "bombtag03.mp";
            case 3: return "bombtag04.mp";
            case 4: return "bombtag05.mp";
            default: return nullptr;
        }
    }

    const char* MenuUnk0::getCareerMissionName(const char* mapName, u32 missionIdx) {
        if (strcmp(mapName, "Afghan") == 0) {
            switch (missionIdx) {
                case 0: return "unordered03.sp";
                case 1: return "pudo12.sp";
                case 2: return "FollowLeader01.sp";
                case 3: return "Pudo05.sp";
                case 4: return "AirConvoy02.sp";
                case 5: return "CNS09_SP.sp";
                case 6: return "PuDo03.sp";
                case 7: return "AirDrop01.sp";
                case 8: return "LootGrab05SP.sp";
                case 9: return "VehicleAssault02a.sp";
                case 10: return "PuDo10.sp";
                case 11: return "PuDo13.sp";
                default: assert(false);
            }
        }

        if (strcmp(mapName, "Vietnam") == 0) {
            switch (missionIdx) {
                case 0: return "FollowLeader01.sp";
                case 1: return "PuDo01.sp";
                case 2: return "Unordered01.sp";
                case 3: return "AirDrop02.sp";
                case 4: return "LootGrab04_SP.sp";
                case 5: return "AirConvoy01.sp";
                case 6: return "PuDo04.sp";
                case 7: return "Convoy01.sp";
                case 8: return "PuDo03.sp"; // <-- weird jump table... todo: check this
                case 9: return "BombRun01.sp";
                case 10: return "CNS03t_SP.sp";
                case 11: return "VehicleAssault01.sp";
                case 12: return "PuDo02.sp";
                default: assert(false);
            }
        }

        if (strcmp(mapName, "afghan_snow") == 0) {
            switch (missionIdx) {
                case 0: return "PuDo07.sp";
                case 1: return "Pudo01.sp";
                case 2: return "CNS05_SP.sp";
                case 3: return "FollowLeader01.sp"; // <-- weird jump table... todo: check this
                case 4: return "VehicleAssault01a.sp";
                case 5: return "LootGrab02_SP.sp";
                case 6: return "Unordered01.sp";
                case 7: return "AirConvoy03.sp";
                case 8: return "PuDo09.sp";
                case 9: return "FollowLeader02.sp";
                case 10: return "CNS08_SP.sp";
                case 11: return "PuDo14.sp";
                case 12: return "BombRun01.sp";
                case 13: return "VehicleAssault03.sp";
                default: assert(false);
            }
        }

        assert(false);
        return "";
    }

    u32 MenuUnk0::getCheckpointMissionCount(const char* mapName) {
        return 5;
    }

    u32 MenuUnk0::getCNSMissionCount(const char* mapName) {
        return 5;
    }

    u32 MenuUnk0::getLootgrabMissionCount(const char* mapName) {
        return 5;
    }

    u32 MenuUnk0::getBombTagMissionCount(const char* mapName) {
        return 5;
    }

    u32 MenuUnk0::getCareerMissionCount(const char* mapName) {
        if (strcmp(mapName, "Afghan") == 0) return 12;
        if (strcmp(mapName, "Vietnam") == 0) return 13;
        if (strcmp(mapName, "afghan_snow") == 0) return 14;

        assert(false);
        return 0;
    }




    InMenuState::InMenuState() {
        m_justBooted = true;
        m_uiInterestIndex = 0;
        m_state = 0;
    }

    InMenuState::~InMenuState() {
        delete m_widgets;
    }

    void InMenuState::Enter(GameEngine* eng) {
        Reset(eng);

        // FUN_002ed360(0, 0);
        // FUN_002ed360(0, 1);

        m_widgets = new MenuWidgets();
        m_frontendBase = new ui2Base("FE-base");

        // AllocSomeGlobalString();

        displayUIInterest();
        LANGUAGE lang;
        u32 strFlags;
        m_frontendBase->getLangInfo(&lang, &strFlags);
        m_frontendBase->loadStrings("frontend.strtbl", lang, 0xffffffff);

        ui2Movie::FUN_001f29f0(gfx::pipeline::iWidth, gfx::pipeline::iHeight);

        m_frontendBase->setDirectory("tune/ui/NTSC");
        char vidPath[32] = { 0 };
        snprintf(vidPath, 32, "%svid/NTSC/", datAssetManager::getPath());
        // FUN_001f3408(local_120);
        // FUN_001f3440(local_120);
        audManager::loadSoundBanks("aud/fedata", lang != LANGUAGE::German ? 1 : 2);

        // srui2VehicleDisplay::Initialize();

        m_widgets->quitfe_var = new ui2Variable("quitfe_var", 50, nullptr);
        m_widgets->quitfe_var->setIntegerData(0);

        // FUN_0015efb8();

        m_widgets->carPl1_var = new ui2Variable("carPl1_var", 50, nullptr);
        m_widgets->carMate1Pl1_var = new ui2Variable("carMate1Pl1_var", 50, nullptr);
        m_widgets->carMate2Pl1_var = new ui2Variable("carMate2Pl1_var", 50, nullptr);
        m_widgets->carMate3Pl1_var = new ui2Variable("carMate3Pl1_var", 50, nullptr);
        m_widgets->carPl2_var = new ui2Variable("carPl2_var", 50, nullptr);
        m_widgets->carMate1Pl2_var = new ui2Variable("carMate1Pl2_var", 50, nullptr);
        m_widgets->carMate2Pl2_var = new ui2Variable("carMate2Pl2_var", 50, nullptr);
        m_widgets->carMate3Pl2_var = new ui2Variable("carMate3Pl2_var", 50, nullptr);
        m_widgets->level_var = new ui2Variable("level_var", 50, nullptr);
        m_widgets->mission_var = new ui2Variable("mission_var", 50, nullptr);
        m_widgets->skin1_var = new ui2Variable("skin1_var", 50, nullptr);
        m_widgets->skin2_var = new ui2Variable("skin2_var", 50, nullptr);
        m_widgets->humans_var = new ui2Variable("humans_var", 50, nullptr);
        m_widgets->timeOfDay_var = new ui2Variable("timeOfDay_var", 50, nullptr);
        m_widgets->police_var = new ui2Variable("police_var", 50, nullptr);
        m_widgets->team_game_var = new ui2Variable("team_game_var", 50, nullptr);
        m_widgets->transmission1_var = new ui2Variable("transmission1_var", 50, nullptr);
        m_widgets->transmission2_var = new ui2Variable("transmission2_var", 50, nullptr);
        m_widgets->regularStartupScreen_var = new ui2Variable("regularStartupScreen_var", 50, nullptr);
        m_widgets->videoStartupScreen_var = new ui2Variable("videoStartupScreen_var", 50, nullptr);


        m_widgets->MainMenu = new srfeMainMenu("MainMenu", nullptr);
        m_widgets->BombTag = new srfeBombTag("BombTag", nullptr);
        m_widgets->CheckpointRace = new srfeCheckpointRace("CheckpointRace", nullptr);
        m_widgets->CNS = new srfeCNS("CNS", nullptr);
        m_widgets->Joyriding = new srfeJoyriding("Joyriding", nullptr);
        m_widgets->LootGrab = new srfeLootGrab("LootGrab", nullptr);
        m_widgets->TurfWars = new srfeTurfWars("TurfWars", nullptr);
        m_widgets->LoadOrNew = new srfeLoadOrNew("LoadOrNew", nullptr);
        m_widgets->MissionSelect = new srfeMissionSelect("MissionSelect", nullptr);
        m_widgets->NameInput = new srfeNameInput("NameInput", nullptr);
        m_widgets->SavegameSelect = new srfeSavegameSelect("SavegameSelect", nullptr);

        m_widgets->PlayerSelCPR = new srfePlayerSel("PlayerSelCPR", "CheckpointRace", "MainMenu", 1, nullptr);
        m_widgets->PlayerSelJoyriding = new srfePlayerSel("PlayerSelJoyriding", "Joyriding", "MainMenu", 0, nullptr);
        m_widgets->PlayerSelTurfWars = new srfePlayerSel("PlayerSelTurfWars", "TurfWars", "MainMenu", 1, nullptr);
        m_widgets->PlayerSelBonus = new srfePlayerSel("PlayerSelBonus", "BonusMissions", "Extras", 1, nullptr);
        
        m_widgets->VehicleSelBonus = new srfeVehicleSel("VehicleSelBonus", "placeholder", "BonusMissions", nullptr);
        m_widgets->VehicleSelBombTag = new srfeVehicleSel("VehicleSelBombTag", "placeholder", "BombTag", nullptr);
        m_widgets->VehicleSelCheckpointRace = new srfeVehicleSel("VehicleSelCheckpointRace", "placeholder", "CheckpointRace", nullptr);
        m_widgets->VehicleSelCNS = new srfeVehicleSel("VehicleSelCNS", "placeholder", "CNS", nullptr);
        m_widgets->VehicleSelJoyriding = new srfeVehicleSel("VehicleSelJoyriding", "placeholder", "Joyriding", nullptr);
        m_widgets->VehicleSelLootGrab = new srfeVehicleSel("VehicleSelLootGrab", "placeholder", "LootGrab", nullptr);
        m_widgets->VehicleSelSmugglerM = new srfeVehicleSel("VehicleSelSmugglerM", "placeholder", "MissionSelect", nullptr);

        m_widgets->VehicleSel2PlBombTag = new srfeVehicleSel2Pl("VehicleSel2PlBombTag", "placeholder", "BombTag", nullptr);
        m_widgets->VehicleSel2PlCheckpointRace = new srfeVehicleSel2Pl("VehicleSel2PlCheckpointRace", "placeholder", "CheckpointRace", nullptr);
        m_widgets->VehicleSel2PlCNS = new srfeVehicleSel2Pl("VehicleSel2PlCNS", "placeholder", "CNS", nullptr);
        m_widgets->VehicleSel2PlJoyriding = new srfeVehicleSel2Pl("VehicleSel2PlJoyriding", "placeholder", "Joyriding", nullptr);
        m_widgets->VehicleSel2PlLootGrab = new srfeVehicleSel2Pl("VehicleSel2PlLootGrab", "placeholder", "LootGrab", nullptr);
        m_widgets->VehicleSel2PlBonus = new srfeVehicleSel2Pl("VehicleSel2PlBonus", "placeholder", "BonusMissions", nullptr);

        m_widgets->Options = new srfeOptions("Options", nullptr);
        m_widgets->ControllerSetup = new srfeControllerSetup("ControllerSetup", nullptr);
        m_widgets->MemcardManager = new srfeMemcardManager("MemcardManager", nullptr);
        m_widgets->LanguageInGame = new srfeLanguageInGame("LanguageInGame", nullptr);
        m_widgets->Extras = new srfeExtras("Extras", nullptr);
        m_widgets->Credits = new srfeCredits("Credits", nullptr);
        m_widgets->Theater = new srfeTheater("Theater", nullptr);
        m_widgets->ConceptArt = new srfeConceptArt("ConceptArt", nullptr);
        m_widgets->BonusMissions = new srfeBonusMissions("BonusMissions", nullptr);
        m_widgets->HiScoresMain = new srfeHiScoresMain("HiScoresMain", nullptr);
        m_widgets->HiScoresSmugglerM = new srfeHiScoresSmuggler("HiScoresSmugglerM", nullptr);
        m_widgets->HiScoresCNS = new srfeHiScoresCNS("HiScoresCNS", nullptr);
        m_widgets->HiScoresLootGrab = new srfeHiScoresLootGrab("HiScoresLootGrab", nullptr);
        m_widgets->HiScoresBombTag = new srfeHiScoresBombTag("HiScoresBombTag", nullptr);
        m_widgets->HiScoresCheckpointRace = new srfeHiScoresCheckpointRace("HiScoresCheckpointRace", nullptr);
        m_widgets->HiScoresBonus = new srfeHiScoresBonus("HiScoresBonus", nullptr);
        m_widgets->loading = new srfeLoading("loading", nullptr);
        m_widgets->Titlescreen = new srfeTitlescreen("Titlescreen", nullptr);
        m_widgets->MovieRockStarLogo = new srfeRockstarLogoMovie("MovieRockStarLogo", nullptr);
        m_widgets->MovieAngelLogo = new srfeAngelLogoMovie("MovieAngelLogo", nullptr);
        m_widgets->MovieIntro = new srfeIntroMovie("MovieIntro", nullptr);
        m_widgets->MovieUIIntro = new srfeUIIntroMovie("MovieUIIntro", nullptr);
        m_widgets->MovieCareer_1 = new srfeCareer1Movie("MovieCareer_1", nullptr);
        m_widgets->MovieCareer_2 = new srfeCareer2Movie("MovieCareer_2", nullptr);
        m_widgets->MovieCareer_3 = new srfeCareer3Movie("MovieCareer_3", nullptr);
        m_widgets->MovieCareer_4 = new srfeCareer4Movie("MovieCareer_4", nullptr);
        m_widgets->MovieCareer_5 = new srfeCareer5Movie("MovieCareer_5", nullptr);
        m_widgets->MovieCareer_6 = new srfeCareer6Movie("MovieCareer_6", nullptr);
        m_widgets->MovieCareer_7 = new srfeCareer7Movie("MovieCareer_7", nullptr);
        m_widgets->MovieCareer_8 = new srfeCareer8Movie("MovieCareer_8", nullptr);
        m_widgets->MovieCareer_9 = new srfeCareer9Movie("MovieCareer_9", nullptr);
        m_widgets->MovieSRNews = new srfeSRNewsMovie("MovieSRNews", nullptr);
        m_widgets->MemcardCheck = new srfeMemcardCheck("MemcardCheck", nullptr);
        m_widgets->LanguageAtBoot = new srfeLanguageSelectAtBoot("LanguageAtBoot", nullptr);

        m_frontendBase->method_0x48();
        m_frontendBase->loadWidget();

        const char* humansArg = nullptr;
        if (datArgParser::GetStringArgument("humans", 0, &humansArg)) {
            if (strcmp(humansArg, "1") == 0) {
                m_widgets->humans_var->setStringData("1");
            } else if (strcmp(humansArg, "2") == 0) {
                const char* numaiArg = nullptr;
                if (datArgParser::GetStringArgument("numai", 0, &numaiArg) && strcmp(numaiArg, "0") == 0 /* && DAT_0035f2e0 != -1 */) {
                    m_widgets->humans_var->setStringData("2-1ON1");
                } else {
                    m_widgets->humans_var->setStringData("2");
                }
            } else {
                assert(false);
            }
        }
        
        // UI::GameLoadingScreen::FUN_001c54e8(&UI::GameLoadingScreen::InstanceMem,9);
        // UI::GameLoadingScreen::InstanceMem.progress_frac = 1.0;
        // UI::GameLoadingScreen::InstanceMem.progress_frac_again_for_some_reason = 1.0;
        // UI::GameLoadingScreen::FUN_001c54f0(&UI::GameLoadingScreen::InstanceMem,0.5);

        const char* someWidgetName = m_regularStartupScreenName;
        if (false /* DAT_0035f278 */) someWidgetName = m_videoStartupScreenName;

        WidgetRef<ui2Screen> screen = ui2Base::getGlobalMaster()->findWidget(someWidgetName, "ui2Screen").cast<ui2Screen>();
        if (screen) screen->setActive(true);
        else {
            someWidgetName = "MainMenu";
            m_widgets->MainMenu->setActive(true);
        }

        // FUN_001096b8();

        bool isNotMovie = !srfeMovieBase::isWidgetAMovie(someWidgetName);
        bool isNotMemcardCheck = strcmp(someWidgetName, "MemcardCheck") != 0;
        bool isNotLanguageAtBoot = strcmp(someWidgetName, "LanguageAtBoot") != 0;

        if (datArgParser::GetBooleanArgument("nofmv") || (isNotMovie && isNotMemcardCheck && isNotLanguageAtBoot)) {
            // FUN_00207750(0x27);
            // FUN_00207750(0x28);
        }

        /*
        if ((GFX::RenderState::Current.field25_0x40 & 0x40) != 0x40) {
            GFX::RenderState::Current.field25_0x40 = GFX::RenderState::Current.field25_0x40 & 0xffffffffffffffbf | 0x40;
            GFX::RenderState::Touched |= 1;
        }
        if ((GFX::RenderState::Current.field24_0x38 & 1) != 1) {
            GFX::RenderState::Current.field24_0x38 = GFX::RenderState::Current.field24_0x38 & 0xfffffffffffffffe | 1;
            GFX::RenderState::Touched |= 1;
        }

        UI::GameLoadingScreen::FUN_001c5408(&UI::GameLoadingScreen::InstanceMem,0);
        UI::GameLoadingScreen::FUN_001c54e8(&UI::GameLoadingScreen::InstanceMem,9);
        UI::GameLoadingScreen::InstanceMem.progress_frac = 1.0;
        UI::GameLoadingScreen::InstanceMem.progress_frac_again_for_some_reason = 1.0;
        GFX::Pipeline::SetVBlank(TRUE);
        */

        if (datArgParser::GetBooleanArgument("autoloadall") && !m_unk0.isMapUnknown()) {
            msgMsgSource::send(MSG_TYPE::LOAD_GAME, 0x80);
        }

        m_justBooted = false;

        // FUN_00300b78();
    }

    void InMenuState::Exit() {
        if (m_widgets) {
            // UI::GameLoadingScreen::FUN_001c54e8(&UI::GameLoadingScreen::InstanceMem,0);
            
            GameEngine::Instance()->BeginFrame(true);
            GameEngine::Instance()->EndFrame();

            GameEngine::Instance()->BeginFrame(true);
            GameEngine::Instance()->EndFrame();

            // UI::GameLoadingScreen::FUN_001c5408(&UI::GameLoadingScreen::InstanceMem,0);
            // FUN_002e55e0(0);

            GameEngine::Instance()->BeginFrame(false);

            m_widgets->Titlescreen = nullptr;
            m_widgets->loading->setActive(true);
            m_widgets->loading.cast<srfeLoading>()->FUN_00135560();
            m_frontendBase->draw();

            GameEngine::Instance()->EndFrame();
        }

        // gfx::pipeline::flushFrame();

        if (m_widgets) {
            m_selections.teamCarsP2[0] = 0;
            m_selections.teamSkinsP1[0] = 0;
            m_selections.teamSkinsP2[0] = 0;
            m_selections.teamCarsP1[0] = 0;

            if (datArgParser::GetBooleanArgument("autoloadall")) {
                if (!m_unk0.isMapUnknown()) {
                    const char* mapName = m_unk0.getMapName();
                    const char* missionName = m_unk0.getMissionName();

                    m_widgets->carPl1_var->maybeClearValue();
                    m_widgets->carPl2_var->maybeClearValue();
                    m_widgets->skin1_var->maybeClearValue();
                    m_widgets->skin2_var->maybeClearValue();
                    m_widgets->level_var->setStringData(mapName);
                    m_widgets->mission_var->setStringData(missionName);
                    m_widgets->humans_var->setStringData("1"); // actually gets "1" from FUN_00109c78, but it returns a global string that is never changed
                    snprintf(m_debugString, 256, "Automated load in progress...\n(%s, %s)", mapName, missionName);

                    m_unk0.advanceToNext();
                }
            } else {
                strncpy(m_regularStartupScreenName, m_widgets->regularStartupScreen_var->getStringData(), 30);

                if (m_widgets->videoStartupScreen_var->getType() == VariableType::String) {
                    strncpy(m_videoStartupScreenName, m_widgets->videoStartupScreen_var->getStringData(), 30);
                    // HasVideoStartupScreen = true;
                } else {
                    m_videoStartupScreenName[0] = 0;
                    // HasVideoStartupScreen = false;
                }
            }

            
            processLevelSelection();
            processHumansSelection();

            bool isSinglePlayerMission = processMissionSelection();
            i32 teamGameVar = processTeamGameSelection();

            processTeamCarSelections(teamGameVar);
            processTeamSkinSelections(teamGameVar, isSinglePlayerMission);
            processTransmissionSelections();
            processPoliceSelection(isSinglePlayerMission);
            processTimeOfDaySelection();

            datArgParser::Remove("teams");
            if (m_selections.humans[0] != '1') {
                if (strcmp(m_widgets->mission_var->getStringData(), "cruise.mp") == 0) {
                    datArgParser::AddReplace("teams", "nn");
                }
            }
        }

        // srui2VehicleDisplay::Shutdown();

        if (m_widgets) delete m_widgets;
        m_widgets = nullptr;
        m_frontendBase = nullptr;
                                   
        // DestroySomeGlobalString();
        // FUN_00109880();
        // FUN_002076f8();
        // UI::GameLoadingScreen::FUN_001c5408(&UI::GameLoadingScreen::InstanceMem,0);
        // UI::Unknown::uiUnknown14::Instance = NULL;
    }

    void InMenuState::Input() {
        if (!m_widgets || m_state != 0) return;
        if (m_widgets->quitfe_var->getIntegerData()) m_state = 1;
    }

    void InMenuState::Update() {
        // FUN_00207538();
        switch (m_state) {
            case 1: {
                if (Aud3DObjectManager::isAlive()) {
                    // FUN_00207888(0x27);
                    // FUN_00207888(0x28);
                    // FUN_00207888(0x3d);
                }

                // FUN_002079e0();
                // FUN_00207380();

                m_state = 2;
                break;
            }
            case 2: {
                i32 iVar3 = 1; // = FUN_002073a8();
                if (iVar3 != 1) return;

                m_state = 3;
                break;
            }
            case 3: {
                msgMsgSource::send(MSG_TYPE::LOAD_GAME, 0x80);
                m_state = 4;
                break;
            }
            default: return;
        }
    }

    void InMenuState::Reset(GameEngine* eng) {
    }

    void InMenuState::Draw() {
        gfx::pipeline::Clear(3, 0x80000000, 1.0f, 0);
        
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(gfx::pipeline::fWidth * debug_ui_scale, gfx::pipeline::fHeight * debug_ui_scale));
        ImGui::Begin("#m", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        m_frontendBase->draw();
        ImGui::End();
    }

    bool InMenuState::Done() {
        return false;
    }

    void InMenuState::displayUIInterest() {
        WidgetRef<srui2Bitmap> loadImg = nullptr;
        if (m_justBooted) {
            loadImg = new srui2Bitmap("StateFELoadImage", "uiinterest02.tga", 0, 0, 10, 0, nullptr);
            m_uiInterestIndex = (rand() % 22) + 1;
        } else {
            if (m_uiInterestIndex == 2) m_uiInterestIndex = 3;

            char bitmapPath[32] = { 0 };
            snprintf(bitmapPath, 32, "uiinterest%02.tga", m_uiInterestIndex);

            m_uiInterestIndex++;

            loadImg = new srui2Bitmap("StateFELoadImage", bitmapPath, 0, 0, 10, 0, nullptr);
        }

        GameEngine::Instance()->BeginFrame(true);

        // FUN_002ed360(0, 1);
        loadImg->draw();

        GameEngine::Instance()->EndFrame();

        // gfx::pipeline::flushFrame();
        
        GameEngine::Instance()->BeginFrame(false);
        GameEngine::Instance()->EndFrame();
    }

    void InMenuState::processLevelSelection() {
        strncpy(m_selections.mapName, m_widgets->level_var->getStringData(), 30);
        datArgParser::AddReplace("level", m_selections.mapName);

        if (strcmp(m_selections.mapName, "Afghan") == 0) m_selections.mapId = MAP_SELECTION::Afghan;
        else if (strcmp(m_selections.mapName, "Vietnam") == 0) m_selections.mapId = MAP_SELECTION::Vietnam;
        else if (strcmp(m_selections.mapName, "afghan_snow") == 0) m_selections.mapId = MAP_SELECTION::AfghanSnow;
        else assert(false);
    }

    void InMenuState::processHumansSelection() {
        if (strcmp(m_widgets->humans_var->getStringData(), "1") == 0) {
            snprintf(m_selections.humans, 30, "1");
            // IsSinglePlayer = true;
        } else {
            snprintf(m_selections.humans, 30, "2");
            // IsSinglePlayer = false;
        }

        datArgParser::AddReplace("humans", m_selections.humans);
    }

    bool InMenuState::processMissionSelection() {
        strncpy(m_selections.missionName, m_widgets->mission_var->getStringData(), 30);
        datArgParser::AddReplace("mission", m_selections.missionName);
        
        u32 missionNameLen = strlen(m_selections.missionName);
        if (m_selections.missionName[missionNameLen - 2] == 's') {
            if (m_selections.missionName[missionNameLen - 1] == 'p') {
                return true;
            }
        }

        return false;
    }

    i32 InMenuState::processTeamGameSelection() {
        if (m_widgets->team_game_var->getType() != VariableType::Integer) return 0;
        return m_widgets->team_game_var->getIntegerData();
    }

    void InMenuState::processTeamCarSelections(i32 teamGameVar) {
        datArgParser::Remove("cars");
        datArgParser::Remove("teamAcars");
        datArgParser::Remove("teamBcars");
        if (m_widgets->carPl1_var->getType() != VariableType::String) return;

        const char* carMate1Pl1 = "";
        const char* carMate2Pl1 = "";
        const char* carMate3Pl1 = "";
        const char* carPl1 = m_widgets->carPl1_var->getStringData();

        const char* carMate1Pl2 = "";
        const char* carMate2Pl2 = "";
        const char* carMate3Pl2 = "";
        const char* carPl2 = "";

        switch (teamGameVar) {
            case 1: {
                carMate1Pl1 = m_widgets->carMate1Pl1_var->getStringData();
                break;
            }
            case 2: {
                carMate1Pl1 = m_widgets->carMate1Pl1_var->getStringData();
                carMate2Pl1 = m_widgets->carMate2Pl1_var->getStringData();
                break;
            }
            case 3: {
                carMate1Pl1 = m_widgets->carMate1Pl1_var->getStringData();
                carMate2Pl1 = m_widgets->carMate2Pl1_var->getStringData();
                carMate3Pl1 = m_widgets->carMate3Pl1_var->getStringData();
                break;
            }
        }

        snprintf(m_selections.teamCarsP1, 30, "%s%s%s%s", carPl1, carMate1Pl1, carMate2Pl1, carMate3Pl1);

        if (m_selections.humans[0] != '1') {
            carPl2 = m_widgets->carPl2_var->getStringData();

            switch (teamGameVar) {
                case 1: {
                    carMate1Pl2 = m_widgets->carMate1Pl2_var->getStringData();
                    break;
                }
                case 2: {
                    carMate1Pl2 = m_widgets->carMate1Pl2_var->getStringData();
                    carMate2Pl2 = m_widgets->carMate2Pl2_var->getStringData();
                    break;
                }
                case 3: {
                    carMate1Pl2 = m_widgets->carMate1Pl2_var->getStringData();
                    carMate2Pl2 = m_widgets->carMate2Pl2_var->getStringData();
                    carMate3Pl2 = m_widgets->carMate3Pl2_var->getStringData();
                    break;
                }
            }
            
            snprintf(m_selections.teamCarsP2, 30, "%s%s%s%s", carPl2, carMate1Pl2, carMate2Pl2, carMate3Pl2);
        }

        if (teamGameVar == 0) {
            char buf[32] = { 0 };
            snprintf(buf, 30, "%s%s", m_selections.teamCarsP1, m_selections.teamCarsP2);
            strncpy(m_selections.teamCarsP1, buf, 30);
            datArgParser::AddReplace("cars", m_selections.teamCarsP1);
        } else {
            datArgParser::AddReplace("teamAcars", m_selections.teamCarsP1);
            if (m_selections.humans[0] != '1') {
                datArgParser::AddReplace("teamBcars", m_selections.teamCarsP2);
            }
        }
    }

    void InMenuState::processTeamSkinSelections(i32 teamGameVar, bool isSinglePlayerMission) {
        datArgParser::Remove("skins");
        datArgParser::Remove("teamAskin");
        datArgParser::Remove("teamBskin");
        if (isSinglePlayerMission || m_widgets->skin1_var->getType() != VariableType::String) return;

        strncpy(m_selections.teamSkinsP1, m_widgets->skin1_var->getStringData(), 30);

        if (m_selections.humans[0] != '1') {
            strncpy(m_selections.teamSkinsP2, m_widgets->skin2_var->getStringData(), 30);
        }

        if (teamGameVar == 0) {
            char buf[32] = { 0 };
            snprintf(buf, 32, "%s%s", m_selections.teamSkinsP1, m_selections.teamSkinsP2);
            strncpy(m_selections.teamSkinsP1, buf, 30);
            datArgParser::AddReplace("skins", m_selections.teamSkinsP1);
        } else {
            datArgParser::AddReplace("teamAskin", m_selections.teamSkinsP1);
            if (m_selections.humans[0] != '1') {
                datArgParser::AddReplace("teamBskin", m_selections.teamSkinsP2);
            }
        }
    }

    void InMenuState::processTransmissionSelections() {
        datArgParser::Remove("trans");
        if (m_widgets->transmission1_var->getType() == VariableType::Integer) {
            snprintf(m_selections.transmissions, 30, "%d", m_widgets->transmission1_var->getIntegerData());

            if (m_selections.humans[0] != '1') {
                if (m_widgets->transmission2_var->getType() == VariableType::Integer) {
                    char buf[32] = { 0 };
                    snprintf(buf, 32, "%s%d", m_selections.transmissions, m_widgets->transmission2_var->getIntegerData());
                    strncpy(m_selections.transmissions, buf, 30);
                }
            }

            datArgParser::AddReplace("trans", m_selections.transmissions);
        }
    }

    void InMenuState::processPoliceSelection(bool isSinglePlayerMission) {
        if (isSinglePlayerMission) {
            datArgParser::Remove("numai");
            return;
        }
        
        u32 numAI = 0;
        if (m_widgets->police_var->getType() != VariableType::Integer || m_widgets->police_var->getIntegerData() != 0) {
            if (false /* INT_0035f2e0 == -1*/) {
                numAI = 14;
                if (m_selections.humans[0] == '1') numAI = 15;
            } else {
                if (strcmp(m_widgets->humans_var->getStringData(), "2-1ON1") == 0) {
                    numAI = 0;
                } else if (false /* INT_0035f2e0 != 5 */) {
                    numAI = 6;
                    if (m_selections.humans[0] == '1') numAI = 7;
                } else {
                    const char* level = nullptr;
                    datArgParser::GetStringArgument("level", 0, &level);
                    if (strcmp(level, "afghan") == 0) {
                        numAI = 2;
                    } else {
                        numAI = 2;
                        if (strcmp(level, "vietnam") == 0) numAI = 14;
                    }
                }
            }
        }

        snprintf(m_selections.numAI, 30, "%d", numAI);
        datArgParser::AddReplace("numai", m_selections.numAI);
    }
    
    void InMenuState::processTimeOfDaySelection() {
        if (false /* INT_0035f2e0 != -1 */ || m_widgets->timeOfDay_var->getType() != VariableType::Integer) {
            datArgParser::Remove("variant");
            return;
        }

        u32 timeOfDay = m_widgets->timeOfDay_var->getIntegerData();
        u32 variantId = 0;

        if (strcmp(m_selections.mapName, "Afghan") == 0) {
            switch (timeOfDay) {
                case 0: { variantId = 6; break; }
                case 1: { variantId = 0; break; }
                case 2: { variantId = 5; break; }
                case 3: { variantId = 3; break; }
                default: assert(false);
            }
        } else if (strcmp(m_selections.mapName, "Vietnam") == 0) {
            switch (timeOfDay) {
                case 0: { variantId = 6; break; }
                case 1: { variantId = 0; break; }
                case 2: { variantId = 5; break; }
                case 3: { variantId = 3; break; }
                default: assert(false);
            }
        } else if (strcmp(m_selections.mapName, "afghan_snow") == 0) {
            switch (timeOfDay) {
                case 0: { variantId = 5; break; }
                case 1: { variantId = 0; break; }
                case 2: { variantId = 6; break; }
                case 3: { variantId = 4; break; }
                default: assert(false);
            }
        } else assert(false);
        
        snprintf(m_selections.variant, 30, "%d", variantId);
        datArgParser::AddReplace("variant", m_selections.variant);
    }
};