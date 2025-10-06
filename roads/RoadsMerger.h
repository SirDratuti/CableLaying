#pragma once

#include <map>
#include <vector>
#include "../config/RoadConfig.h"
#include "RoadEdge.h"
#include "../primitives/Edge.h"
#include "../geometry/HddEdge.h"

typedef unsigned long long NodeId;
typedef std::vector<std::pair<RoadEdgeType, NodeId> > NodeConnections;

class RoadsMerger {
    std::map<NodeId, NodeConnections> nodesConnections;
    std::map<NodeId, RoadNode> nodes;
    const RoadConfig &config;

public:
    RoadsMerger(
        const std::vector<HddEdge> &hddEdges,
        const std::vector<std::vector<Edge> > &componentsEdges,
        const RoadConfig &config
    );

    long double getTotalCost();

    std::map<NodeId, NodeConnections> getNodesConnections();

    std::map<NodeId, RoadNode> getNodes();

private:
    void splitTrenchNodes(NodeId &currentNodeId);

    std::vector<std::pair<RoadEdgeType, NodeId>> splitTrenchToHDD(NodeId startNodeId, NodeId endNodeId, NodeId &currentNodeId);

    int calculateNumberOfHDDSegments(double distance) const;

    long double calculateCost(const Point &from, const Point &to, RoadEdgeType edgeType) const;

    static double calculateDistance(const RoadNode &node1, const RoadNode &node2);

    static RoadNode createIntermediateNode(const RoadNode &start, const RoadNode &end, double t);

    static void eraseTrenchNode(NodeId nodeId, NodeConnections &nodeConnections);

    static void connectWithTrenchNode(NodeId nodeId, NodeConnections &nodeConnections);
};
