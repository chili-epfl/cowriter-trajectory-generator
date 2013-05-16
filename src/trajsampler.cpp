#include<iostream>

#include "trajsampler.h"

using namespace std;

TrajSampler::TrajSampler(const BezierPath &path) : bpath(path) {}

BaseSampler::BaseSampler(const BezierPath &path) : TrajSampler(path) {}

vector<point> BaseSampler::sample(int density)
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

HomogenousSampler::HomogenousSampler(const BezierPath &path) : TrajSampler(path) {}

vector<point> HomogenousSampler::sample(int density)
{
    float len = bpath.length();

    float inc = len / (density * 10);

    curvatures.clear();

    vector<point> points;

    for (float dist = 0.0 ; dist <= len ; dist += inc) {
            point p = bpath.pointAtDistance(dist);
            p.x += bpath.origin.x;
            p.y += bpath.origin.y;
            points.push_back(p);
            curvatures.push_back(0.0);
    }

    return points;
}
