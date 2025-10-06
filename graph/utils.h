#pragma once

#include "../primitives/Point.h"
#include "../primitives/Edge.h"
#include <cmath>

namespace utils {
    inline double cosToAngle(const double cos) {
        return acos(std::max(-1.0, std::min(1.0, cos)));
    }

    inline double angleToRadians(const double angleDeviation) {
        return angleDeviation * M_PI / 180.0;
    }

    inline double edgeLength(const Edge &e) {
        const double dx = e.end.x - e.start.x;
        const double dy = e.end.y - e.start.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    inline double length(const Point &p) {
        return std::sqrt(p.x * p.x + p.y * p.y);
    }

    inline double distance(const Point &p1, const Point &p2) {
        const double dx = p2.x - p1.x;
        const double dy = p2.y - p1.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    inline double angleBetweenEdges(const Edge &e1, const Edge &e2) {
        const double dx1 = e1.end.x - e1.start.x;
        const double dy1 = e1.end.y - e1.start.y;
        const double dx2 = e2.end.x - e2.start.x;
        const double dy2 = e2.end.y - e2.start.y;

        const double dot = dx1 * dx2 + dy1 * dy2;
        const double len1 = sqrt(dx1 * dx1 + dy1 * dy1);
        const double len2 = sqrt(dx2 * dx2 + dy2 * dy2);

        if (len1 < constants::epsilon || len2 < constants::epsilon) return 0.0;

        double cosAngle = dot / (len1 * len2);
        cosAngle = std::max(-1.0, std::min(1.0, cosAngle));

        return acos(cosAngle) * 180.0 / M_PI;
    }

    inline double dotProduct(const Point &a, const Point &b) {
        return a.x * b.x + a.y * b.y;
    }
}
