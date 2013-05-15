
#include <math.h>       /* pow */

#include "bezierpath.h"

point bezier::pointAt(float t)
{
    point p;

    //cf http://en.wikipedia.org/wiki/B%C3%A9zier_curve
    p.x = pow(1 - t, 3) * ox
          + 3 * pow(1 - t, 2) * t * c1x
          + 3 * pow(t, 2) * (1 - t) * c2x
          + pow(t, 3) * x;
    p.y = pow(1 - t, 3) * oy
          + 3 * pow(1 - t, 2) * t * c1y
          + 3 * pow(t, 2) * (1 - t) * c2y
          + pow(t, 3) * y;
    return p;
}



