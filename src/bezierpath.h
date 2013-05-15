#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <vector>

struct point {
    float x, y;
};

struct bezier {
    float ox, oy;
    float c1x, c1y;
    float c2x, c2y;
    float x, y;

    point pointAt(float t);
};

struct bezierpath {
    point origin;
    std::vector<bezier> curves;

};

#endif // BEZIERPATH_H
