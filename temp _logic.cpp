#include "temp_logic.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>

float to_fahrenheit(float c) {
    return (c * 9.0f / 5.0f) + 32.0f;
}

float to_celsius(float f) {
    return (f - 32.0f) * 5.0f / 9.0f;
}

float convert_using_json(const nlohmann::json& config, float value) {
    return value; // sensor values are in Celsius; defer conversion to UI logic
}