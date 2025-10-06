#include "HddResolver.h"

#include "../primitives/Edge.h"
#include "../primitives/Point.h"
#include "../graph/MinimalEdgeFinder.h"
#include "HddEdge.h"

bool HddResolver::resolveHddOptimization(
    const Edge &edge1,
    const Edge &edge2,
    const RoadConfig &roadConfig,
    HddEdge &result
) {
    const auto finder = MinimalEdgeFinder{edge1, edge2, roadConfig};

    Point p1{}, p2{};

    double bestLength;
    double angle;

    const bool isExist = finder.findMinimalEdge(p1, p2, bestLength, angle);
    if (isExist) {
        result = {edge1.id, edge2.id, Edge{p1, p2}};
    }
    return isExist;
}
