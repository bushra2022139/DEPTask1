#ifndef LOCATION_H
#define LOCATION_H
#include<string>

class Location {
public:
    std::string name;
    double latitude;
    double longitude;

    Location(std::string name, double latitude, double longitude);
};

#endif // LOCATION_H
