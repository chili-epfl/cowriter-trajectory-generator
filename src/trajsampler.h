#ifndef TRAJSAMPLER_H
#define TRAJSAMPLER_H

#include <vector>

#include "bezierpath.h"

class TrajSampler
{
public:
    TrajSampler(const bezierpath& path);

    std::vector<point> sample(int density);

private:
    const bezierpath& bpath;
};

#endif // TRAJSAMPLER_H
