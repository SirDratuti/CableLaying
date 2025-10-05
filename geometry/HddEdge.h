#ifndef ROADS_HDDEDGE_H
#define ROADS_HDDEDGE_H

#include "../primitives/Edge.h"

class HddEdge {
public:
    EdgeId fromId;
    EdgeId toId;
    Edge edge;

    HddEdge() : fromId(-1), toId(-1) {
    }

    HddEdge(
        const EdgeId fromId,
        const EdgeId toId,
        const Edge &edge
    ) : fromId(fromId), toId(toId), edge(edge) {
    }
};

#endif
