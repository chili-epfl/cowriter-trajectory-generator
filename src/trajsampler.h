#ifndef TRAJSAMPLER_H
#define TRAJSAMPLER_H

#include <vector>

#include "bezierpath.h"

class TrajSampler
{
public:
    TrajSampler(const BezierPath& path);

    virtual std::vector<point> sample(int density) = 0;

    std::vector<float> curvatures;

protected:
    const BezierPath& bpath;
};

class BaseSampler : public TrajSampler
{
public:
    BaseSampler(const BezierPath& path);
    std::vector<point> sample(int density);
};

class HomogenousSampler : public TrajSampler
{
public:
    HomogenousSampler(const BezierPath& path);
    std::vector<point> sample(int density);
};

#endif // TRAJSAMPLER_H
