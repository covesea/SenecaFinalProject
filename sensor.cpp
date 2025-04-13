#include "sensor.hpp"
#include <fstream>
#include <stdexcept>

float read_temperature_from_sysfs(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    int milli_celsius;
    file >> milli_celsius;
    return milli_celsius / 1000.0f;
}