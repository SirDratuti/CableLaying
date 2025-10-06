#pragma once

#include "RoadNode.h"

enum RoadEdgeType {
    Trench, HDD, Transition
};

class RoadEdge {
public:
    RoadNode from;
    RoadNode to;
    RoadEdgeType type;

    RoadEdge(const RoadNode &from, const RoadNode &to, const RoadEdgeType type) : from(from), to(to), type(type) {
    }
};
