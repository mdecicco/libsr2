#pragma once
#include <libsr2/types.h>
#include <libsr2/states/GameState.h>
#include <libsr2/ui/ui2Screen.h>
#include <libsr2/ui/ui2Variable.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    class ui2Widget;

    struct MenuWidgets {
        WidgetRef<ui2Screen> Titlescreen;
        WidgetRef<ui2Screen> MainMenu;
        WidgetRef<ui2Screen> BombTag;
        WidgetRef<ui2Screen> CheckpointRace;
        WidgetRef<ui2Screen> CNS;
        WidgetRef<ui2Screen> LootGrab;
        WidgetRef<ui2Screen> Joyriding;
        WidgetRef<ui2Screen> PlayerSelCPR;
        WidgetRef<ui2Screen> PlayerSelJoyriding;
        WidgetRef<ui2Screen> PlayerSelTurfWars;
        WidgetRef<ui2Screen> PlayerSelBonus;
        WidgetRef<ui2Screen> TurfWars;
        WidgetRef<ui2Screen> Options;
        WidgetRef<ui2Screen> ControllerSetup;
        WidgetRef<ui2Screen> MemcardManager;
        WidgetRef<ui2Screen> LanguageInGame;
        WidgetRef<ui2Screen> LoadOrNew;
        WidgetRef<ui2Screen> MissionSelect;
        WidgetRef<ui2Screen> NameInput;
        WidgetRef<ui2Screen> SavegameSelect;
        WidgetRef<ui2Screen> Extras;
        WidgetRef<ui2Screen> Credits;
        WidgetRef<ui2Screen> Theater;
        WidgetRef<ui2Screen> ConceptArt;
        WidgetRef<ui2Screen> BonusMissions;
        WidgetRef<ui2Screen> VehicleSelBombTag;
        WidgetRef<ui2Screen> VehicleSelCheckpointRace;
        WidgetRef<ui2Screen> VehicleSelCNS;
        WidgetRef<ui2Screen> VehicleSelJoyriding;
        WidgetRef<ui2Screen> VehicleSelLootGrab;
        WidgetRef<ui2Screen> VehicleSelSmugglerM;
        WidgetRef<ui2Screen> VehicleSelBonus;
        WidgetRef<ui2Screen> VehicleSel2PlBombTag;
        WidgetRef<ui2Screen> VehicleSel2PlCheckpointRace;
        WidgetRef<ui2Screen> VehicleSel2PlCNS;
        WidgetRef<ui2Screen> VehicleSel2PlJoyriding;
        WidgetRef<ui2Screen> VehicleSel2PlLootGrab;
        WidgetRef<ui2Screen> VehicleSel2PlBonus;
        WidgetRef<ui2Screen> HiScoresMain;
        WidgetRef<ui2Screen> HiScoresSmugglerM;
        WidgetRef<ui2Screen> HiScoresCNS;
        WidgetRef<ui2Screen> HiScoresLootGrab;
        WidgetRef<ui2Screen> HiScoresBombTag;
        WidgetRef<ui2Screen> HiScoresCheckpointRace;
        WidgetRef<ui2Screen> HiScoresBonus;
        WidgetRef<ui2Screen> MovieRockStarLogo;
        WidgetRef<ui2Screen> MovieAngelLogo;
        WidgetRef<ui2Screen> MovieIntro;
        WidgetRef<ui2Screen> MovieUIIntro;
        WidgetRef<ui2Screen> MovieCareer_1;
        WidgetRef<ui2Screen> MovieCareer_2;
        WidgetRef<ui2Screen> MovieCareer_3;
        WidgetRef<ui2Screen> MovieCareer_4;
        WidgetRef<ui2Screen> MovieCareer_5;
        WidgetRef<ui2Screen> MovieCareer_6;
        WidgetRef<ui2Screen> MovieCareer_7;
        WidgetRef<ui2Screen> MovieCareer_8;
        WidgetRef<ui2Screen> MovieCareer_9;
        WidgetRef<ui2Screen> MovieSRNews;
        WidgetRef<ui2Screen> loading;
        WidgetRef<ui2Screen> MemcardCheck;
        WidgetRef<ui2Screen> LanguageAtBoot;
        WidgetRef<ui2Variable> level_var;
        WidgetRef<ui2Variable> mission_var;
        WidgetRef<ui2Variable> carPl1_var;
        WidgetRef<ui2Variable> carMate1Pl1_var;
        WidgetRef<ui2Variable> carMate2Pl1_var;
        WidgetRef<ui2Variable> carMate3Pl1_var;
        WidgetRef<ui2Variable> carPl2_var;
        WidgetRef<ui2Variable> carMate1Pl2_var;
        WidgetRef<ui2Variable> carMate2Pl2_var;
        WidgetRef<ui2Variable> carMate3Pl2_var;
        WidgetRef<ui2Variable> skin1_var;
        WidgetRef<ui2Variable> skin2_var;
        WidgetRef<ui2Variable> humans_var;
        WidgetRef<ui2Variable> timeOfDay_var;
        WidgetRef<ui2Variable> police_var;
        WidgetRef<ui2Variable> team_game_var;
        WidgetRef<ui2Variable> transmission1_var;
        WidgetRef<ui2Variable> transmission2_var;
        WidgetRef<ui2Variable> regularStartupScreen_var;
        WidgetRef<ui2Variable> videoStartupScreen_var;
        WidgetRef<ui2Variable> quitfe_var;
    };

    class MenuUnk0 {
        public:
            MenuUnk0();

            bool isMapUnknown();
            const char* getMapName();
            const char* getGameModeName();
            const char* getMissionName();
            u32 getMissionCountForMode();
            bool advanceToNext();

            static const char* getCheckpointMissionName(const char* mapName, u32 missionIdx);
            static const char* getCNSMissionName(const char* mapName, u32 missionIdx);
            static const char* getLootgrabMissionName(const char* mapName, u32 missionIdx);
            static const char* getBombTagMissionName(const char* mapName, u32 missionIdx);
            static const char* getCareerMissionName(const char* mapName, u32 missionIdx);
            static u32 getCheckpointMissionCount(const char* mapName);
            static u32 getCNSMissionCount(const char* mapName);
            static u32 getLootgrabMissionCount(const char* mapName);
            static u32 getBombTagMissionCount(const char* mapName);
            static u32 getCareerMissionCount(const char* mapName);
            
            GAME_MODE gameMode;
            MAP_SELECTION mapId;
            u32 missionIdx;
    };

    struct MenuSelections {
        char teamCarsP1[32];
        char teamCarsP2[32];
        char teamSkinsP1[32];
        char teamSkinsP2[32];
        char mapName[32];
        char humans[32];
        char transmissions[32];
        char missionName[32];
        char numAI[32];
        char variant[32];

        MAP_SELECTION mapId;
    };

    class InMenuState : public GameState {
        public:
            InMenuState();
            ~InMenuState();

            virtual void Enter(GameEngine* eng);
            virtual void Exit();
            virtual void Input();
            virtual void Update();
            virtual void Reset(GameEngine* eng);
            virtual void Draw();
            virtual bool Done();

        protected:
            void displayUIInterest();
            void processLevelSelection();
            void processHumansSelection();
            bool processMissionSelection();
            i32 processTeamGameSelection();
            void processTeamCarSelections(i32 teamGameVar);
            void processTeamSkinSelections(i32 teamGameVar, bool isSinglePlayerMission);
            void processTransmissionSelections();
            void processPoliceSelection(bool isSinglePlayerMission);
            void processTimeOfDaySelection();
            
            MenuWidgets* m_widgets;
            WidgetRef<ui2Base> m_frontendBase;
            bool m_justBooted;
            u32 m_uiInterestIndex;
            char m_regularStartupScreenName[32];
            char m_videoStartupScreenName[32];
            MenuUnk0 m_unk0;
            MenuSelections m_selections;
            char m_debugString[256];
            u32 m_state;
    };
};