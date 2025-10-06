cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 20)

project(Roads)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS geometry)

add_executable(
        Roads main.cpp

        primitives/LineString.h
        primitives/Polygon.h
        primitives/Polygon.cpp
        primitives/Point.h
        primitives/Edge.h
        primitives/primitives_converter.h

        json/GeoJsonEncoder.h
        json/GeoJsonEncoder.cpp
        json/GeoJsonParser.h
        json/GeoJsonParser.cpp
        json/JsonReader.h
        json/JsonReader.cpp
        json/JsonWriter.h
        json/JsonWriter.cpp

        config/RoadConfigParser.h
        config/RoadConfigParser.cpp
        config/RoadConfig.h

        graph/utils.h
        graph/ComponentEdge.h
        graph/Component.h
        graph/GraphType.h
        graph/ComponentsConnector.h
        graph/ComponentsConnector.cpp
        graph/GraphResolver.h
        graph/GraphResolver.cpp
        graph/UnionFinder.h
        graph/UnionFinder.cpp
        graph/MinimalEdgeFinder.h
        graph/MinimalEdgeFinder.cpp

        geometry/constants.h
        geometry/HddEdge.h
        geometry/Types.h
        geometry/PolygonMerger.h
        geometry/PolygonMerger.cpp
        geometry/HddResolver.h
        geometry/HddResolver.cpp

        roads/RoadEdge.h
        roads/RoadNode.h
        roads/RoadsMerger.h
        roads/RoadsMerger.cpp
)

target_link_libraries(Roads PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Roads PRIVATE Boost::geometry)