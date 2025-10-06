#include "ComponentsConnector.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "Component.h"
#include "ComponentEdge.h"
#include "UnionFinder.h"
#include "../config/RoadConfig.h"
#include "../geometry/HddResolver.h"


std::vector<ComponentEdge>
ComponentsConnector::connectGraph(
    const std::vector<Component> &components,
    const RoadConfig &roadConfig
) {
    const auto graph = constructGraph(components, roadConfig);
    return connectGraph(graph);
}

std::vector<ComponentEdge>
ComponentsConnector::connectGraph(const std::map<ComponentId, std::map<ComponentId, HddEdge> > &graph) {
    std::vector<ComponentEdge> edges;
    std::set<ComponentId> nodes;

    for (const auto &[from, neighbors]: graph) {
        nodes.insert(from);
        for (const auto &[v, hddEdge]: neighbors) {
            nodes.insert(v);
            edges.emplace_back(from, v, hddEdge);
        }
    }

    std::sort(edges.begin(), edges.end());

    UnionFinder unionFinder;
    std::vector<ComponentEdge> componentEdges;

    for (const auto &edge: edges) {
        if (unionFinder.find(edge.from) != unionFinder.find(edge.to)) {
            unionFinder.unite(edge.from, edge.to);
            componentEdges.push_back(edge);

            if (componentEdges.size() == nodes.size() - 1) {
                break;
            }
        }
    }


    return componentEdges;
}

std::map<ComponentId, std::map<ComponentId, HddEdge> >
ComponentsConnector::constructGraph(
    const std::vector<Component> &components,
    const RoadConfig &roadConfig
) {
    std::map<ComponentId, std::map<ComponentId, HddEdge> > graph;

    for (int i = 0; i < components.size(); i++) {
        const auto &component1 = components[i];
        std::map<ComponentId, HddEdge> componentGraph;

        for (int j = 0; j < components.size(); j++) {
            if (i == j) continue;
            const auto &component2 = components[j];

            if (auto shortestEdge = HddEdge{};
                findShortestEdge(component1, component2, roadConfig, shortestEdge)) {
                componentGraph[component2.componentId] = shortestEdge;
            }
        }

        graph[component1.componentId] = componentGraph;
    }

    return graph;
}

bool ComponentsConnector::findShortestEdge(
    const Component &component1,
    const Component &component2,
    const RoadConfig &roadConfig,
    HddEdge &resultEdge
) {
    const auto edges1 = component1.edges;
    const auto edges2 = component2.edges;

    auto answerEdge = HddEdge{};
    bool exists = false;

    for (const auto &edge1: edges1) {
        for (const auto &edge2: edges2) {
            if (HddEdge tmpEdge = {}; HddResolver::resolveHddOptimization(edge1, edge2, roadConfig, tmpEdge)) {
                const double length = utils::edgeLength(tmpEdge.edge);
                const double answerLength = utils::edgeLength(answerEdge.edge);

                if (!exists || answerLength > length) {
                    answerEdge = tmpEdge;
                }

                exists = true;
            }
        }
    }

    if (exists) {
        resultEdge = answerEdge;
        return true;
    }

    return false;
}
