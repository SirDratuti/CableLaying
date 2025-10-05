#include "geometry/PolygonMerger.h"
#include "graph/GraphResolver.h"
#include "json/GeoJsonEncoder.h"
#include "json/GeoJsonParser.cpp"
#include "json/JsonReader.cpp"
#include "json/JsonWriter.cpp"
#include "json/RoadConfigParser.cpp"
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

    const auto componentEdges = GraphResolver::resolveGraph(
        edgesVector,
        roadConfig.getHddMinLength(),
        roadConfig.getHddMaxLength(),
        roadConfig.getAlpha()
    );

    std::vector<HddEdge> edges;

    for (auto component_edge: componentEdges) {
        edges.emplace_back(component_edge.edge);
    }

    std::vector<BG_LineString> lineStrings{};

    for (auto edge: edges) {
        lineStrings.push_back(primitives_converter::convert(edge.edge));
    }

    auto merger = RoadsMerger(edges, edgesVector);
    double totalCost = merger.getTotalCost(roadConfig);
    std::cout << "Total cost: " << std::fixed << totalCost << std::endl;

    nlohmann::json graphJson = GeoJsonEncoder::graphToLayeredFeaturesCollection(merger.getNodes(), merger.getNodesConnections());
    JsonWriter::saveToFile(graphJson, "../data/graph.geojson");

    return 0;
}
