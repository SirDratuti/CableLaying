#include "Polygon.h"

#include <stdexcept>
#include <vector>
#include "LineString.h"

Polygon::Polygon() = default;

void Polygon::addRing(const LineString &ring) {
    rings.push_back(ring);
}

const LineString &
Polygon::getExteriorRing() const {
    if (rings.empty()) {
        throw std::runtime_error("No rings in polygon");
    }
    return rings[0];
}

std::vector<LineString> &
Polygon::getRings() { return rings; }

std::vector<LineString>
Polygon::getInteriorRings() const {
    if (rings.size() <= 1) return {};
    return {rings.begin() + 1, rings.end()};
}

bool Polygon::hasHoles() const {
    return rings.size() > 1;
}
