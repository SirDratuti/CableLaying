#pragma once

#include "boost/geometry.hpp"
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>

typedef boost::geometry::model::d2::point_xy<double> BG_Point;
typedef boost::geometry::model::polygon<BG_Point> BG_Polygon;
typedef boost::geometry::model::multi_polygon<BG_Polygon> BG_MultiPolygon;
typedef boost::geometry::model::ring<BG_Point> BG_Ring;
typedef boost::geometry::model::linestring<BG_Point> BG_LineString;
