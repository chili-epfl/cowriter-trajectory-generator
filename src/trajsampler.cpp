#include<iostream>

#include "trajsampler.h"

using namespace std;


void TrajSampler::setPath(const BezierPath &path)
{
    bpath = path;
}

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

vector<point> HomogeneousSampler::sample(int density)
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
            curvatures.push_back(bpath.curvatureAtDistance(dist));
    }

    return points;
}

#define ITERATIONS 5
std::vector<point> CurvatureSampler::sample(int density)
{
    HomogeneousSampler sampler;
    sampler.setPath(bpath);
    auto distribution = sampler.sample(density);
    auto initialCurvatures = sampler.curvatures;

    for (int j = 0; j < ITERATIONS; j++) {

        for (int i = 0; i < initialDistribution.size(); i++) {
            //TODO!
        }
    }
    return distribution;
}
