#pragma once

#include "Point.h"
#include "Edge.h"

#include "../geometry/Types.h"

namespace primitives_converter {
    static Point convert(const BG_Point &point) {
        return {point.x(), point.y()};
    }

    static BG_Point convert(const Point &point) {
        return {point.x, point.y};
    }

    static BG_LineString convert(const Edge &edge) {
        return {convert(edge.start), convert(edge.end)};
    }
};
