#include <libsr2/frontend/srfeVehicleSel.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    srfeVehicleSel::Widgets* srfeVehicleSel::WidgetsInstance = nullptr;
    u32 srfeVehicleSel::ActiveCount = 0;
    u32 srfeVehicleSel::InitializedCount = 0;

    struct MenuOption {
        char name[16];
        char value[16];
        u32 index;
    };

    MenuOption Teams[10] = {
        { "Name1" , "d", 1 },
        { "Name2" , "k", 2 },
        { "Name3" , "p", 8 },
        { "Name4" , "t", 3 },
        { "Name5" , "m", 7 },
        { "Name6" , "c", 5 },
        { "Name7" , "s", 4 },
        { "Name8" , "y", 0 },
        { "Name9" , "n", 9 },
        { "Name10", "x", 6 }
    };

    MenuOption Vehicles[8] = {
        { "Buggy"    , "d", 0 },
        { "Jeep"     , "k", 1 },
        { "ATV"      , "p", 3 },
        { "Rally"    , "t", 5 },
        { "Trophy"   , "m", 2 },
        { "SUV"      , "c", 4 },
        { "Military" , "s", 6 },
        { "Massive"  , "y", 7 }
    };

    srfeVehicleSel::srfeVehicleSel(
        const char* name,
        const char* prevScreenName,
        const char* nextScreenName,
        const WidgetRef<ui2Master>& master
    ) : srfeBasicLayout2(name, master) {
        m_prevScreenName = prevScreenName;
        m_nextScreenName = nextScreenName;

        field_0xb8 = 0;
        field_0xc4 = 0;
        field_0xc8 = 0;

        if (m_isActive) ActiveCount++;
        if (WidgetsInstance) {
            if (ActiveCount == 1) WidgetsInstance->master->setActive(true);
        }
    }

    srfeVehicleSel::~srfeVehicleSel() {
        deinitScreen();

        if (m_isActive) ActiveCount--;
    }

    void srfeVehicleSel::setActive(bool active) {
        bool wasActive = m_isActive;
        srfeBasicLayout2::setActive(active);

        if (m_isActive == active) return;

        if (active) {
            ActiveCount++;

            if (ActiveCount == 1) {
                WidgetsInstance->master->setActive(true);
            }
        } else {
            ActiveCount--;

            if (ActiveCount == 0 && WidgetsInstance) {
                WidgetsInstance->master->setActive(false);
            }
        }
    }

    void srfeVehicleSel::initScreen() {
        srfeBasicLayout2::initScreen();
        InitializedCount++;

        m_headingText->setTextName("VehicleSel_Heading");

        Widgets* w = WidgetsInstance;
        if (!w) initWidgets();

        m_input->addListener(this, WidgetEventType::BackPressed, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        m_input->addListener(this, WidgetEventType::AcceptPressed, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
    }

    void srfeVehicleSel::deinitScreen() {
    }

    void srfeVehicleSel::initWidgets() {
        WidgetsInstance = new Widgets();
        Widgets* w = WidgetsInstance;

        auto gm = ui2Base::getGlobalMaster();

        w->master = new ui2Master("VehicleSel_Master", gm->getMaster(), true);
        gm->pushMaster(w->master);

        w->vehicleInput1 = new ui2Input("VehicleSel_VehicleInput1", nullptr);
        w->p1TeamMenu = new srui2CycleMenu(
            "VehicleSel_TeamPl1_cycmenu",
            10,
            "VehicleSel_TeamPl1",
            "VehicleSel_Team",
            "VehicleSel_MenuName_Team",
            0,
            0,
            "skin1_var",
            nullptr
        );

        for (u32 i = 0;i < 10;i++) {
            w->p1TeamMenu->addOption(Teams[i].name, Teams[i].index, Teams[i].value);
        }

        w->vehicleInput1->addListener(w->p1TeamMenu, WidgetEventType::AnyInput);

        w->p1VehicleMenu = new srui2CycleMenu(
            "VehicleSel_VehiclePl1_cycmenu",
            8,
            "VehicleSel_VehiclePl1",
            "VehicleSel_VehicleType",
            "VehicleSel_MenuName_Player",
            0,
            0,
            "carPl1_var",
            nullptr
        );

        for (u32 i = 0;i < 8;i++) {
            w->p1VehicleMenu->addOption(Vehicles[i].name, Vehicles[i].index, Vehicles[i].value);
        }

        w->vehicleInput1->addListener(w->p1VehicleMenu, WidgetEventType::AnyInput);

        w->p1MateMenu1 = new srui2CycleMenu(
            "VehicleSel_VehicleMate1Pl1_cycmenu",
            8,
            "VehicleSel_VehicleMate1Pl1",
            "VehicleSel_VehicleType",
            "VehicleSel_MenuName_Mate1",
            0,
            0,
            "carMate1Pl1_var",
            nullptr
        );

        w->p1MateMenu2 = new srui2CycleMenu(
            "VehicleSel_VehicleMate2Pl1_cycmenu",
            8,
            "VehicleSel_VehicleMate2Pl1",
            "VehicleSel_VehicleType",
            "VehicleSel_MenuName_Mate2",
            0,
            0,
            "carMate2Pl1_var",
            nullptr
        );

        w->p1MateMenu3 = new srui2CycleMenu(
            "VehicleSel_VehicleMate3Pl1_cycmenu",
            8,
            "VehicleSel_VehicleMate3Pl1",
            "VehicleSel_VehicleType",
            "VehicleSel_MenuName_Mate3",
            0,
            0,
            "carMate3Pl1_var",
            nullptr
        );

        for (u32 i = 0;i < 8;i++) {
            w->p1MateMenu1->addOption(Vehicles[i].name, Vehicles[i].index, Vehicles[i].value);
            w->p1MateMenu2->addOption(Vehicles[i].name, Vehicles[i].index, Vehicles[i].value);
            w->p1MateMenu3->addOption(Vehicles[i].name, Vehicles[i].index, Vehicles[i].value);
        }

        w->p1MateMenu1->addListener(w->p1MateMenu1, WidgetEventType::AnyInput);
        w->p1MateMenu2->addListener(w->p1MateMenu2, WidgetEventType::AnyInput);
        w->p1MateMenu3->addListener(w->p1MateMenu3, WidgetEventType::AnyInput);

        w->menu1 = new ui2Menu("VehicleSel_Menu1_menu", 1, 5, nullptr);
        w->menu1->FUN_001f8218(w->p1TeamMenu, 0, 0, 1, &ui2Widget::acceptEvent);
        w->menu1->FUN_001f8218(w->p1VehicleMenu, 0, 1, 1, &ui2Widget::acceptEvent);
        w->menu1->FUN_001f8218(w->p1MateMenu1, 0, 2, 1, &ui2Widget::acceptEvent);
        w->menu1->FUN_001f8218(w->p1MateMenu2, 0, 3, 1, &ui2Widget::acceptEvent);
        w->menu1->FUN_001f8218(w->p1MateMenu3, 0, 4, 1, &ui2Widget::acceptEvent);
        w->menu1->FUN_001fa518();

        w->menu1Tbl = new ui2Table("VehicleSel_menu1_tbl", 1, 5, 350, 180, nullptr);
        w->menu1Tbl->setRowSizes(20, -1, -2);
        w->menu1Tbl->FUN_001fc6f8(w->p1TeamMenu, 0, 0, &ui2Widget::acceptEvent);
        w->menu1Tbl->FUN_001fc6f8(w->p1VehicleMenu, 0, 1, &ui2Widget::acceptEvent);
        w->menu1Tbl->FUN_001fc6f8(w->p1MateMenu1, 0, 2, &ui2Widget::acceptEvent);
        w->menu1Tbl->FUN_001fc6f8(w->p1MateMenu2, 0, 3, &ui2Widget::acceptEvent);
        w->menu1Tbl->FUN_001fc6f8(w->p1MateMenu3, 0, 4, &ui2Widget::acceptEvent);

        w->menu1CareerTbl = new ui2Table("VehicleSel_menu1-career_tbl", 1, 4, 350, 180, nullptr);
        w->menu1CareerTbl->setRowSizes(20, -1, -2);
        w->menu1CareerTbl->FUN_001fc6f8(w->p1VehicleMenu, 0, 0, &ui2Widget::acceptEvent);
        w->menu1CareerTbl->FUN_001fc6f8(w->p1MateMenu1, 0, 1, &ui2Widget::acceptEvent);
        w->menu1CareerTbl->FUN_001fc6f8(w->p1MateMenu2, 0, 2, &ui2Widget::acceptEvent);
        w->menu1CareerTbl->FUN_001fc6f8(w->p1MateMenu3, 0, 3, &ui2Widget::acceptEvent);

        w->vehicleDisplay1 = new srui2VehicleDisplay("VehicleSel_vehicleDispl1", 40, 148, 0xfffffffb, w->master);
        w->vehicleDisplay1->FUN_001e0f68(290, 160);

        w->vehicleLockedL = new ui2Sound("VehicleSel_vehicleLocked_l_snd", 53, nullptr);
        w->vehicleLockedL->FUN_00208068(-1.0f);
        w->vehicleLockedL->FUN_00207fc8();

        w->vehicleLockedR = new ui2Sound("VehicleSel_vehicleLocked_r_snd", 54, nullptr);
        w->vehicleLockedR->FUN_00208068(1.0f);
        w->vehicleLockedR->FUN_00207fc8();

        w->vehicleTeamDisp1L = new ui2Sound("VehicleSel_vehicleTeamDispl_l_snd", 64, nullptr);
        w->vehicleTeamDisp1L->FUN_00208068(0.0f);
        w->vehicleTeamDisp1L->FUN_00207fc8();

        w->vehicleTeamDisp1R = new ui2Sound("VehicleSel_vehicleTeamDispl_r_snd", 65, nullptr);
        w->vehicleTeamDisp1R->FUN_00208068(0.0f);
        w->vehicleTeamDisp1R->FUN_00207fc8();

        w->valueTextBackdrop = new ui2Image("VehicleSel_val_text_backdrop_img", "uisquareblue", 455, 290, 5, nullptr);
        w->valueTextBackdrop->setSize(65, 88);

        w->valueBackdrop = new ui2Image("VehicleSel_val_backdrop_img", "uisquarered", 520, 290, 5, nullptr);
        w->valueBackdrop->setSize(31, 88);

        w->menuLeftBackdrop = new ui2Image("VehicleSel_menu_left_backdrop_img", "uisquareblue", 340, 175, 5, nullptr);
        w->menuLeftBackdrop->setSize(115, 115);

        w->menuMiddleBackdrop = new ui2Image("VehicleSel_menu_middle_backdrop_img", "uisquarered", 455, 175, 5, nullptr);
        w->menuMiddleBackdrop->setSize(95, 115);

        w->menuRightBackdrop = new ui2Image("VehicleSel_menu_right_backdrop_img", "uisquareblue", 550, 175, 5, nullptr);
        w->menuRightBackdrop->setSize(40, 115);

        w->carBackdrop = new ui2Image("VehicleSel_car_backdrop_img", "uisquarered", 30, 120, 5, nullptr);
        w->carBackdrop->setSize(310, 260);

        initWidgetsSub1();

        w->vehicleValue1Tbl->FUN_001fc540(465, 295);
        w->vehicleHeading1->method_0xe8(130, 125);
        w->vehicleDesc1->method_0xe8(85, 310);
        w->vehicleDesc1->setBounds(240, 95);

        // Why is `this` used when setting up listeners for widgets that apply to all instances???
        w->p1TeamMenu->addListener(this, WidgetEventType::UNK29, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1VehicleMenu->addListener(this, WidgetEventType::UNK29, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1MateMenu1->addListener(this, WidgetEventType::UNK29, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1MateMenu2->addListener(this, WidgetEventType::UNK29, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1MateMenu3->addListener(this, WidgetEventType::UNK29, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        
        w->p1TeamMenu->addListener(this, WidgetEventType::UNK38, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1VehicleMenu->addListener(this, WidgetEventType::UNK38, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1MateMenu1->addListener(this, WidgetEventType::UNK38, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1MateMenu2->addListener(this, WidgetEventType::UNK38, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);
        w->p1MateMenu3->addListener(this, WidgetEventType::UNK38, (WidgetEventCallback)&srfeVehicleSel::interceptEvent);

        w->transInput1 = new ui2Input("VehicleSel_transInput1", nullptr);

        initWidgetsSub2();

        w->transInput1->addListener(w->trans1SelectionMenu, WidgetEventType::AnyInput);
        w->trans1Tbl->FUN_001fc540(150, 190);

        gm->popMaster();

        if (ActiveCount == 0) w->master->setActive(false);
    }

    void srfeVehicleSel::initWidgetsSub1() {
    }

    void srfeVehicleSel::initWidgetsSub2() {
    }
};