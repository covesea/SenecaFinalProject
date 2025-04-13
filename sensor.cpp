#include "sensor.hpp"
#include <fstream>
#include <stdexcept>

float read_temperature_from_sysfs(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    float celsius;
    file >> celsius;
    return celsius;
}