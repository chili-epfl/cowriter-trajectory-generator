#include<iostream>

#include "trajsampler.h"

using namespace std;


void TrajSampler::setPath(const BezierPath &path)
{
    bpath = path;
}

Trajectory BaseSampler::sample(int density)
{
    Trajectory traj;

    for (auto curve : bpath.curves) {
        for (float t = 0.; t < 1 ; t += 1.f/density) {
            TrajPoint tp;
            point p = curve.pointAt(t);
            p.x += bpath.origin.x;
            p.y += bpath.origin.y;
            tp.p = p;
            tp.curvature =  curve.curvatureAt(t);
            tp.vel = curve.velocityAt(t);
            traj.push_back(tp);
        }
    }

    return traj;
}

Trajectory HomogeneousSampler::sample(int density)
{
    Trajectory traj;

    float len = bpath.length();

    float inc = len / (density * 10);


    for (float dist = 0.0 ; dist <= len ; dist += inc) {
            TrajPoint tp;
            point p = bpath.pointAtDistance(dist);
            p.x += bpath.origin.x;
            p.y += bpath.origin.y;
            tp.p = p;
            tp.curvature = bpath.curvatureAtDistance(dist);
            tp.vel = bpath.velocityAtDistance(dist);
            traj.push_back(tp);
    }

    return traj;
}

#define ITERATIONS 5
Trajectory CurvatureSampler::sample(int density)
{
    Trajectory traj;

    HomogeneousSampler sampler;
    sampler.setPath(bpath);
    auto distribution = sampler.sample(density);

    for (int j = 0; j < ITERATIONS; j++) {

        for (int i = 0; i < distribution.size(); i++) {
            //TODO!
        }
    }
    return traj;
}
