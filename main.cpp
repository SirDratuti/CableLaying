#include "geometry/PolygonMerger.h"
#include "graph/GraphResolver.h"
#include "json/GeoJsonEncoder.h"
#include "json/GeoJsonParser.h"
#include "json/JsonReader.h"
#include "json/JsonWriter.h"
#include "config/RoadConfigParser.h"
#include "primitives/Edge.h"
#include "primitives/primitives_converter.h"
#include "roads/RoadsMerger.h"

int main() {
    const auto roadConfigJson = JsonReader::loadFromFile("../data/roadconfig.json");
    const auto geoJson = JsonReader::loadFromFile("../data/roads.geojson");

    const auto roadConfig = RoadConfigParser::parseRoadConfig(roadConfigJson);
    const std::vector polygons = GeoJsonParser::parsePolygons(geoJson);

    const auto mergedPolygons = PolygonMerger::merge(polygons);

    std::vector<BG_Ring> rings{};
    for (const auto &polygon: mergedPolygons) {
        BG_Ring ring;
        boost::geometry::assign_points(ring, polygon.outer());
        rings.push_back(ring);

        for (const auto &inner: polygon.inners()) {
            BG_Ring innerRing;
            boost::geometry::assign_points(innerRing, inner);
            rings.push_back(innerRing);
        }
    }

    int edgeId = 0;
    std::vector<std::vector<Edge> > edgesVector = GraphResolver::ringsToEdges(rings, edgeId);

    const auto componentEdges = GraphResolver::resolveGraph(edgesVector, roadConfig);

    std::vector<HddEdge> edges;
    for (const auto &componentEdge: componentEdges) {
        edges.emplace_back(
            componentEdge.edge.fromId, componentEdge.edge.toId,
            Edge{++edgeId, componentEdge.edge.edge.start, componentEdge.edge.edge.end});
    }

    std::vector<BG_LineString> lineStrings{};

    for (const auto &edge: edges) {
        lineStrings.push_back(primitives_converter::convert(edge.edge));
    }

    auto merger = RoadsMerger(edges, edgesVector, roadConfig);
    long double totalCost = merger.getTotalCost();
    std::cout << "Total cost: " << std::fixed << totalCost << std::endl;

    nlohmann::json graphJson = GeoJsonEncoder::graphToLayeredFeaturesCollection(
        merger.getNodes(), merger.getNodesConnections());

    std::string fileName;
    if (roadConfig.getGraphType() == Default) {
        fileName = "../data/graph.geojson";
    } else {
        fileName = "../data/graphHddOnly.geojson";
    }

    JsonWriter::saveToFile(graphJson, fileName);

    return 0;
}
