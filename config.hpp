#pragma once
#include <string>
#include <nlohmann/json.hpp>

nlohmann::json load_config(const std::string& path);