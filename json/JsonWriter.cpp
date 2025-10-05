#include <fstream>
#include <nlohmann/json.hpp>

class JsonWriter {
public:
    static void saveToFile(const nlohmann::json &geoJson, const std::string &filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }

        file << geoJson.dump(4);
        file.close();
    }

    static std::string toString(const nlohmann::json &geoJson, const bool pretty = false) {
        return pretty ? geoJson.dump(4) : geoJson.dump();
    }
};
