#include "PolygonMerger.h"

#include "Types.h"

BG_MultiPolygon
PolygonMerger::merge(const std::vector<BG_Polygon> &polygons) {
    if (polygons.empty()) {
        return {};
    }

    std::vector<BG_Polygon> fixedPolygons;
    for (const auto &poly: polygons) {
        BG_Polygon fixed = poly;
        if (!boost::geometry::is_valid(fixed)) {
            boost::geometry::correct(fixed);
        }
        fixedPolygons.push_back(fixed);
    }

    BG_MultiPolygon mergedMultipolygon{fixedPolygons[0]};

    for (size_t i = 1; i < fixedPolygons.size(); ++i) {
        BG_MultiPolygon tempResult;
        boost::geometry::union_(mergedMultipolygon, fixedPolygons[i], tempResult);
        mergedMultipolygon = tempResult;
    }

    return mergedMultipolygon;
}
