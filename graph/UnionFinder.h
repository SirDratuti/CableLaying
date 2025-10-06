#pragma once

#include <map>

#include "Component.h"

class UnionFinder {
    std::map<ComponentId, ComponentId> parent;
    std::map<ComponentId, int> rank;

public:
    ComponentId find(ComponentId x);

    void unite(ComponentId x, ComponentId y);
};
