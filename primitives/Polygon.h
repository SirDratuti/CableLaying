#pragma once

#include <vector>
#include "LineString.h"

class Polygon {
    std::vector<LineString> rings;

public:
    void addRing(const LineString &ring);

    const LineString &getExteriorRing() const;

    std::vector<LineString> &getRings();

    std::vector<LineString> getInteriorRings() const;

    bool hasHoles() const;

    Polygon();
};
