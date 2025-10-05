#include "GraphResolver.h"

#include <vector>

#include "Component.h"
#include "ComponentsConnector.h"
#include "../geometry/Types.h"


std::vector<ComponentEdge>
GraphResolver::resolveGraph(
    const std::vector<std::vector<Edge> > &componentEdges,
    const double distanceMin,
    const double distanceMax,
    const double angleTolerance
) {
    std::vector<Component> components;
    int componentId = 0;

    for (const std::vector<Edge> &componentEdge: componentEdges) {
        components.emplace_back(componentId++, componentEdge);
    }

    auto componentsEdges = ComponentsConnector::connectGraph(
        components,
        distanceMin,
        distanceMax,
        angleTolerance
    );

    return componentsEdges;
}

std::vector<std::vector<Edge> >
GraphResolver::ringsToEdges(const std::vector<BG_Ring> &rings, int &edgeId) {
    std::vector<std::vector<Edge> > componentEdges;
    for (const BG_Ring &ring: rings) {
        std::vector<Point> points;

        for (const auto &point: ring) {
            points.emplace_back(
                boost::geometry::get<0>(point),
                boost::geometry::get<1>(point)
            );
        }

        auto edges = pointsToEdges(points, edgeId);
        componentEdges.push_back(edges);
    }

    return componentEdges;
}

std::vector<Edge>
GraphResolver::pointsToEdges(const std::vector<Point> &points, int &edgeId) {
    std::vector<Edge> edges;
    const size_t n = points.size();

    for (int i = 0; i < n; ++i) {
        edges.emplace_back(edgeId++, points[i], points[(i + 1) % n]);
    }

    return edges;
}
