#ifndef ROADS_COMPONENT_H
#define ROADS_COMPONENT_H

#include <vector>
#include "../primitives/Edge.h"

typedef int ComponentId;

class Component {
public:
    ComponentId componentId;
    std::vector<Edge> edges;

    Component(
        const ComponentId componentId,
        const std::vector<Edge> &edges
    ) : componentId(componentId), edges(edges) {
    }
};

#endif
