#include <nlohmann/json.hpp>

#include "../config/RoadConfig.h"

class RoadConfigParser {
public:
    static RoadConfig parseRoadConfig(const nlohmann::json &jsonData) {
        try {
            const double trenchCost = jsonData["trenchCost"];
            const double hddCost = parseOrDefault(jsonData, "hddCost", trenchCost * 2);
            const double transitionCost = jsonData["transitionCost"];
            const double hddMinLength = parseOrDefault(jsonData, "hddMinLength", DEFAULT_HDD_MIN_LENGTH);
            const double hddMaxLength = parseOrDefault(jsonData, "hddMaxLength", DEFAULT_HDD_MAX_LENGTH);
            const double alpha = jsonData["alpha"];

            return RoadConfig{
                trenchCost,
                hddCost,
                transitionCost,
                hddMinLength,
                hddMaxLength,
                alpha
            };
        } catch (const std::exception &e) {
            throw std::runtime_error("Error parsing road config: " + std::string(e.what()));
        }
    }

private:
    static double parseOrDefault(
        const nlohmann::json &jsonData,
        const std::string &name,
        const double defaultValue
    ) {
        try {
            return jsonData[name];
        } catch (...) {
            return defaultValue;
        }
    }

    static constexpr double DEFAULT_HDD_MIN_LENGTH = 30;
    static constexpr double DEFAULT_HDD_MAX_LENGTH = 150;
};
