#include <map>
#include <ranges>
#include <vector>

#include "RoadEdge.h"
#include "RoadNode.h"
#include "RoadsMerger.h"

#include <iostream>
#include <set>

#include "../primitives/Edge.h"
#include "../primitives/Point.h"
#include "../geometry/HddEdge.h"
#include "../graph/utils.h"

RoadsMerger::RoadsMerger(
    const std::vector<HddEdge> &hddEdges,
    const std::vector<std::vector<Edge> > &componentsEdges,
    const RoadConfig &config
) : config(config) {
    std::map<EdgeId, std::pair<NodeId, NodeId> > edgeEnds;
    NodeId nodeId = 0;

    for (const auto &componentEdges: componentsEdges) {
        for (int i = 0; i < componentEdges.size(); i++) {
            const auto currentEdge = componentEdges[i];
            const auto start = currentEdge.start;

            nodes[nodeId + i] = RoadNode{TrenchNode, start};

            if (i == 0) {
                nodesConnections[nodeId + i] = NodeConnections{
                    {Trench, nodeId + i + 1},
                    {Trench, nodeId + componentEdges.size() - 1}
                };
                edgeEnds[currentEdge.id] = std::make_pair(nodeId + i, nodeId + i + 1);
            } else if (i == componentEdges.size() - 1) {
                nodesConnections[nodeId + i] = NodeConnections{
                    {Trench, nodeId},
                    {Trench, nodeId + i - 1}
                };
                edgeEnds[currentEdge.id] = std::make_pair(nodeId + i, nodeId + 1);
            } else {
                nodesConnections[nodeId + i] = NodeConnections{
                    {Trench, nodeId + i + 1},
                    {Trench, nodeId + i - 1}
                };
                edgeEnds[currentEdge.id] = std::make_pair(nodeId + i, nodeId + i + 1);
            }
        }

        nodeId = componentEdges.size() + nodeId;
    }


    for (const auto &hddEdge: hddEdges) {
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

    for (const auto &hddEdge: hddEdges) {
        EdgeId i1 = hddEdge.fromId;
        EdgeId i2 = hddEdge.toId;

        EdgeId i3 = hddEdge.edge.id;

        auto [p1, p2] = edgeEnds[i1];
        auto [p3, p4] = edgeEnds[i2];

        auto [p5, p6] = edgeEnds[i3];

        nodes[p5] = {HDDNode, hddEdge.edge.end};
        nodes[p6] = {HDDNode, hddEdge.edge.start};

        auto &p1Connections = nodesConnections[p1];
        auto &p2Connections = nodesConnections[p2];
        auto &p3Connections = nodesConnections[p3];
        auto &p4Connections = nodesConnections[p4];

        if (config.getGraphType() == Default) {
            auto pm = nodeId + 1;
            nodes[pm] = {TrenchNode, hddEdge.edge.start};

            auto pm1 = nodeId + 2;
            nodes[pm1] = {TrenchNode, hddEdge.edge.end};

            eraseTrenchNode(p2, p1Connections);
            eraseTrenchNode(p1, p2Connections);
            connectWithTrenchNode(pm, p1Connections);
            connectWithTrenchNode(pm, p2Connections);

            eraseTrenchNode(p3, p4Connections);
            eraseTrenchNode(p4, p3Connections);

            connectWithTrenchNode(pm1, p3Connections);
            connectWithTrenchNode(pm1, p4Connections);

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
        } else {
            eraseTrenchNode(p2, p1Connections);
            eraseTrenchNode(p1, p2Connections);
            connectWithTrenchNode(p6, p1Connections);
            connectWithTrenchNode(p6, p2Connections);

            eraseTrenchNode(p3, p4Connections);
            eraseTrenchNode(p4, p3Connections);

            connectWithTrenchNode(p5, p3Connections);
            connectWithTrenchNode(p5, p4Connections);

            nodesConnections[p5] = std::vector{
                std::make_pair(Trench, p3),
                std::make_pair(Trench, p4),
                std::make_pair(HDD, p6),
            };
            nodesConnections[p6] = std::vector{
                std::make_pair(Trench, p1),
                std::make_pair(Trench, p2),
                std::make_pair(HDD, p5),
            };
        }
    }

    if (config.getGraphType() == HddOnly) {
        splitTrenchEdgesToHDD(nodeId);
    }
}

void RoadsMerger::splitTrenchEdgesToHDD(
    NodeId &currentNodeId
) {
    std::map<NodeId, NodeConnections> newConnections;
    std::set<std::pair<NodeId, NodeId> > processedTrenchEdges;

    for (const auto &[nodeId, connections]: nodesConnections) {
        NodeConnections updatedConnections;

        for (const auto &[edgeType, connectedNodeId]: connections) {
            if (edgeType == Trench) {
                auto edgeKey = nodeId < connectedNodeId
                                   ? std::make_pair(nodeId, connectedNodeId)
                                   : std::make_pair(connectedNodeId, nodeId);

                if (!processedTrenchEdges.contains(edgeKey)) {
                    processedTrenchEdges.insert(edgeKey);

                    auto hddChain = splitTrenchToHDD(nodeId, connectedNodeId, currentNodeId);

                    updateConnectionsWithHDDChain(newConnections, nodeId, connectedNodeId, hddChain, true);
                    updateConnectionsWithHDDChain(newConnections, connectedNodeId, nodeId, hddChain, false);
                }
            } else {
                updatedConnections.emplace_back(edgeType, connectedNodeId);
                newConnections[nodeId].emplace_back(edgeType, connectedNodeId);
            }
        }
    }

    nodesConnections = std::move(newConnections);
}

RoadsMerger::HDDChain RoadsMerger::splitTrenchToHDD(
    const NodeId startNodeId,
    const NodeId endNodeId,
    NodeId &nodeId
) {
    HDDChain chain;
    chain.startNodeId = startNodeId;
    chain.endNodeId = endNodeId;

    const RoadNode &startNode = nodes.at(startNodeId);
    const RoadNode &endNode = nodes.at(endNodeId);

    const double distance = calculateDistance(startNode, endNode);

    if (const int numSegments = calculateNumberOfHDDSegments(distance); numSegments == 1) {
        return chain;
    } else {
        for (int i = 1; i < numSegments; i++) {
            const double t = static_cast<double>(i) / numSegments;
            const RoadNode intermediateNode = createIntermediateNode(startNode, endNode, t);
            NodeId intermediateNodeId = ++nodeId;

            nodes[intermediateNodeId] = intermediateNode;
            chain.intermediateNodes.push_back(intermediateNodeId);
        }
    }

    return chain;
}

void RoadsMerger::updateConnectionsWithHDDChain(
    std::map<NodeId, NodeConnections> &newConnections,
    const NodeId fromNodeId,
    const NodeId toNodeId,
    const HDDChain &chain,
    const bool forwardDirection
) {
    if (chain.intermediateNodes.empty()) {
        newConnections[fromNodeId].emplace_back(HDD, toNodeId);
    } else {
        const std::vector<NodeId> &intermediates = chain.intermediateNodes;

        if (forwardDirection) {
            NodeId currentNode = fromNodeId;

            for (NodeId nextNode: intermediates) {
                newConnections[currentNode].emplace_back(HDD, nextNode);
                currentNode = nextNode;
            }
            newConnections[currentNode].emplace_back(HDD, toNodeId);
        } else {
            NodeId currentNode = fromNodeId;

            for (unsigned long long intermediate: std::ranges::reverse_view(intermediates)) {
                newConnections[currentNode].emplace_back(HDD, intermediate);
                currentNode = intermediate;
            }
            newConnections[currentNode].emplace_back(HDD, toNodeId);
        }
    }
}

int RoadsMerger::calculateNumberOfHDDSegments(const double distance) const {
    const double maxHDDLength = config.getHddMaxLength();
    return std::max(1, static_cast<int>(std::ceil(distance / maxHDDLength)));
}

double RoadsMerger::calculateDistance(const RoadNode &node1, const RoadNode &node2) {
    return utils::distance(node1.point, node2.point);
}

RoadNode RoadsMerger::createIntermediateNode(
    const RoadNode &start,
    const RoadNode &end,
    const double t
) {
    const auto intermediateX = start.point.x + t * (end.point.x - start.point.x);
    const auto intermediateY = start.point.y + t * (end.point.y - start.point.y);
    return {HDDNode, Point{intermediateX, intermediateY}};
}

long double RoadsMerger::getTotalCost() {
    long double result = 0;

    for (const auto &[nodeId, nodeConnections]: nodesConnections) {
        const auto &[nodePoint, nodeType] = nodes[nodeId];

        for (const auto &[edgeType, neighbourId]: nodeConnections) {
            if (nodeId >= neighbourId) continue;

            const auto &[neighbourPoint, neighbourNodeType] = nodes[neighbourId];
            const long double edgeCost = calculateCost(nodePoint, neighbourPoint, edgeType);

            result += edgeCost;
        }
    }

    return result;
}

std::map<NodeId, NodeConnections> RoadsMerger::getNodesConnections() {
    return nodesConnections;
}

std::map<NodeId, RoadNode> RoadsMerger::getNodes() {
    return nodes;
}

long double RoadsMerger::calculateCost(
    const Point &from,
    const Point &to,
    const RoadEdgeType edgeType
) const {
    if (edgeType == Transition) {
        return config.getTransitionCost();
    }

    const double distance = utils::distance(from, to);
    if (edgeType == HDD) {
        return distance * config.getHddCost();
    }
    return distance * config.getTrenchCost();
}

void RoadsMerger::eraseTrenchNode(
    const NodeId nodeId,
    NodeConnections &nodeConnections
) {
    std::erase(nodeConnections, std::make_pair(Trench, nodeId));
}

void RoadsMerger::connectWithTrenchNode(
    const NodeId nodeId,
    NodeConnections &nodeConnections
) {
    nodeConnections.emplace_back(Trench, nodeId);
}
