#ifndef ROADS_LINESTRING_H
#define ROADS_LINESTRING_H
#include <vector>

#include "Point.h"

class LineString {
    std::vector<Point> points;

public:
    explicit LineString(const std::vector<Point> &points) : points(points) {
    }

    std::vector<Point> &getPoints() { return points; }
};

#endif
