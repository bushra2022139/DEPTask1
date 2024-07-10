#ifndef HISTORICAL_WEATHER_SYSTEM_H
#define HISTORICAL_WEATHER_SYSTEM_H

#include "location.h"
#include <nlohmann/json.hpp>  // Include nlohmann/json.hpp for JSON parsing

class HistoricalWeatherSystem {
public:
    nlohmann::json fetchHistoricalData(const Location& location, const std::string& start_date, const std::string& end_date);
    void displayHistoricalData(const nlohmann::json& historicalData);
    void exportToCSV(const nlohmann::json& data, const std::string& filename);
    void exportToJSON(const nlohmann::json& data, const std::string& filename);
};

#endif // HISTORICAL_WEATHER_SYSTEM_H
