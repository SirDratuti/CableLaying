#pragma once

#include "../primitives/Edge.h"
#include "HddEdge.h"
#include "../config/RoadConfig.h"

class HddResolver {
public:
    static bool resolveHddOptimization(
        const Edge &edge1,
        const Edge &edge2,
        const RoadConfig &roadConfig,
        HddEdge &result
    );
};
