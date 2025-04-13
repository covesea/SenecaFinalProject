#include "config.hpp"
#include <fstream>
#include <stdexcept>

nlohmann::json load_config(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + path);
    }

    nlohmann::json j;
    file >> j;
    return j;
}