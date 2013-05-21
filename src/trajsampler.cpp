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

Trajectory BaseSampler::sample(int density, int iterations)
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

Trajectory HomogeneousSampler::sample(int density, int iterations)
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

Trajectory CurvatureSampler::sample(int density, int iterations)
{
    Trajectory traj;

    startSampling();

    vector<float> distribution, next_distribution;

    // create initial homogeneous distribution
    float len = bpath.length();
    float inc = len / (density * 10);
    for (float dist = 0.0 ; dist <= len ; dist += inc) {
        distribution.push_back(dist);
    }

    for (uint j = 0; j < iterations; j++) {
        next_distribution.push_back(distribution[0]);

        for (uint i = 1; i < distribution.size() - 1; i++) {
            //TODO: optimization: we compute 2 times curvature for the same distance!
            float displ = bpath.curvatureAtDistance(distribution.at(i + 1)) - bpath.curvatureAtDistance(distribution.at(i - 1));
            next_distribution.push_back(distribution.at(i) + displ * 100);
        }
        next_distribution.push_back(distribution.back());
        distribution = next_distribution;
        next_distribution.clear();
    }

    stopSampling();

    for (float dist : distribution) {
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
