#include <map>
#include <vector>

#include "RoadEdge.h"
#include "RoadNode.h"
#include "RoadsMerger.h"

#include <iostream>
#include <ostream>

#include "../primitives/Edge.h"
#include "../primitives/Point.h"
#include "../geometry/HddEdge.h"
#include "../graph/utils.h"

RoadsMerger::RoadsMerger(
    const std::vector<HddEdge> &hddEdges,
    const std::vector<std::vector<Edge> > &componentsEdges
) {
    std::map<EdgeId, std::pair<NodeId, NodeId> > edgeEnds;
    NodeId nodeId = 0;


    for (const auto &componentEdges: componentsEdges) {
        for (int i = 0; i < componentEdges.size(); i++) {
            const auto currentEdge = componentEdges[i];
            const auto start = currentEdge.start;

            nodes[nodeId + i] = RoadNode{TrenchNode, start};

            edgeEnds[currentEdge.id] = std::make_pair(nodeId + i, nodeId + i + 1);

            if (i == 0) {
                nodesConnections[nodeId + i] = NodeConnections{
                    {Trench, nodeId + i + 1},
                    {Trench, nodeId + componentEdges.size() - 1}
                };
            } else if (i == componentEdges.size() - 1) {
                nodesConnections[nodeId + i] = NodeConnections{
                    {Trench, (nodeId + 1)},
                    {Trench, (nodeId + i - 1)}
                };
            } else {
                nodesConnections[nodeId + i] = NodeConnections{
                        {Trench, (nodeId + i + 1)},
                        {Trench, (nodeId + i - 1)}
                };
            }
        }

        nodeId = componentEdges.size() + nodeId;
    }


    for (auto &hddEdge: hddEdges) {
        const auto start = hddEdge.edge.start;
        const auto end = hddEdge.edge.end;

        const NodeId startId = nodeId + 1;
        const NodeId endId = nodeId + 2;
        nodes[startId] = {HDDNode, start};
        nodes[endId] = {HDDNode, end};

        edgeEnds[hddEdge.edge.id] = std::make_pair(startId, endId);

        nodesConnections[startId] = NodeConnections{{HDD, endId}};
        nodesConnections[endId] = NodeConnections{{HDD, startId}};

        nodeId += 2;
    }

    for (auto &hddEdge: hddEdges) {
        EdgeId i1 = hddEdge.fromId;
        EdgeId i2 = hddEdge.toId;

        EdgeId i3 = hddEdge.edge.id;

        auto [p1, p2] = edgeEnds[i1];
        auto [p3, p4] = edgeEnds[i2];

        auto [p5, p6] = edgeEnds[i3];

        nodes[p5] = {HDDNode, hddEdge.edge.end};
        nodes[p6] = {HDDNode, hddEdge.edge.start};

        auto p1Connections = nodesConnections[p1];
        auto p2Connections = nodesConnections[p2];
        auto p3Connections = nodesConnections[p3];
        auto p4Connections = nodesConnections[p4];

        auto pm = nodeId + 1;
        nodes[pm] = {TrenchNode, hddEdge.edge.start};

        auto pm1 = nodeId + 2;
        nodes[pm1] = {TrenchNode, hddEdge.edge.end};

        eraseNode(p2, Trench, p1Connections);
        eraseNode(p1, Trench, p2Connections);
        connectWithNode(pm, Trench, p1Connections);
        connectWithNode(pm, Trench, p2Connections);

        eraseNode(p3, Trench, p4Connections);
        eraseNode(p4, Trench, p3Connections);

        connectWithNode(pm1, Trench, p3Connections);
        connectWithNode(pm1, Trench, p4Connections);

        nodesConnections[pm] = std::vector{
            std::make_pair(Trench, p1),
            std::make_pair(Trench, p2),
            std::make_pair(Transition, p6)
        };
        nodesConnections[pm1] = std::vector{
            std::make_pair(Trench, p3),
            std::make_pair(Trench, p4),
            std::make_pair(Transition, p5)
        };

        nodesConnections[p5] = std::vector{
            std::make_pair(Transition, pm1),
            std::make_pair(HDD, p6),
        };
        nodesConnections[p6] = std::vector{
            std::make_pair(Transition, pm),
            std::make_pair(HDD, p5),
        };

        nodeId += 2;
    }
}

long double RoadsMerger::getTotalCost(
    const RoadConfig &roadConfig
) {
    long double result = 0;
    for (auto &[nodeId, nodeConnections]: nodesConnections) {
        auto [nodePoint, nodeType] = nodes[nodeId];

        for (auto &[edgeType, neighbourId]: nodeConnections) {
            auto [neighbourPoint, neighbourNodeType] = nodes[neighbourId];

            const long double edgeCost = calculateCost(roadConfig, nodePoint, neighbourPoint, edgeType);
            result += edgeCost;
        }
    }

    return result / 2.0;
}

std::map<NodeId, NodeConnections> RoadsMerger::getNodesConnections() {
    return nodesConnections;
}

std::map<NodeId, RoadNode> RoadsMerger::getNodes() {
    return nodes;
}

long double RoadsMerger::calculateCost(
    const RoadConfig &roadConfig,
    const Point &from,
    const Point &to,
    const RoadEdgeType edgeType
) {
    if (edgeType == Transition) {
        return roadConfig.getTransitionCost();
    }

    const double distance = utils::distance(from, to);
    if (edgeType == HDD) {
        return distance * roadConfig.getHddCost();
    }
    return distance * roadConfig.getTrenchCost();
}

void RoadsMerger::eraseNode(
    const NodeId nodeId,
    const RoadEdgeType edgeType,
    NodeConnections &nodeConnections
) {
    std::erase(nodeConnections, std::make_pair(edgeType, nodeId));
}

void RoadsMerger::connectWithNode(
    const NodeId nodeId,
    const RoadEdgeType edgeType,
    NodeConnections &nodeConnections
) {
    nodeConnections.emplace_back(edgeType, nodeId);
}
