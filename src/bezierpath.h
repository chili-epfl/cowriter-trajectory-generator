#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <vector>
#include <utility> //pair

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

    float curvatureAt(float t) const;

    std::pair<float, float> getParamForLength(float target, float error = 0.1, float t = 0.0, float width = 1.0, float offset = 0.0) const;

private:
    void split(BezierCubicPatch* left, BezierCubicPatch* right) const;
    void addifclose(float* length, float error) const;

    float pos(float t, float a, float b, float c, float d) const;
    float derivate(float t, float a, float b, float c, float d) const;
    float sec_derivate(float t, float a, float b, float c, float d) const;
};

struct BezierPath {
    point origin;
    std::vector<BezierCubicPatch> curves;

    float length(float error = 1.0) const;
    point pointAtDistance(float distance, float error = 0.001) const;
    float curvatureAtDistance(float dist, float error = 0.001) const;
    point pointAt(float t) const;
    float curvatureAt(float t) const;
};

#endif // BEZIERPATH_H
