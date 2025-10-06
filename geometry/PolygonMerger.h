#pragma once

#include "Types.h"

class PolygonMerger {
public:
    static BG_MultiPolygon merge(const std::vector<BG_Polygon>& polygons);
};
