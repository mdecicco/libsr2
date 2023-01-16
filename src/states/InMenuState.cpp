#include <libsr2/states/InMenuState.h>
#include <libsr2/libsr2.h>
#include <libsr2/states/gameFSM.h>

namespace sr2 {
    typedef ui2Widget* (*WidgetGenFunc_TypeA)(const char* /* var name */, u8 /* size */, StoredWidget* /* nobody knows */);
    typedef ui2Widget* (*WidgetGenFunc_TypeB)(const char* /* unknown */, StoredWidget* /* nobody knows */);
    typedef ui2Widget* (*WidgetGenFunc_TypeC)(const char* /* unknown 1 */, const char* /* unknown 2 */, const char* /* unknown 3 */, i32 /* unknown 4 */, StoredWidget* /* nobody knows */);
    typedef ui2Widget* (*WidgetGenFunc_TypeD)(const char* /* unknown 1 */, const char* /* unknown 2 */, const char* /* unknown 3 */, StoredWidget* /* nobody knows */);
    typedef ui2Widget* (*WidgetGenFunc_TypeE)(const char* /* unknown */, StoredWidget* /* nobody knows */);

    ui2Widget* Dummy_TypeA(const char* name, u8 size, StoredWidget* unk1) {
        return nullptr;
    }
    ui2Widget* Dummy_TypeB(const char* name, StoredWidget* unk1) {
        return nullptr;
    }
    ui2Widget* Dummy_TypeC(const char* unk0, const char* unk1, const char* unk2, i32 unk3, StoredWidget* unk4) {
        return nullptr;
    }
    ui2Widget* Dummy_TypeD(const char* unk0, const char* unk1, const char* unk2, StoredWidget* unk4) {
        return nullptr;
    }
    ui2Widget* Dummy_TypeE(const char* name, StoredWidget* unk1) {
        return nullptr;
    }
    
    struct wva {
        u16 idx;
        const char* a;
        u8 sz;
        WidgetGenFunc_TypeA creator;
    };

    struct wvb {
        u16 idx;
        const char* a;
        WidgetGenFunc_TypeB creator;
    };

    struct wvc {
        u16 idx;
        const char* a;
        const char* b;
        const char* c;
        i32 d;
        WidgetGenFunc_TypeC creator;
    };

    struct wvd {
        u16 idx;
        const char* a;
        const char* b;
        const char* c;
        WidgetGenFunc_TypeD creator;
    };

    struct wve {
        u16 idx;
        const char* a;
        WidgetGenFunc_TypeE creator;
    };

    InMenuState::InMenuState() {
    }

    InMenuState::~InMenuState() {
        delete field_0x8;
    }

