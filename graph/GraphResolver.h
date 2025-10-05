#ifndef ROADS_GRAPHRESOLVER_H
#define ROADS_GRAPHRESOLVER_H

#include <vector>

#include "ComponentEdge.h"
#include "../geometry/Types.h"

class GraphResolver {
public:
    static std::vector<ComponentEdge> resolveGraph(
        const std::vector<std::vector<Edge> > &componentEdges,
        double distanceMin,
        double distanceMax,
        double angleTolerance
    );
    static std::vector<std::vector<Edge> > ringsToEdges(const std::vector<BG_Ring> &rings, int &edgeId);
    static std::vector<Edge> pointsToEdges(const std::vector<Point> &points, int &edgeId);
};

#endif
