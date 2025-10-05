#ifndef ROADS_UNIONFINDER_H
#define ROADS_UNIONFINDER_H

#include <map>

#include "Component.h"

class UnionFinder {
    std::map<ComponentId, ComponentId> parent;
    std::map<ComponentId, int> rank;

public:
    ComponentId find(ComponentId x);

    void unite(ComponentId x, ComponentId y);
};

#endif
