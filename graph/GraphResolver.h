#pragma once

#include <vector>

#include "ComponentEdge.h"
#include "../config/RoadConfig.h"
#include "../geometry/Types.h"

class GraphResolver {
public:
    static std::vector<ComponentEdge> resolveGraph(
        const std::vector<std::vector<Edge> > &componentEdges,
        const RoadConfig &roadConfig
    );
    static std::vector<std::vector<Edge> > ringsToEdges(const std::vector<BG_Ring> &rings, int &edgeId);
    static std::vector<Edge> pointsToEdges(const std::vector<Point> &points, int &edgeId);
};
