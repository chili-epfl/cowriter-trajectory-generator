#ifndef TRAJSAMPLER_H
#define TRAJSAMPLER_H

#include <vector>

#include "bezierpath.h"

class TrajSampler
{
public:
    void setPath(const BezierPath& path);

    virtual std::vector<point> sample(int density) = 0;

    std::vector<float> curvatures;

protected:
    BezierPath bpath;
};

class BaseSampler : public TrajSampler
{
public:
    std::vector<point> sample(int density);
};

class HomogeneousSampler : public TrajSampler
{
public:
    std::vector<point> sample(int density);
};

class CurvatureSampler : public TrajSampler
{
public:
    std::vector<point> sample(int density);

protected:
    void moveAlongCurve(std::vector<point>& points);
};


#endif // TRAJSAMPLER_H
