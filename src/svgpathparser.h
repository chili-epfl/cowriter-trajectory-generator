#ifndef SVGPATHPARSER_H
#define SVGPATHPARSER_H

#include  "bezierpath.h"

class QIODevice;
class SvgPathParser
{
public:
    SvgPathParser();

    bool read(QIODevice *device);

    bezierpath path;

private:
    enum mode {MOVE, RELCUBIC, ABSCUBIC};
    bool parsePath(std::string& content);
    point splitToken(std::string& tok);
};

#endif // SVGPATHPARSER_H