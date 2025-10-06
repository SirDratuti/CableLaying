#pragma once

#include "../primitives/Point.h"

enum RoadNodeType {
    TrenchNode, HDDNode
};

class RoadNode {
public:
    Point point;
    RoadNodeType nodeType;

    RoadNode() : nodeType(TrenchNode) {}

    RoadNode(
        const RoadNodeType nodeType,
        const Point &point
    ) : point(point), nodeType(nodeType) {
    }
};
