#pragma once

#include "GraphType.h"
#include "../config/RoadConfig.h"
#include "../primitives/Point.h"
#include "../primitives/Edge.h"

class MinimalEdgeFinder {
    Edge edge1, edge2;
    GraphType graphType;
    double radiansTolerance;
    double hddMinLength, hddMaxLength;

public:
    MinimalEdgeFinder(
        const Edge &edge1,
        const Edge &edge2,
        const RoadConfig &roadConfig
    );

    bool findMinimalEdge(
        Point &bestPointStart,
        Point &bestPointEnd,
        double &bestLength,
        double &bestAngleDeviation
    ) const;

private:
    bool checkAngleConstraints(double t1, double t2, double &angleDeviation) const;

    bool canEdgesBeSubdivided(double t1, double t2) const;

    bool canSegmentSatisfyConstraints(double segmentLength) const;

    double objectiveFunction(double t1, double t2) const;
};
