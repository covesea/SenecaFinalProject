#pragma once

#include <nlohmann/json.hpp>
#include <string>

float to_fahrenheit(float c);
float to_celsius(float f);

float convert_using_json(const nlohmann::json& config, float value);
