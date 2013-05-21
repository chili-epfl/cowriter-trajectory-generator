#include <iostream>
#include <math.h>       /* pow, sqrt, fabs */

#include "bezierpath.h"

#define distance(x1, y1, x2, y2) (sqrt(pow(x2-x1,2) + pow(y2-y1,2)))

using namespace std;

BezierCubicPatch::BezierCubicPatch()
{
    last_error = 0.;
    last_length = 0.;
    param_length_cache.clear();
}

point BezierCubicPatch::pointAt(float t) const
{
    point p;

    //cf http://en.wikipedia.org/wiki/B%C3%A9zier_curve
    p.x = pos(t, ox, c1x, c2x, x);
    p.y = pos(t, oy, c1y, c2y, y);
    return p;
}

float BezierCubicPatch::pos(float t, float a, float b, float c, float d) const
{
        return pow(1 - t, 3) * a
            + 3 * pow(1 - t, 2) * t * b
            + 3 * pow(t, 2) * (1 - t) * c
            + pow(t, 3) * d;
}

float BezierCubicPatch::derivate(float t, float a, float b, float c, float d) const
{
        // output of WolphramAlpha
        return -3 * (
                    a * pow(-1 + t,2)
                    + b * (-1 + 4 * t - 3 * pow(t,2))
                        + t * (-2 * c + 3 * c * t - d * t)
                );
}
float BezierCubicPatch::sec_derivate(float t, float a, float b, float c, float d) const
{
        // output of WolphramAlpha
        return -6 * (
                    a * (-1 + t)
                    + b * (2 - 3 * t)
                    + 3 * c * t
                        - c - d * t
                );
}

float BezierCubicPatch::length(float error) const
{
    //we recompute the length only if the error threshold has changed.
    if (error == last_error) return last_length;

    last_length = addifclose(error);
    last_error = error;
    return last_length;
}

float BezierCubicPatch::curvatureAt(float t) const
{
    // https://en.wikipedia.org/wiki/Curvature#Curvature_of_plane_curves
    float xprime = derivate(t, ox, c1x, c2x, x);
    float xprimeprime = sec_derivate(t, ox, c1x, c2x, x);
    float yprime = derivate(t, oy, c1y, c2y, y);
    float yprimeprime = sec_derivate(t, oy, c1y, c2y, y);

    return fabs(xprime * yprimeprime - yprime * xprimeprime) / pow(xprime * xprime + yprime * yprime, 1.5);
}

point BezierCubicPatch::velocityAt(float t) const
{
    point p;
    p.x = derivate(t, ox, c1x, c2x, x);
    p.y = derivate(t, oy, c1y, c2y, y);
    return p;
}
void BezierCubicPatch::split(BezierCubicPatch *left, BezierCubicPatch *right) const
{
    //"bezsplit" is lifted shamelessly from Schneider's Bezier curve-fitter.
    int   i, j;                               /* Index variables  */
    point Vtemp[4][4];                      /* Triangle Matrix */

    /* Copy control points  */
    Vtemp[0][0].x = ox;
    Vtemp[0][0].y = oy;
    Vtemp[0][1].x = c1x;
    Vtemp[0][1].y = c1y;
    Vtemp[0][2].x = c2x;
    Vtemp[0][2].y = c2y;
    Vtemp[0][3].x = x;
    Vtemp[0][3].y = y;


    /* Triangle computation */
    for (i = 1; i <= 3; i++) {
        for (j =0 ; j <= 3 - i; j++) {
            Vtemp[i][j].x =
                0.5 * Vtemp[i-1][j].x + 0.5 * Vtemp[i-1][j+1].x;
            Vtemp[i][j].y =
                0.5 * Vtemp[i-1][j].y + 0.5 * Vtemp[i-1][j+1].y;
        }
    }

    left->ox = Vtemp[0][0].x;
    left->oy = Vtemp[0][0].y;
    left->c1x = Vtemp[1][0].x;
    left->c1y = Vtemp[1][0].y;
    left->c2x = Vtemp[2][0].x;
    left->c2y = Vtemp[2][0].y;
    left->x =  Vtemp[3][0].x;
    left->y =  Vtemp[3][0].y;

    right->ox = Vtemp[3][0].x;
    right->oy = Vtemp[3][0].y;
    right->c1x = Vtemp[2][1].x;
    right->c1y = Vtemp[2][1].y;
    right->c2x = Vtemp[1][2].x;
    right->c2y = Vtemp[1][2].y;
    right->x =  Vtemp[0][3].x;
    right->y =  Vtemp[0][3].y;
}

