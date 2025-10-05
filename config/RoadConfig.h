#ifndef ROADS_ROADCONFIG_H
#define ROADS_ROADCONFIG_H

class RoadConfig {
    double trenchCost;
    double hddCost;
    double transitionCost;

    double hddMinLength;
    double hddMaxLength;

    double alpha;

public:
    RoadConfig(
        const double trenchCost,
        const double hddCost,
        const double transitionCost,
        const double hddMinLength,
        const double hddMaxLength,
        const double alpha
    ) : trenchCost(trenchCost),
        hddCost(hddCost),
        transitionCost(transitionCost),
        hddMinLength(hddMinLength),
        hddMaxLength(hddMaxLength),
        alpha(alpha) {
    }

    double getTrenchCost() const { return trenchCost; }
    double getHddCost() const { return hddCost; }
    double getTransitionCost() const { return transitionCost; }
    double getHddMinLength() const { return hddMinLength; }
    double getHddMaxLength() const { return hddMaxLength; }
    double getAlpha() const { return alpha; }
};

#endif
