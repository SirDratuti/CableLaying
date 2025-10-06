#pragma once

#include <vector>
#include <nlohmann/json.hpp>

#include "../geometry/Types.h"
#include "../roads/RoadsMerger.h"
#include <boost/geometry.hpp>

class GeoJsonEncoder {
public:
    static nlohmann::json graphToLayeredFeaturesCollection(
        const std::map<NodeId, RoadNode> &nodes,
        const std::map<NodeId, NodeConnections> &connections);

    static nlohmann::json polygonsToFeatureCollection(const std::vector<BG_Polygon> &polygons);

    static nlohmann::json lineStringsToFeatureCollection(const std::vector<BG_LineString> &lines);

    static nlohmann::json lineStringToFeature(const BG_LineString &line);

    static nlohmann::json polygonToFeature(const BG_Polygon &polygon);

    static nlohmann::json multiPolygonToFeature(const BG_MultiPolygon &multiPolygon);

private:
    static nlohmann::json createMultiPointLayer(
        const std::map<NodeId, RoadNode> &nodes,
        RoadNodeType nodeType,
        const std::string &layerName);

    static void createMultiLineStringLayers(
        const std::map<NodeId, RoadNode> &nodes,
        const std::map<NodeId, NodeConnections> &connections,
        nlohmann::json &features);

    static nlohmann::json createMultiLineStringLayer(
        const nlohmann::json &coordinates,
        const nlohmann::json &edgeMetadata,
        const std::string &layerId, const std::string &layerName,
        RoadEdgeType edgeType);

    static nlohmann::json pointToGeoJSON(const BG_Point &point);

    static nlohmann::json ringToGeoJSON(const BG_Ring &ring);

    static nlohmann::json polygonToGeoJSON(const BG_Polygon &polygon);

    static nlohmann::json multiPolygonToGeoJSON(const BG_MultiPolygon &multiPolygon);

    static nlohmann::json lineStringToGeometry(const BG_LineString &line);

    static nlohmann::json getCoordinates(const BG_LineString &line);

    static nlohmann::json getCrs();
};
