#ifndef WEATHER_VARIABLE_H
#define WEATHER_VARIABLE_H

#include <unordered_map>
#include <string>

class WeatherVariable {
private:
    std::unordered_map<std::string, double> variables;

public:
    void addVariable(const std::string& name, double value);
    void removeVariable(const std::string& name);
    void listVariables() const;
};

#endif // WEATHER_VARIABLE_H
