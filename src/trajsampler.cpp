#include <iostream>
#include <time.h>

#include "trajsampler.h"

using namespace std;


void TrajSampler::setPath(const BezierPath &path)
{
    bpath = path;
}

void TrajSampler::startSampling()
{
    starttime = clock();
}
void TrajSampler::stopSampling()
{
    cerr << "Sampling took " << ((float)(clock() - starttime)) / (CLOCKS_PER_SEC * 0.001) << "ms" << endl;
}

Trajectory BaseSampler::sample(int density)
{
    Trajectory traj;

    startSampling();
    for (auto curve : bpath.curves) {
        for (float t = 0.; t < 1 ; t += 1.f/density) {
            TrajPoint tp;
            point p = curve->pointAt(t);
            p.x += bpath.origin.x;
            p.y += bpath.origin.y;
            tp.p = p;
            tp.curvature =  curve->curvatureAt(t);
            tp.vel = curve->velocityAt(t);
            traj.push_back(tp);
        }
    }

    stopSampling();
    return traj;
}

Trajectory HomogeneousSampler::sample(int density)
{
    Trajectory traj;

    float len = bpath.length();

    float inc = len / (density * 10);


    startSampling();

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

    stopSampling();
    return traj;
}

#define ITERATIONS 5
Trajectory CurvatureSampler::sample(int density)
{
    Trajectory traj;

    startSampling();

    HomogeneousSampler sampler;
    sampler.setPath(bpath);
    auto distribution = sampler.sample(density);

    for (int j = 0; j < ITERATIONS; j++) {

        for (int i = 0; i < distribution.size(); i++) {
            //TODO!
        }
    }

    stopSampling();
    return traj;
}
