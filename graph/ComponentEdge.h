#ifndef ROADS_COMPONENTEDGE_H
#define ROADS_COMPONENTEDGE_H

#include "Component.h"
#include "utils.h"
#include "../geometry/HddEdge.h"

class ComponentEdge {
public:
    ComponentId from;
    ComponentId to;
    HddEdge edge;

    ComponentEdge(
        const ComponentId from,
        const ComponentId to,
        const HddEdge &edge
    ) : from(from), to(to), edge(edge) {
    }

    bool operator<(const ComponentEdge &other) const {
        return utils::edgeLength(edge.edge) < utils::edgeLength(other.edge.edge);
    }
};

#endif
