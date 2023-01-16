#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class MissionWeatherData {
        public:
            MissionWeatherData();
            ~MissionWeatherData();

            bool load(const char* map_name);
            WEATHER_TYPE getWeatherVariant(const char* missionName) const;

            static WEATHER_TYPE CurrentWeatherType;
        protected:
            u32 m_entry_count;
            struct {
                char mission[32];
                WEATHER_TYPE weather;
            } m_missions[32];
    };
};