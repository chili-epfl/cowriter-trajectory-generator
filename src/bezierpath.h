#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <map>
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

    BezierCubicPatch();

    point pointAt(float t) const;

    /**
      Algo to compute length based on Gravesen's + Hollasch impl
      http://steve.hollasch.net/cgindex/curves/cbezarclen.html

      Use memoization for improved performances

      error: acceptable length error, in path's unit
    */
    float length(float error = 0.001) const;

    float curvatureAt(float t) const;

    /* Returns the velocity vector at time t
    */
    point velocityAt(float t) const;

    float paramAtDist(float dist) const;

private:
    void split(BezierCubicPatch* left, BezierCubicPatch* right) const;
    float addifclose(float error) const;

    std::pair<float, float> recursiveComputeParamAtDist(float target, float error = 0.001, float t = 0.0, float width = 1.0, float offset = 0.0) const;

    float pos(float t, float a, float b, float c, float d) const;
    float derivate(float t, float a, float b, float c, float d) const;
    float sec_derivate(float t, float a, float b, float c, float d) const;

    mutable float last_error;
    mutable float last_length;
    mutable std::map<float, float> param_length_cache;
};

struct BezierPath {
    point origin;
    std::vector<BezierCubicPatch*> curves;

    float length(float error = 0.001) const;
    point pointAtDistance(float distance) const;

    /**
    normalized curvature: curvature is computed based on the second
    derivative of the parametric curve (cf
    https://en.wikipedia.org/wiki/Curvature#Curvature_of_plane_curves )
    and normalized by applying the fonction: 1-1/(2x+1) where x is the raw
    curvature.
    **/
    float curvatureAtDistance(float dist) const;
    point velocityAtDistance(float dist) const;
    point pointAt(float t) const;
    // normalized curvature
    float curvatureAt(float t) const;
};

#endif // BEZIERPATH_H
