#include<iostream>

#include "trajsampler.h"

using namespace std;

TrajSampler::TrajSampler(const BezierPath &path) : bpath(path)
{
}

vector<point> TrajSampler::sample(int density)
{
    curvatures.clear();

    vector<point> points;

    for (auto curve : bpath.curves) {
        for (float t = 0.; t < 1 ; t += 1.f/density) {
            point p = curve.pointAt(t);
            p.x += bpath.origin.x;
            p.y += bpath.origin.y;
            points.push_back(p);
            curvatures.push_back(curve.curvatureAt(t));
        }
    }

    return points;
}
