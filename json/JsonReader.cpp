#include "JsonReader.h"

#include <fstream>
#include "nlohmann/json.hpp"

nlohmann::json JsonReader::loadFromFile(const std::string &filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        nlohmann::json jsonData;
        file >> jsonData;
        return jsonData;
    } catch (const std::exception &e) {
        throw std::runtime_error("Error loading GeoJSON: " + std::string(e.what()));
    }
}
