#include "RoadConfigParser.h"

#include <nlohmann/json.hpp>

#include "RoadConfig.h"

RoadConfig RoadConfigParser::parseRoadConfig(const nlohmann::json &jsonData) {
    try {
        const double trenchCost = jsonData["trenchCost"];
        const auto hddCost = parseOrDefault<double>(jsonData, "hddCost", trenchCost * 2);
        const double transitionCost = jsonData["transitionCost"];
        const auto hddMinLength = parseOrDefault<double>(jsonData, "hddMinLength", DEFAULT_HDD_MIN_LENGTH);
        const auto hddMaxLength = parseOrDefault<double>(jsonData, "hddMaxLength", DEFAULT_HDD_MAX_LENGTH);
        const double alpha = jsonData["alpha"];

        const GraphType graphType = resolveGraphType(
            parseOrDefault<bool>(jsonData, "hddOnly", false));

        return RoadConfig{
            trenchCost,
            hddCost,
            transitionCost,
            hddMinLength,
            hddMaxLength,
            alpha,
            graphType
        };
    } catch (const std::exception &e) {
        throw std::runtime_error("Error parsing road config: " + std::string(e.what()));
    }
}

GraphType RoadConfigParser::resolveGraphType(const bool onlyHdd) {
    return onlyHdd ? HddOnly : Default;
}
