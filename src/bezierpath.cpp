
#include <math.h>       /* pow, sqrt */

#include "bezierpath.h"

#define distance(x1, y1, x2, y2) (sqrt(pow(x2-x1,2) + pow(y2-y1,2)))
point bezier::pointAt(float t) const
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

float bezier::length(float error) const
{
    float len;
    addifclose(&len, error);
    return len;
}

void bezier::split(bezier *left, bezier *right) const
{
    //"bezsplit" is lifted shamelessly from Schneider's Bezier curve-fitter.
    int   i, j;                               /* Index variables  */
    point Vtemp[4][4];                      /* Triangle Matrix */

    /* Copy control points  */
    Vtemp[0][0].x = ox;
    Vtemp[0][0].y = oy;
    Vtemp[0][1].x = c1x;
    Vtemp[0][1].y = c1y;
    Vtemp[0][2].x = c2x;
    Vtemp[0][2].y = c2y;
    Vtemp[0][3].x = x;
    Vtemp[0][3].y = y;


    /* Triangle computation */
    for (i = 1; i <= 3; i++) {
        for (j =0 ; j <= 3 - i; j++) {
            Vtemp[i][j].x =
                0.5 * Vtemp[i-1][j].x + 0.5 * Vtemp[i-1][j+1].x;
            Vtemp[i][j].y =
                0.5 * Vtemp[i-1][j].y + 0.5 * Vtemp[i-1][j+1].y;
        }
    }

    left->ox = Vtemp[0][0].x;
    left->oy = Vtemp[0][0].y;
    left->c1x = Vtemp[1][0].x;
    left->c1y = Vtemp[1][0].y;
    left->c2x = Vtemp[2][0].x;
    left->c2y = Vtemp[2][0].y;
    left->x =  Vtemp[3][0].x;
    left->y =  Vtemp[3][0].y;

    right->ox = Vtemp[3][0].x;
    right->oy = Vtemp[3][0].y;
    right->c1x = Vtemp[2][1].x;
    right->c1y = Vtemp[2][1].y;
    right->c2x = Vtemp[1][2].x;
    right->c2y = Vtemp[1][2].y;
    right->x =  Vtemp[0][3].x;
    right->y =  Vtemp[0][3].y;
}

void bezier::addifclose(float *length, float error) const
{
    bezier left, right;                  /* bez poly splits */

    float len = distance(ox, oy, c1x, c1y)
        + distance(c1x, c1y, c2x, c2y)
        + distance(c2x, c2y, x, y);

    float chord = distance(ox,oy,x,y);

    if((len-chord) > error)
    {
      split(&left,&right);                 /* split in two */
      left.addifclose(length, error);        /* try left side */
      right.addifclose(length, error);       /* try right side */
      return;
    }

    *length = *length + len;

    return;
}


float bezierpath::length(float error) const
{
    float len = 0.0;
    for (auto c : curves) len += c.length(error);
    return len;
}


