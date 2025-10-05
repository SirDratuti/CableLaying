cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 20)

project(Roads)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS geometry)

add_executable(
        Roads main.cpp

        primitives/LineString.h
        primitives/Polygon.h
        primitives/Point.h
        primitives/Edge.h
        primitives/primitives_converter.h
        primitives/Polygon.cpp

        json/GeoJsonEncoder.h
        json/GeoJsonParser.cpp
        json/JsonReader.cpp
        json/JsonWriter.cpp
        json/RoadConfigParser.cpp
        json/GeoJsonEncoder.cpp

        graph/Component.h
        graph/ComponentsConnector.h
        graph/GraphResolver.h
        graph/utils.h
        graph/ComponentEdge.h
        graph/UnionFinder.h
        graph/MinimalEdgeFinder.h
        graph/ComponentsConnector.cpp
        graph/MinimalEdgeFinder.cpp
        graph/GraphResolver.cpp
        graph/UnionFinder.cpp

        geometry/PolygonMerger.h
        geometry/Types.h
        geometry/HddResolver.h
        geometry/constants.h
        geometry/HddEdge.h
        geometry/HddResolver.cpp
        geometry/PolygonMerger.cpp

        config/RoadConfig.h

        roads/RoadEdge.h
        roads/RoadNode.h
        roads/RoadsMerger.h
        roads/RoadsMerger.cpp
)

target_link_libraries(Roads PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Roads PRIVATE Boost::geometry)