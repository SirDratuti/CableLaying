#pragma once
#include <nlohmann/json_fwd.hpp>

class JsonWriter {
public:
    static void saveToFile(const nlohmann::json &geoJson, const std::string &filename);
};
