#include "MinimalEdgeFinder.h"

#include <cmath>

#include "utils.h"
#include "../primitives/Point.h"
#include "../primitives/Edge.h"
#include <boost/math/tools/minima.hpp>

#include "../config/RoadConfig.h"

Point pointAt(const Edge &edge, const double t) {
    return {
        edge.start.x + t * (edge.end.x - edge.start.x),
        edge.start.y + t * (edge.end.y - edge.start.y)
    };
}

MinimalEdgeFinder::MinimalEdgeFinder(
    const Edge &edge1,
    const Edge &edge2,
    const RoadConfig &roadConfig
)
    : edge1(edge1),
      edge2(edge2),
      graphType(roadConfig.getGraphType()),
      hddMinLength(roadConfig.getHddMinLength()),
      hddMaxLength(roadConfig.getHddMaxLength()) {
    radiansTolerance = utils::angleToRadians(roadConfig.getAlpha());
}

bool MinimalEdgeFinder::checkAngleConstraints(const double t1, const double t2, double &angleDeviation) const {
    const Point p1 = pointAt(edge1, t1);
    const Point p2 = pointAt(edge2, t2);

    const double edgeLength = utils::distance(p1, p2);

    if (edgeLength < constants::epsilon) return false;

    const double dx1 = edge1.end.x - edge1.start.x;
    const double dy1 = edge1.end.y - edge1.start.y;
    const double dx2 = edge2.end.x - edge2.start.x;
    const double dy2 = edge2.end.y - edge2.start.y;

    const double dx = p2.x - p1.x;
    const double dy = p2.y - p1.y;

    const double dot1 = dx * dx1 + dy * dy1;
    const double dot2 = dx * dx2 + dy * dy2;

    const double len1 = sqrt(dx1 * dx1 + dy1 * dy1);
    const double len2 = sqrt(dx2 * dx2 + dy2 * dy2);

    if (len1 < constants::epsilon || len2 < constants::epsilon) return false;

    const double angle1 = utils::cosToAngle(dot1 / (edgeLength * len1));
    const double angle2 = utils::cosToAngle(dot2 / (edgeLength * len2));

    const double dev1 = std::abs(angle1 - M_PI / 2);
    const double dev2 = std::abs(angle2 - M_PI / 2);

    angleDeviation = std::max(dev1, dev2);

    return angleDeviation <= radiansTolerance;
}

double MinimalEdgeFinder::objectiveFunction(const double t1, const double t2) const {
    const Point p1 = pointAt(edge1, t1);
    const Point p2 = pointAt(edge2, t2);

    const double length = utils::distance(p1, p2);

    if (length < hddMinLength || length > hddMaxLength) {
        return 1e10;
    }

    if (graphType == HddOnly && !canEdgesBeSubdivided(t1, t2)) {
        return 1e10;
    }

    if (double angleDev; !checkAngleConstraints(t1, t2, angleDev)) {
        return 1e10;
    }

    return length;
}

bool MinimalEdgeFinder::findMinimalEdge(
    Point &bestPointStart,
    Point &bestPointEnd,
    double &bestLength,
    double &bestAngleDeviation
) const {
    constexpr int bits = std::numeric_limits<double>::digits;

    double best_t1 = 0.5, best_t2 = 0.5;
    double currentBestLength = 1e10;

    constexpr int gridSize = 10;

    for (int i = 0; i <= gridSize; ++i) {
        for (int j = 0; j <= gridSize; ++j) {
            const double t1 = static_cast<double>(i) / gridSize;
            const double t2 = static_cast<double>(j) / gridSize;

            if (double angleDeviation; checkAngleConstraints(t1, t2, angleDeviation)) {
                Point p1 = pointAt(edge1, t1);
                Point p2 = pointAt(edge2, t2);

                if (const double length = utils::distance(p1, p2);
                    length >= hddMinLength && length <= hddMaxLength
                ) {
                    if (graphType == Default || canEdgesBeSubdivided(t1, t2)) {
                        if (length < currentBestLength) {
                            currentBestLength = length;
                            best_t1 = t1;
                            best_t2 = t2;
                        }
                    }
                }
            }
        }
    }

    uintmax_t maxIterations = 100;
    const auto f1 = [&](const double t1) { return objectiveFunction(t1, best_t2); };
    const auto f2 = [&](const double t2) { return objectiveFunction(best_t1, t2); };

    for (int refineIterations = 0; refineIterations < 20; ++refineIterations) {
        try {
            if (auto [t1, best] = boost::math::tools::brent_find_minima(f1, 0.0, 1.0, bits, maxIterations);
                best < currentBestLength && best < constants::epsilon) {
                best_t1 = t1;
                currentBestLength = best;
            }
        } catch (...) {
        }
        try {
            if (auto [t2, best] = boost::math::tools::brent_find_minima(f2, 0.0, 1.0, bits, maxIterations);
                best < currentBestLength && best < constants::epsilon) {
                best_t2 = t2;
                currentBestLength = best;
            }
        } catch (...) {
        }
    }

    bestPointStart = pointAt(edge1, best_t1);
    bestPointEnd = pointAt(edge2, best_t2);
    bestLength = utils::distance(bestPointStart, bestPointEnd);
    checkAngleConstraints(best_t1, best_t2, bestAngleDeviation);

    return bestLength >= hddMinLength &&
           bestLength <= hddMaxLength &&
           bestAngleDeviation <= radiansTolerance &&
           (graphType == Default || canEdgesBeSubdivided(best_t1, best_t2));
}

bool MinimalEdgeFinder::canEdgesBeSubdivided(const double t1, const double t2) const {
    const double totalLength1 = utils::edgeLength(edge1);
    const double totalLength2 = utils::edgeLength(edge2);

    const double edge1Segment1 = totalLength1 * t1;
    const double edge1Segment2 = totalLength1 * (1.0 - t1);

    const double edge2Segment1 = totalLength2 * t2;
    const double edge2Segment2 = totalLength2 * (1.0 - t2);

    return canSegmentSatisfyConstraints(edge1Segment1) &&
           canSegmentSatisfyConstraints(edge1Segment2) &&
           canSegmentSatisfyConstraints(edge2Segment1) &&
           canSegmentSatisfyConstraints(edge2Segment2);
}

bool MinimalEdgeFinder::canSegmentSatisfyConstraints(const double segmentLength) const {
    if (segmentLength < hddMinLength) {
        return false;
    }
    if (segmentLength <= hddMaxLength) {
        return true;
    }

    const int minSegments = std::ceil(segmentLength / hddMaxLength);
    const double maxPossibleLengthPerSegment = segmentLength / minSegments;

    return maxPossibleLengthPerSegment >= hddMinLength;
}
