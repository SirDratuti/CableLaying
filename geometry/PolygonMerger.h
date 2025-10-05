#ifndef ROADS_POLYGONMERGER_H
#define ROADS_POLYGONMERGER_H

#include "Types.h"

class PolygonMerger {
public:
    static BG_MultiPolygon merge(const std::vector<BG_Polygon>& polygons);
};

#endif