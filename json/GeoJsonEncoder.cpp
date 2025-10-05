#include "GeoJsonEncoder.h"

#include <vector>
#include <boost/geometry.hpp>
#include <nlohmann/json.hpp>

#include "../geometry/Types.h"
#include "../roads/RoadsMerger.h"

nlohmann::json GeoJsonEncoder::graphToLayeredFeaturesCollection(
    const std::map<NodeId, RoadNode> &nodes,
    const std::map<NodeId, NodeConnections> &connections
) {
    nlohmann::json features = nlohmann::json::array();

    features.push_back(createMultiPointLayer(nodes, TrenchNode, "Trench Nodes"));
    features.push_back(createMultiPointLayer(nodes, HDDNode, "HDD Nodes"));

    createMultiLineStringLayers(nodes, connections, features);

    return {
        {"type", "FeatureCollection"},
        {"crs", getCrs()},
        {"features", features}
    };
}

nlohmann::json GeoJsonEncoder::createMultiPointLayer(
    const std::map<NodeId, RoadNode> &nodes,
    const RoadNodeType nodeType,
    const std::string &layerName
) {
    nlohmann::json feature;
    feature["type"] = "Feature";

    nlohmann::json geometry;
    geometry["type"] = "MultiPoint";
    geometry["coordinates"] = nlohmann::json::array();

    nlohmann::json nodeMetadata = nlohmann::json::array();

    for (const auto &node: nodes | std::views::values) {
        if (node.nodeType == nodeType) {
            geometry["coordinates"].push_back({node.point.x, node.point.y});

            nlohmann::json nodeInfo;
            nodeMetadata.push_back(nodeInfo);
        }
    }

    feature["geometry"] = geometry;

    nlohmann::json properties;
    properties["name"] = layerName;
    properties["featureType"] = "nodes";
    properties["nodeType"] = nodeType == TrenchNode ? "TrenchNode" : "HDDNode";

    feature["properties"] = properties;
    return feature;
}

void GeoJsonEncoder::createMultiLineStringLayers(
    const std::map<NodeId, RoadNode> &nodes,
    const std::map<NodeId, NodeConnections> &connections,
    nlohmann::json &features
) {
    std::map<RoadEdgeType, nlohmann::json> edgeLayers;
    std::map<RoadEdgeType, nlohmann::json> edgeMetadata;

    edgeLayers[Trench] = nlohmann::json::array();
    edgeLayers[HDD] = nlohmann::json::array();
    edgeLayers[Transition] = nlohmann::json::array();

    edgeMetadata[Trench] = nlohmann::json::array();
    edgeMetadata[HDD] = nlohmann::json::array();
    edgeMetadata[Transition] = nlohmann::json::array();

    for (const auto &[sourceNodeId, connectionList]: connections) {
        const auto &sourceNode = nodes.at(sourceNodeId);

        for (const auto &[edgeType, targetNodeId]: connectionList) {
            if (sourceNodeId >= targetNodeId) continue;

            const auto &targetNode = nodes.at(targetNodeId);

            nlohmann::json line = {
                {sourceNode.point.x, sourceNode.point.y},
                {targetNode.point.x, targetNode.point.y}
            };
            edgeLayers[edgeType].push_back(line);

            nlohmann::json edgeInfo;
            edgeInfo["sourceType"] = sourceNode.nodeType == TrenchNode ? "TrenchNode" : "HDDNode";
            edgeInfo["targetType"] = targetNode.nodeType == TrenchNode ? "TrenchNode" : "HDDNode";
            edgeMetadata[edgeType].push_back(edgeInfo);
        }
    }

    std::map<RoadEdgeType, std::string> layerNames = {
        {Trench, "trench-edges"},
        {HDD, "hdd-edges"},
        {Transition, "transition-edges"}
    };

    std::map<RoadEdgeType, std::string> displayNames = {
        {Trench, "Trench Edges"},
        {HDD, "HDD Edges"},
        {Transition, "Transition Edges"}
    };

    for (const auto &[edgeType, coordinates]: edgeLayers) {
        if (!coordinates.empty()) {
            features.push_back(createMultiLineStringLayer(
                coordinates,
                edgeMetadata[edgeType],
                layerNames[edgeType],
                displayNames[edgeType],
                edgeType
            ));
        }
    }
}

