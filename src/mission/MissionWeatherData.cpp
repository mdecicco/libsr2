#include <libsr2/mission/MissionWeatherData.h>
#include <libsr2/io/parCsvFile.h>
#include <libsr2/utilities/utils.h>

#include <string.h>

namespace sr2 {
    WEATHER_TYPE MissionWeatherData::CurrentWeatherType = weather_unk0;
    MissionWeatherData::MissionWeatherData() {
        m_entry_count = 0;
    }

    MissionWeatherData::~MissionWeatherData() {

    }

    bool MissionWeatherData::load(const char* map_name) {
        parCsvFile csv;
        std::string fn = std::string(map_name) + "_weather";
        if (!csv.load("mission", fn.c_str(), 1, 1)) return false;
        m_entry_count = csv.rowCount();
        for (u32 i = 0;i < m_entry_count;i++) {
            strncpy(m_missions[i].mission, csv.getCell(i, 0), 32);
            m_missions[i].weather = (WEATHER_TYPE)csv.getInt(i, 1);
        }

        return true;
    }
    
    WEATHER_TYPE MissionWeatherData::getWeatherVariant(const char* missionName) const {
        char buf[32] = { 0 };
        snprintf(buf, 32, "%s", missionName);
        char* p = strchr(buf, '.');
        if (!p) return weather_unk0;

        *p = 0;

        for (u32 i = 0;i < m_entry_count;i++) {
            if (strncmp(buf, m_missions[i].mission, 32) == 0) return m_missions[i].weather;
        }

        return weather_unk0;
    }
};