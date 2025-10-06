#pragma once

#include <map>
#include <vector>
#include "Component.h"
#include "ComponentEdge.h"
#include "../config/RoadConfig.h"

class ComponentsConnector {
public:
    static std::vector<ComponentEdge> connectGraph(
        const std::vector<Component> &components,
        const RoadConfig &roadConfig
    );

private:
    static std::vector<ComponentEdge>
    connectGraph(const std::map<ComponentId, std::map<ComponentId, HddEdge> > &graph);

    static std::map<ComponentId, std::map<ComponentId, HddEdge> >
    constructGraph(const std::vector<Component> &components, const RoadConfig &roadConfig);

    static bool findShortestEdge(
        const Component &component1, const Component &component2,
        const RoadConfig &roadConfig, HddEdge &resultEdge);
};