nlohmann::json GeoJsonEncoder::createMultiLineStringLayer(
    const nlohmann::json &coordinates,
    const nlohmann::json &edgeMetadata,
    const std::string &layerId,
    const std::string &layerName,
    const RoadEdgeType edgeType
) {
    nlohmann::json feature;
    feature["type"] = "Feature";

    nlohmann::json geometry;
    geometry["type"] = "MultiLineString";
    geometry["coordinates"] = coordinates;
    feature["geometry"] = geometry;

    nlohmann::json properties;
    properties["layer"] = layerId;
    properties["name"] = layerName;
    properties["featureType"] = "edges";

    std::string edgeTypeStr;
    switch (edgeType) {
        case Trench: edgeTypeStr = "Trench";
            break;
        case HDD: edgeTypeStr = "HDD";
            break;
        case Transition: edgeTypeStr = "Transition";
            break;
        default: edgeTypeStr = "Unknown";
    }
    properties["edgeType"] = edgeTypeStr;
    properties["edgeCount"] = edgeMetadata.size();
    properties["edgeMetadata"] = edgeMetadata;

    feature["properties"] = properties;
    return feature;
}

nlohmann::json GeoJsonEncoder::polygonToGeoJSON(const BG_Polygon &polygon) {
    nlohmann::json coordinates = nlohmann::json::array();

    coordinates.push_back(ringToGeoJSON(polygon.outer()));

    for (const auto &interior: polygon.inners()) {
        coordinates.push_back(ringToGeoJSON(interior));
    }

    return {
        {"type", "Polygon"},
        {"coordinates", coordinates}
    };
}

nlohmann::json GeoJsonEncoder::multiPolygonToGeoJSON(const BG_MultiPolygon &multiPolygon) {
    nlohmann::json coordinates = nlohmann::json::array();

    for (const auto &polygon: multiPolygon) {
        nlohmann::json polygonCoords = nlohmann::json::array();

        polygonCoords.push_back(ringToGeoJSON(polygon.outer()));

        for (const auto &interior: polygon.inners()) {
            polygonCoords.push_back(ringToGeoJSON(interior));
        }

        coordinates.push_back(polygonCoords);
    }

    return {
        {"type", "MultiPolygon"},
        {"coordinates", coordinates}
    };
}

nlohmann::json GeoJsonEncoder::polygonToFeature(const BG_Polygon &polygon) {
    return {
        {"type", "Feature"},
        {"geometry", polygonToGeoJSON(polygon)}
    };
}

nlohmann::json GeoJsonEncoder::multiPolygonToFeature(const BG_MultiPolygon &multiPolygon) {
    return {
        {"type", "Feature"},
        {"geometry", multiPolygonToGeoJSON(multiPolygon)}
    };
}

nlohmann::json GeoJsonEncoder::polygonsToFeatureCollection(const std::vector<BG_Polygon> &polygons) {
    nlohmann::json features = nlohmann::json::array();

    for (const auto &polygon: polygons) {
        features.push_back(polygonToFeature(polygon));
    }

    return {
        {"type", "FeatureCollection"},
        {"features", features}
    };
}

nlohmann::json GeoJsonEncoder::lineStringToFeature(const BG_LineString &line) {
    return {
        {"type", "Feature"},
        {
            "geometry", {
                {"type", "LineString"},
                {"coordinates", getCoordinates(line)}
            }
        }
    };
}

nlohmann::json GeoJsonEncoder::lineStringsToFeatureCollection(const std::vector<BG_LineString> &lines) {
    nlohmann::json features = nlohmann::json::array();

    for (const auto &line: lines) {
        features.push_back(lineStringToFeature(line));
    }

    return {
        {"type", "FeatureCollection"},
        {"crs", getCrs()},
        {"features", features}
    };
}

nlohmann::json GeoJsonEncoder::pointToGeoJSON(const BG_Point &point) {
    return nlohmann::json::array({point.x(), point.y()});
}

nlohmann::json GeoJsonEncoder::ringToGeoJSON(const BG_Ring &ring) {
    nlohmann::json coordinates = nlohmann::json::array();

    for (const auto &point: ring) {
        coordinates.push_back(pointToGeoJSON(point));
    }

    return coordinates;
}

nlohmann::json GeoJsonEncoder::lineStringToGeometry(const BG_LineString &line) {
    return {
        {"type", "LineString"},
        {"coordinates", getCoordinates(line)}
    };
}

nlohmann::json GeoJsonEncoder::getCoordinates(const BG_LineString &line) {
    nlohmann::json coordinates = nlohmann::json::array();
    for (const auto &point: line) {
        coordinates.push_back({
            boost::geometry::get<0>(point),
            boost::geometry::get<1>(point)
        });
    }
    return coordinates;
}

nlohmann::json GeoJsonEncoder::getCrs() {
    return {
        {"type", "name"},
        {
            "properties", {
                {"name", "urn:ogc:def:crs:EPSG::3857"}
            }
        }
    };
}
