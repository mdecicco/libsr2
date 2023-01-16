#include <libsr2/states/GameLoadState.h>
#include <libsr2/managers/phMaterialMgr.h>
#include <libsr2/managers/TriggerManager.h>
#include <libsr2/managers/missManager.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/terrain/offroadLevel.h>
#include <libsr2/sim/phMaterial.h>
#include <libsr2/states/gameFSM.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/utilities/argParser.h>
#include <libsr2/gfx/gfx.h>
#include <libsr2/mission/MissionWeatherData.h>
#include <libsr2/libsr2.h>

namespace sr2 {
    GameLoadState::GameLoadState() {
        next_load_switch_mode = load_switch_mode = 0;
    }

    GameLoadState::~GameLoadState() {

    }

    void GameLoadState::Reset() {
        next_load_switch_mode = load_switch_mode = 0;

        // i18n::txtStringTable::Load(&g_gameplayText, "gameplay", g_languageId, 0xffff, false);
        // if (gfx::pipeline::CopyToFront != 4) {
            // gfx::pipeline::FlickerBlend = 0x40;
            // gfx::pipeline::FlickerXShift = 0;
        // }
    }

    void GameLoadState::Enter(GameEngine* eng) {
        char* level = "test";
        char* mission = "cruise.mp";
        datArgParser::GetStringArgument("level", 0, &level);
        datArgParser::GetStringArgument("mission", 0, &mission);
        // InitGameLoadingScreen();

        Reset();

        // srAudMgr::Get()->method_0x130();
        // uiProgress::Get()->deactivate();
        // uiProgress::Get()->set_position(320, 403);
        // uiProgress::Get()->set_size(290, 13);
        // uiProgress::Get()->field_0x84 = 0;
        //
        // ... And more stuff for the incomprehensible UI system
        // Allocate "loading-base" ui2Base object and whatnot
    }

    void GameLoadState::Update() {
        char* mission_name = "cruise.mp";
        char* map_name = "test";
        i32 variant = 0;
        datArgParser::GetStringArgument("level", 0, &map_name);
        datArgParser::GetStringArgument("mission", 0, &mission_name);
        datArgParser::GetIntegerArgument("variant", 0, &variant);

        load_switch_mode = next_load_switch_mode;
        switch (load_switch_mode) {
            case 0: {
                // gameStateLoading Fade In
                next_load_switch_mode = 1;
                break;
            }
            case 1: {
                // gameStateLoading Loading Level

                // gfx::gfxTexture::DefaultMipBiasK = -4.0;
                // gfx::gfxTexture::DefaultMipBiasL = 0;

                // Creature::crAnimation::InitChannels(12, 100);

                TriggerMgr::Create();

                phMaterialMgr::create(200);
                auto dm = phMaterialMgr::getDefaultMaterial();
                dm->friction = 0.7f;
                dm->elasticity = 0.5f;
                
                offroadLevel* lvl = offroadLevel::create();
                // UnkManager* um = new UnkManager(0x78);
                // dgBangerDataManager* bdm = new dgBangerDataManager(0x80);
                // dynManager* dm = new dynManager();
                // dgGeyserManager* gm = new dgGeyserManager();
                // Managers::ParticleManager::Reset();

                MissionWeatherData weather;
                weather.load(map_name);
                variant = weather.getWeatherVariant(mission_name);
                if (datArgParser::GetBooleanArgument("variant")) {
                    datArgParser::GetIntegerArgument("variant", 0, &variant);
                } else {
                    static char _variant[8] = { 0 };
                    snprintf(_variant, 8, "%d", variant);
                    datArgParser::AddReplace("variant", _variant);
                }

                if (variant - 2 < 2) {
                    if (strcmp(map_name, "Afghan") == 0 || strcmp(map_name, "Vietnam") == 0) {
                        /*
                        FUN_00254a00(1);
                        if (Aud3DObjectManager::IsAlive()) {
                            if (variant == 3) srAudMgr::FUN_001c1238(0.4f, 0);
                            else srAudMgr::FUN_001c1238(0.4f, 1);
                        }
                        */
                    }
                } // else FUN_00254a00(0);

                MissionWeatherData::CurrentWeatherType = (WEATHER_TYPE)variant;

                u32 player_count = 1;
                // player_count = GameEngine::Instance()->geHumanPlayerCount();
                lvl->setMaxViewports(player_count, -1);
                lvl->load(map_name, mission_name, (WEATHER_TYPE)variant);
                lvl->FUN_00218f20(0);

                /*
                if (Aud3DObjectManager::IsAlive()) {
                    srAudMgr::LoadCreatureAudioCsv(map_name);
                }
                */

                if (player_count == 4) {
                    // ???
                    exit(-1);
                }

                std::string wheel_ptx = format("ptx_wheel_%s", map_name);
                if (datAssetManager::exists("texture", wheel_ptx.c_str(), "tex")) {
                    // Vehicle::vehWheelPtx::MaybeDefaultLevelPtxTexName = duplicate_string(wheel_ptx.c_str());
                }

                // FUN_00247e60(lvl->getShadowAlpha());

                next_load_switch_mode = 2;
                break;
            }
            case 2: {
                // Loading Level
                next_load_switch_mode = 3;
                break;
            }
            case 3: {
                // Loading Mission
                next_load_switch_mode = 4;
                break;
            }
            case 4: {
                // Initializing audio
                next_load_switch_mode = 7;
                break;
            }
            case 5: {
                // Loading AI
                next_load_switch_mode = 6;
                break;
            }
            case 6: {
                // Initializing physics
                next_load_switch_mode = 8;
                break;
            }
            case 7: {
                // Initializing player
                next_load_switch_mode = 5;
                break;
            }
            case 8: {
                // Initializing HUDs
                next_load_switch_mode = 11;
                break;
            }
            case 9: {
                // Initializing GFX
                next_load_switch_mode = 13;
                break;
            }
            case 10: {
                // Loading creatures
                next_load_switch_mode = 12;
                break;
            }
            case 11: {
                // Loading triggers
                next_load_switch_mode = 10;
                break;
            }
            case 12: {
                // Loading bank
                next_load_switch_mode = 9;
                break;
            }
            case 13: {
                // gameStateLoading done fading
                load_switch_mode = next_load_switch_mode = 14;
                break;
            }
        }
    }

    void GameLoadState::Draw() {
        if (gfx::pipeline::OrthoVP != gfx::pipeline::Viewport) {
            gfx::pipeline::ForceSetViewport(gfx::pipeline::OrthoVP);
        }

        if (load_switch_mode != 14) {
            // some_global.progress_frac = uiProgress::Get()->progress_value / 100.0f;
            // some_global.progress_frac_again_for_some_reason = uiProgress::Get()->progress_value / 100.0f;
            // uiProgress::Get()->Draw();
        }
    }

    bool GameLoadState::Done() {
        if (load_switch_mode == 14) {
            // Utils::msgMsgSource::Send(0x3802, 0x80);
            // the above line essentially does the following until literally everything else is implemented
            GameEngine::Instance()->ChangeState(GAME_STATE::IN_GAME);
        }
        return false;
    }

    void GameLoadState::Exit() {
        // uiProgress::Get()->some_method(); <- method not determined yet
        // uiProgress::Get()->field_0x84 = 1;
        datArgParser::Remove("variant");
    }
};