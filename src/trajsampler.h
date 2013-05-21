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

    virtual Trajectory sample(int density) = 0;

protected:
    BezierPath bpath;
};

class BaseSampler : public TrajSampler
{
public:
    Trajectory sample(int density);
};

class HomogeneousSampler : public TrajSampler
{
public:
    Trajectory sample(int density);
};

class CurvatureSampler : public TrajSampler
{
public:
    Trajectory sample(int density);
};


#endif // TRAJSAMPLER_H
