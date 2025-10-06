#pragma once
#include <string>
#include <nlohmann/json.hpp>

#include "RoadConfig.h"

class RoadConfigParser {
public:
    static RoadConfig parseRoadConfig(const nlohmann::json &jsonData);

private:
    static GraphType resolveGraphType(const bool onlyHdd);

    template<typename T>
    static T parseOrDefault(
        const nlohmann::json &jsonData,
        const std::string &name,
        const T defaultValue
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