float BezierCubicPatch::addifclose(float error) const
{
    BezierCubicPatch left, right;                  /* bez poly splits */

    float len = distance(ox, oy, c1x, c1y)
        + distance(c1x, c1y, c2x, c2y)
        + distance(c2x, c2y, x, y);

    float chord = distance(ox,oy,x,y);

    if((len-chord) > error)
    {
      split(&left,&right);                 /* split in two */
      len = left.addifclose(error);        /* try left side */
      len += right.addifclose(error);       /* try right side */
      return len;
    }

    return len;
}


float BezierCubicPatch::paramAtDist(float dist) const
{
    if (param_length_cache.find(dist) == param_length_cache.end()) {

            auto res = recursiveComputeParamAtDist(dist);
            param_length_cache[dist] = res.first;
            return res.first;
    }
    return param_length_cache[dist];
}


pair<float, float> BezierCubicPatch::recursiveComputeParamAtDist(float target, float error, float t, float width, float offset) const
{
    float len = length(error);
    float delta = (offset + len) - target;

    if (fabs(delta) <= error) { // found it!
        pair<float, float> res(t, offset + len);
        return res;
    }

    else if (delta < 0) { // outside of the curve!
        pair<float, float> res(-1.0, len);
        return res;
    }
    else {
        BezierCubicPatch left, right;
        split(&left,&right);
        auto first_half = left.recursiveComputeParamAtDist(target, error, t, width/2, offset);
        if (first_half.first < 0) { // we are above the length of the first half
                return right.recursiveComputeParamAtDist(target, error, t + width/2, width/2, offset + first_half.second);
        }
        else return first_half;
    }


}



float BezierPath::length(float error) const
{
    float len = 0.0;
    for (auto c : curves) len += c->length(error);
    return len;
}

point BezierPath::pointAtDistance(float dist) const
{
    float len = 0.0;
    float old_len = 0.0;

    for ( auto c : curves) {
        old_len = len;
        len += c->length();
        if (dist <= len) {
            float t = c->paramAtDist(dist - old_len);
            if (t < 0.) t = 1.0;
            return c->pointAt(t);
        }
    }
    // distance over path length!
    cerr << "Unreachable distance (path length is " << len << ")!" <<endl;
    point p; p.x = p.y = 0.;
    return p;
}

float BezierPath::curvatureAtDistance(float dist) const
{
    float len = 0.0;
    float old_len = 0.0;

    for ( auto c : curves) {
        old_len = len;
        len += c->length();
        if (dist <= len) {
            float t = c->paramAtDist(dist - old_len);
            if (t < 0.) t = 1.0;
            float curvature = c->curvatureAt(t);
            return 1. - (1./(2*curvature + 1));
        }
    }
    // distance over path length!
    cerr << "Unreachable distance (path length is " << len << ")!" <<endl;

    return 0.;
}

point BezierPath::velocityAtDistance(float dist) const
{
    float len = 0.0;
    float old_len = 0.0;

    for ( auto c : curves) {
        old_len = len;
        len += c->length();
        if (dist <= len) {
            float t = c->paramAtDist(dist - old_len);
            if (t < 0.) t = 1.0;
            return c->velocityAt(t);
        }
    }
    // distance over path length!
    cerr << "Unreachable distance (path length is " << len << ")!" <<endl;

    point p; p.x = p.y = 0.;
    return p;
}


point BezierPath::pointAt(float t) const
{
    float dist = length() * t;
    return pointAtDistance(dist);
}

float BezierPath::curvatureAt(float t) const
{
    float dist = length() * t;
    return curvatureAtDistance(dist);
}


