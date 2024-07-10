#include "historical_weather_system.h"
#include <cpr/cpr.h> // Include cpr/cpr.h for making HTTP requests
#include <fstream>
#include <iostream>

nlohmann::json HistoricalWeatherSystem::fetchHistoricalData(const Location& location, const std::string& start_date, const std::string& end_date) {
    std::string url = "https://api.open-meteo.com/v1/history?latitude=" + std::to_string(location.latitude) +
                      "&longitude=" + std::to_string(location.longitude) + "&start_date=" + start_date + "&end_date=" + end_date;
    auto response = cpr::Get(cpr::Url{url});
    auto json_data = nlohmann::json::parse(response.text);
    return json_data;
}

void HistoricalWeatherSystem::displayHistoricalData(const nlohmann::json& historicalData) {
    std::cout << "Historical Data:\n" << historicalData.dump(4) << std::endl;
}

void HistoricalWeatherSystem::exportToCSV(const nlohmann::json& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (auto& element : data.items()) {
            file << element.key() << "," << element.value() << "\n";
        }
        file.close();
    }
}

void HistoricalWeatherSystem::exportToJSON(const nlohmann::json& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);
        file.close();
    }
}
