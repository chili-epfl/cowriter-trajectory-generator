#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <vector>

struct point {
    float x, y;
};

struct BezierCubicPatch {
    float ox, oy;  // origin of the Bezier curve
    float c1x, c1y; // 1st ctl point
    float c2x, c2y; // 2nd ctl point
    float x, y; // end of the bezier curve

    point pointAt(float t) const;

    /**
      Algo to compute length based on Gravesen's + Hollasch impl
      http://steve.hollasch.net/cgindex/curves/cbezarclen.html

      error: acceptable length error, in path's unit
    */
    float length(float error = 1.0) const;


private:
    void split(BezierCubicPatch* left, BezierCubicPatch* right) const;
    void addifclose(float* length, float error) const;
};

struct BezierPath {
    point origin;
    std::vector<BezierCubicPatch> curves;

    float length(float error = 1.0) const;
};

#endif // BEZIERPATH_H
