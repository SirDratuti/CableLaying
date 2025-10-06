#include "JsonWriter.h"

#include <fstream>
#include <nlohmann/json.hpp>

void JsonWriter::saveToFile(const nlohmann::json &geoJson, const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file << geoJson.dump(4);
    file.close();
}
