#ifndef ROADS_EDGE_H
#define ROADS_EDGE_H

#include "Point.h"

typedef int EdgeId;

class Edge {
public:
    EdgeId id;
    Point start, end;

    Edge() : id(-1), start(Point{0, 0}), end(Point{0, 0}) {
    }

    Edge(const Point a, const Point b) : id(-1), start(a), end(b) {
    }

    Edge(const EdgeId id, const Point a, const Point b) : id(id), start(a), end(b) {
    }

    std::string toString() const {
        return "[" + start.toString() + " " + end.toString() + "]";
    }
};

#endif
