#ifndef ROADS_ROADSMERGER_H
#define ROADS_ROADSMERGER_H

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

public:
    RoadsMerger(
        const std::vector<HddEdge> &hddEdges,
        const std::vector<std::vector<Edge> > &componentsEdges
    );

    long double getTotalCost(const RoadConfig &roadConfig);

    std::map<NodeId, NodeConnections> getNodesConnections();

    std::map<NodeId, RoadNode> getNodes();

private:
    static long double calculateCost(const RoadConfig &roadConfig, const Point &from, const Point &to,
                                     RoadEdgeType edgeType);

    static void eraseNode(NodeId nodeId, RoadEdgeType edgeType, NodeConnections &nodeConnections);

    static void connectWithNode(NodeId nodeId, RoadEdgeType edgeType, NodeConnections &nodeConnections);
};

#endif
