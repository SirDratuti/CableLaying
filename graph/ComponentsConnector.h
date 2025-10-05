#ifndef ROADS_COMPONENTSCONNECTOR_H
#define ROADS_COMPONENTSCONNECTOR_H

#include <map>
#include <vector>
#include "Component.h"
#include "ComponentEdge.h"

class ComponentsConnector {
public:
    static std::vector<ComponentEdge> connectGraph(
        const std::vector<Component> &components,
        double distanceMin,
        double distanceMax,
        double angleTolerance
    );

private:
    static std::vector<ComponentEdge>
    connectGraph(const std::map<ComponentId, std::map<ComponentId, HddEdge> > &graph);

    static std::map<ComponentId, std::map<ComponentId, HddEdge> >
    constructGraph(
        const std::vector<Component> &components,
        double distanceMin, double distanceMax, double angleTolerance);

    static bool findShortestEdge(
        const Component &component1, const Component &component2,
        double angleTolerance, double distanceMin, double distanceMax,
        HddEdge &resultEdge);
};

#endif
