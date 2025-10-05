#ifndef ROADS_EdgeFINDER_H
#define ROADS_EdgeFINDER_H

#include "../primitives/Point.h"
#include "../primitives/Edge.h"

class MinimalEdgeFinder {
    Edge edge1, edge2;
    double radiansTolerance;
    double distanceMin, distanceMax;

public:
    MinimalEdgeFinder(
        const Edge &edge1,
        const Edge &edge2,
        double angleTolerance,
        double distanceMin,
        double distanceMax
    );

    bool checkAngleConstraints(double t1, double t2, double &angleDeviation) const;

    double objectiveFunction(double t1, double t2) const;

    bool findMinimalEdge(
        Point &bestPointStart,
        Point &bestPointEnd,
        double &bestLength,
        double &bestAngleDeviation
    ) const;
};

#endif
