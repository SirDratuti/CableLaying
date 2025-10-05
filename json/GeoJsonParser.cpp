#include "nlohmann/json.hpp"
#include <vector>
#include <stdexcept>
#include "../geometry/Types.h"

class GeoJsonParser {
public:
    static BG_MultiPolygon parsePolygons(const nlohmann::json &jsonData) {
        try {
            if (jsonData["type"] != "FeatureCollection") {
                throw std::runtime_error("Expected FeatureCollection type");
            }

            BG_MultiPolygon polygons;
            for (const auto &feature: jsonData["features"]) {
                if (feature["geometry"]["type"] == "Polygon") {
                    polygons.push_back(parsePolygon(feature["geometry"]));
                }
            }

            return polygons;
        } catch (const std::exception &e) {
            throw std::runtime_error("Error parsing polygons: " + std::string(e.what()));
        }
    }

private:
    static BG_Polygon parsePolygon(const nlohmann::json &geometry) {
        BG_Polygon polygon;

        for (const auto &ring: geometry["coordinates"]) {
            std::vector<BG_Point> points;

            for (const auto &coord: ring) {
                double longitude = coord[0];
                double latitude = coord[1];
                points.emplace_back(longitude, latitude);
            }

            boost::geometry::append(polygon, points);
        }

        return polygon;
    }
};
