#ifndef TRAJSAMPLER_H
#define TRAJSAMPLER_H

#include <vector>

#include "bezierpath.h"

struct TrajPoint
{
    point p;
    float curvature;
    point vel;
};

typedef std::vector<TrajPoint> Trajectory;

class TrajSampler
{
public:
    void setPath(const BezierPath& path);

    virtual ~TrajSampler() {}

    virtual Trajectory sample(int density, int iterations = 1) = 0;

protected:
    BezierPath bpath;

    void startSampling();
    void stopSampling();
    clock_t starttime;
};

class BaseSampler : public TrajSampler
{
public:
    Trajectory sample(int density, int iterations);
};

class HomogeneousSampler : public TrajSampler
{
public:
    Trajectory sample(int density, int iterations);
};

class CurvatureSampler : public TrajSampler
{
public:
    Trajectory sample(int density, int iterations);
};


#endif // TRAJSAMPLER_H
