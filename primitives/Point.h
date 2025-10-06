#pragma once

#include <string>
#include "../geometry/constants.h"

class Point {
public:
    double x;
    double y;

    Point() : x(0), y(0) {
    }

    Point(const double x, const double y) : x(x), y(y) {
    }

    bool operator==(const Point &other) const {
        return std::abs(x - other.x) < constants::epsilon && std::abs(y - other.y) < constants::epsilon;
    }

    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};
