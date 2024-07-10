#include "weather_forecasting_system.h"
#include <cpr/cpr.h> // Include cpr/cpr.h for making HTTP requests
#include <fstream>
#include <iostream>

nlohmann::json WeatherForecastingSystem::fetchWeatherData(const Location& location) {
    std::string url = "https://api.open-meteo.com/v1/forecast?latitude=" + std::to_string(location.latitude) +
                      "&longitude=" + std::to_string(location.longitude) + "&hourly=temperature_2m";
    auto response = cpr::Get(cpr::Url{url});
    auto json_data = nlohmann::json::parse(response.text);
    return json_data;
}

void WeatherForecastingSystem::displayWeatherData(const nlohmann::json& weatherData) {
    std::cout << "Weather Data:\n" << weatherData.dump(4) << std::endl;
}

void WeatherForecastingSystem::exportToCSV(const nlohmann::json& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (auto& element : data.items()) {
            file << element.key() << "," << element.value() << "\n";
        }
        file.close();
    }
}

void WeatherForecastingSystem::exportToJSON(const nlohmann::json& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);
        file.close();
    }
}
