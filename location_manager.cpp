#include "location_manager.h"
#include <iostream>
#include <algorithm>

void LocationManager::addLocation(const Location& location) {
    locations.push_back(location);
}

void LocationManager::removeLocation(const std::string& name) {
    locations.erase(
        std::remove_if(locations.begin(), locations.end(),
            [&name](const Location& location) { return location.name == name; }),
        locations.end());
}

void LocationManager::listLocations() const {
    for (const auto& location : locations) {
        std::cout << "Name: " << location.name
                  << ", Latitude: " << location.latitude
                  << ", Longitude: " << location.longitude << std::endl;
    }
}
