#ifndef SVGPATHPARSER_H
#define SVGPATHPARSER_H

#include  "bezierpath.h"

typedef struct
{
    bool yOriginAtBottom; //y=0 at top (false) or bottom (true) of canvas
} PointProcessingParams;

class QIODevice;
class SvgPathParser
{
public:

    SvgPathParser();

    SvgPathParser(PointProcessingParams pointProcessingParams);

    bool read(QIODevice *device);

    BezierPath path;

private:
    PointProcessingParams pointProcessingParams;
    enum mode {MOVE, RELCUBIC, ABSCUBIC};
    bool parsePath(std::string& content);
    point splitToken(std::string& tok);
    point processPoint(point p);
    float canvasHeight;
};

#endif // SVGPATHPARSER_H
