#pragma once

#include "nlohmann/json.hpp"
#include "../geometry/Types.h"

class GeoJsonParser {
public:
    static BG_MultiPolygon parsePolygons(const nlohmann::json &jsonData);

private:
    static BG_Polygon parsePolygon(const nlohmann::json &geometry);
};
