#ifndef ROADS_HDDRESOLVER_H
#define ROADS_HDDRESOLVER_H

#include "../primitives/Edge.h"
#include "HddEdge.h"

class HddResolver {
public:
    static bool resolveHddOptimization(
        const Edge &edge1,
        const Edge &edge2,
        double angleTolerance,
        double distanceMin,
        double distanceMax,
        HddEdge &result
    );
};

#endif
