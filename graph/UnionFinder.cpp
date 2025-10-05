#include "UnionFinder.h"

#include "Component.h"

ComponentId UnionFinder::find(const ComponentId x) {
    if (!parent.contains(x)) {
        parent[x] = x;
        rank[x] = 0;
        return x;
    }

    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void UnionFinder::unite(const ComponentId x, const ComponentId y) {
    const ComponentId rootX = find(x);
    const ComponentId rootY = find(y);

    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}