    void InMenuState::Enter(GameEngine* eng) {
        // for now
        return;

        Reset(eng);

        field_0x8 = new WidgetStorage();

        auto type_a = [this](u16 idx, const char* name, u8 size, WidgetGenFunc_TypeA create) {
            StoredWidget temp;
            temp.widget = nullptr;
            temp.some_func_table = nullptr; // _widget::vtb_0377b80
            ui2Widget* w = create(name, size, &temp);
            set_widget(idx, w, &temp);
            return w;
        };
        auto type_b = [this](u16 idx, const char* name, WidgetGenFunc_TypeB create) {
            StoredWidget temp;
            temp.widget = nullptr;
            temp.some_func_table = nullptr; // _widget::vtb_0377b80
            ui2Widget* w = create(name, &temp);
            set_widget(idx, w, &temp);
            return w;
        };
        auto type_c = [this](u16 idx, const char* unk1, const char* unk2, const char* unk3, i32 unk4, WidgetGenFunc_TypeC create) {
            StoredWidget temp;
            temp.widget = nullptr;
            temp.some_func_table = nullptr; // _widget::vtb_0377b80
            ui2Widget* w = create(unk1, unk2, unk3, unk4, &temp);
            set_widget(idx, w, &temp);
            return w;
        };
        auto type_d = [this](u16 idx, const char* unk1, const char* unk2, const char* unk3, WidgetGenFunc_TypeD create) {
            StoredWidget temp;
            temp.widget = nullptr;
            temp.some_func_table = nullptr; // _widget::vtb_0377b80
            ui2Widget* w = create(unk1, unk2, unk3, &temp);
            set_widget(idx, w, &temp);
            return w;
        };
        auto type_e = [this](u16 idx, const char* unk1, WidgetGenFunc_TypeE create) {
            StoredWidget temp;
            temp.widget = nullptr;
            temp.some_func_table = nullptr; // _widget::vtb_0377b80
            ui2Widget* w = create(unk1, &temp);
            set_widget(idx, w, &temp);
            return w;
        };


        // load_vehicle_types();


        ui2Widget* quitfe_var = type_a(0x52, "quitfe_var", 0x32, Dummy_TypeA);
        // FUN_002021f0(quitfe_var, 0);

        type_a(0x40, "carPl1_var"               , 0x32, Dummy_TypeA);
        type_a(0x41, "carMate1Pl1_var"          , 0x32, Dummy_TypeA);
        type_a(0x42, "carMate2Pl1_var"          , 0x32, Dummy_TypeA);
        type_a(0x43, "carMate3Pl1_var"          , 0x32, Dummy_TypeA);
        type_a(0x44, "carPl2_var"               , 0x32, Dummy_TypeA);
        type_a(0x45, "carMate1Pl2_var"          , 0x32, Dummy_TypeA);
        type_a(0x46, "carMate2Pl2_var"          , 0x32, Dummy_TypeA);
        type_a(0x47, "carMate3Pl2_var"          , 0x32, Dummy_TypeA);
        type_a(0x3e, "level_var"                , 0x32, Dummy_TypeA);
        type_a(0x3f, "mission_var"              , 0x32, Dummy_TypeA);
        type_a(0x48, "skin1_var"                , 0x32, Dummy_TypeA);
        type_a(0x49, "skin2_var"                , 0x32, Dummy_TypeA);
        type_a(0x4a, "humans_var"               , 0x32, Dummy_TypeA);
        type_a(0x4b, "timeOfDay_var"            , 0x32, Dummy_TypeA);
        type_a(0x4c, "police_var"               , 0x32, Dummy_TypeA);
        type_a(0x4d, "team_game_var"            , 0x32, Dummy_TypeA);
        type_a(0x4e, "transmission1_var"        , 0x32, Dummy_TypeA);
        type_a(0x4f, "transmission2_var"        , 0x32, Dummy_TypeA);
        type_a(0x50, "regularStartupScreen_var" , 0x32, Dummy_TypeA);
        type_a(0x51, "videoStartupScreen_var"   , 0x32, Dummy_TypeA);
        type_b(0x01, "MainMenu"                 , Dummy_TypeB);
        type_b(0x02, "BombTag"                  , Dummy_TypeB);
        type_b(0x03, "CheckpointRace"           , Dummy_TypeB);
        type_b(0x04, "CNS"                      , Dummy_TypeB);
        type_b(0x06, "Joyriding"                , Dummy_TypeB);
        type_b(0x05, "LootGrab"                 , Dummy_TypeB);
        type_b(0x0b, "TurfWars"                 , Dummy_TypeB);
        type_b(0x10, "LoadOrNew"                , Dummy_TypeB);
        type_b(0x11, "MissionSelect"            , Dummy_TypeB);
        type_b(0x12, "NameInput"                , Dummy_TypeB);
        type_b(0x13, "SavegameSelect"           , Dummy_TypeB);
        type_c(0x07, "PlayerSelCPR"               , "CheckpointRace", "MainMenu", 1   , Dummy_TypeC);
        type_c(0x08, "PlayerSelJoyriding"         , "Joyriding"     , "MainMenu", 0   , Dummy_TypeC);
        type_c(0x09, "PlayerSelTurfWars"          , "TurfWars"      , "MainMenu", 1   , Dummy_TypeC);
        type_c(0x10, "PlayerSelBonus"             , "BonusMissions" , "Extras"  , 1   , Dummy_TypeC);
        type_d(0x1f, "VehicleSelBonus"            , "placeholder"   , "BonusMissions" , Dummy_TypeD);
        type_d(0x19, "VehicleSelBombTag"          , "placeholder"   , "BombTag"       , Dummy_TypeD);
        type_d(0x1a, "VehicleSelCheckpointRace"   , "placeholder"   , "CheckpointRace", Dummy_TypeD);
        type_d(0x1b, "VehicleSelCNS"              , "placeholder"   , "CNS"           , Dummy_TypeD);
        type_d(0x1c, "VehicleSelJoyriding"        , "placeholder"   , "Joyriding"     , Dummy_TypeD);
        type_d(0x1d, "VehicleSelLootGrab"         , "placeholder"   , "LootGrab"      , Dummy_TypeD);
        type_d(0x1e, "VehicleSelSmugglerM"        , "placeholder"   , "MissionSelect" , Dummy_TypeD);
        type_d(0x20, "VehicleSel2PlBombTag"       , "placeholder"   , "BombTag"       , Dummy_TypeD);
        type_d(0x21, "VehicleSel2PlCheckpointRace", "placeholder"   , "CheckpointRace", Dummy_TypeD);
        type_d(0x22, "VehicleSel2PlCNS"           , "placeholder"   , "CNS"           , Dummy_TypeD);
        type_d(0x23, "VehicleSel2PlJoyriding"     , "placeholder"   , "Joyriding"     , Dummy_TypeD);
        type_d(0x24, "VehicleSel2PlLootGrab"      , "placeholder"   , "LootGrab"      , Dummy_TypeD);
        type_d(0x25, "VehicleSel2PlBonus"         , "placeholder"   , "BonusMissions" , Dummy_TypeD);
        type_e(0x0c, "Options"                    , Dummy_TypeE);
        type_e(0x0d, "ControllerSetup"            , Dummy_TypeE);
        type_e(0x0e, "MemcardManager"             , Dummy_TypeE);
        type_e(0x0f, "LanguageInGame"             , Dummy_TypeE);
        type_e(0x14, "Extras"                     , Dummy_TypeE);
        type_e(0x15, "Credits"                    , Dummy_TypeE);
        type_e(0x16, "Theater"                    , Dummy_TypeE);
        type_e(0x17, "ConceptArt"                 , Dummy_TypeE);
        type_e(0x18, "BonusMissions"              , Dummy_TypeE);
        type_e(0x26, "HiScoresMain"               , Dummy_TypeE);
        type_e(0x27, "HiScoresSmugglerM"          , Dummy_TypeE);
        type_e(0x28, "HiScoresCNS"                , Dummy_TypeE);
        type_e(0x29, "HiScoresLootGrab"           , Dummy_TypeE);
        type_e(0x2a, "HiScoresBombTag"            , Dummy_TypeE);
        type_e(0x2b, "HiScoresCheckpointRace"     , Dummy_TypeE);
        type_e(0x2c, "HiScoresBonus"              , Dummy_TypeE);
        type_e(0x3b, "loading"                    , Dummy_TypeE);
        type_e(0x00, "Titlescreen"                , Dummy_TypeE);
        type_e(0x2d, "MovieRockStarLogo"          , Dummy_TypeE);
        type_e(0x2e, "MovieAngelLogo"             , Dummy_TypeE);
        type_e(0x2f, "MovieIntro"                 , Dummy_TypeE);
        type_e(0x30, "MovieUIIntro"               , Dummy_TypeE);
        type_e(0x31, "MovieCareer_1"              , Dummy_TypeE);
        type_e(0x32, "MovieCareer_2"              , Dummy_TypeE);
        type_e(0x33, "MovieCareer_3"              , Dummy_TypeE);
        type_e(0x34, "MovieCareer_4"              , Dummy_TypeE);
        type_e(0x35, "MovieCareer_5"              , Dummy_TypeE);
        type_e(0x36, "MovieCareer_6"              , Dummy_TypeE);
        type_e(0x37, "MovieCareer_7"              , Dummy_TypeE);
        type_e(0x38, "MovieCareer_8"              , Dummy_TypeE);
        type_e(0x39, "MovieCareer_9"              , Dummy_TypeE);
        type_e(0x3a, "MovieSRNews"                , Dummy_TypeE);
        type_e(0x3c, "MemcardCheck"               , Dummy_TypeE);
        type_e(0x3d, "LanguageAtBoot"             , Dummy_TypeE);
    }

    void InMenuState::Update() {
        // for now
        GameEngine::Instance()->ChangeState(GAME_STATE::GAME_LOAD);
    }

    void InMenuState::Reset(GameEngine* eng) {
    }

    bool InMenuState::Done() {
        return false;
    }

    void InMenuState::set_widget(u16 idx, ui2Widget* widget, StoredWidget* unk) {
        ui2Widget* prev = field_0x8->widgets[idx].widget;
        if (prev) {
            // probably notifying it of its detachment
            // (**(code **)((prev->base).vtb[2].field_0x0.field_0x0 + 4))();
        }

        field_0x8->widgets[idx].widget = widget;
        if (widget) {
            // probably notifying it of its attachment
            // (**(code **)((widget->base).vtb[1].field_0x0.field_0x0 + 4))();
        }

        unk->some_func_table = nullptr; // _widget::vtb_00377b98
        if (unk->widget) {
            // calls the same method as the if(prev) body. Not sure what it means
            // (**(code **)(((unk->widget)->base).vtb[2].field_0x0.field_0x0 + 4))();
        }
    }
};