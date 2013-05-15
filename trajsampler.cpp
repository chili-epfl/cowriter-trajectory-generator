#include "trajsampler.h"

using namespace std;

TrajSampler::TrajSampler(const bezierpath &path) : bpath(path)
{
}

vector<point> TrajSampler::sample(int density)
{
    vector<point> points;

    for (auto curve : bpath.curves) {
        for (float t = 0.; t < 1 ; t += 1.f/density) {
            point p = curve.pointAt(t);
            p.x += bpath.origin.x;
            p.y += bpath.origin.y;
            points.push_back(p);
        }
    }

    return points;
}
