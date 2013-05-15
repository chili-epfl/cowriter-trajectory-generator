#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <vector>

struct point {
    float x, y;
};

struct bezier {
    float c1x, c1y;
    float c2x, c2y;
    float x, y;
};

struct bezierpath {
    point origin;
    std::vector<bezier> curves;
};

#endif // BEZIERPATH_H
