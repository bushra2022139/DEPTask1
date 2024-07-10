#include "weather_variable.h"
#include <iostream>

void WeatherVariable::addVariable(const std::string& name, double value) {
    variables[name] = value;
}

void WeatherVariable::removeVariable(const std::string& name) {
    variables.erase(name);
}

void WeatherVariable::listVariables() const {
    for (const auto& variable : variables) {
        std::cout << "Variable: " << variable.first
                  << ", Value: " << variable.second << std::endl;
    }
}
