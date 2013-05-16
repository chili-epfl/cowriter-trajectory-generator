#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <QDomDocument>
#include "svgpathparser.h"

class QString;

using namespace std;

SvgPathParser::SvgPathParser(){}

bool SvgPathParser::read(QIODevice *device)
{
    string pathDesc;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        cerr << "Parse error at line " << errorLine << ", column " << errorColumn << "2:\n" << errorStr.toStdString() << endl;
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "svg") {
       cerr << "This file is not an SVG file!" << endl;
        return false;
    }

    QDomNodeList paths = domDocument.elementsByTagName("path");
    for (int i = 0; i < paths.length(); i++) {
        QDomElement path = paths.at(i).toElement();
        cout << "Found path " << path.attribute("id", "no id").toStdString() << endl;
        pathDesc = path.attribute("d", "").toStdString();
    }
    cout << "Only last one is kept!!" << endl;


    return parsePath(pathDesc);

}

/** SVG Path parser.

**Very incomplete!** Mostly suited to Inkscape output

- it assumes the path starts with M or m, followed by the absolute
position of the curve on the page (ie, for 'm', that the path belongs to
something centered on zero)


- TODO: spaces between the letter of the command and the values is not mandatory! we assume here it exists
*/
bool SvgPathParser::parsePath(std::string &content)
{
    //Tokenize the input
    // stolen from http://stackoverflow.com/questions/236129/splitting-a-string-in-c

    vector<string> tokens;
    istringstream iss(content);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));


    path.curves.clear();

    mode currentmode;
    point prev_point;
    prev_point.x = prev_point.y = 0;
    for (int i = 0; i < tokens.size() ; i++) {
        auto tok = tokens[i];
        if(tok.size() == 1) {// command
            if (tok.compare("M") == 0 or tok.compare("m") == 0) {
                currentmode = MOVE;
                path.origin = splitToken(tokens[i+1]);
                i++;
            }
            else if (tok.compare("C") == 0) {
                currentmode = ABSCUBIC;
            }
            else if (tok.compare("c") == 0) {
                currentmode = RELCUBIC;
            }
            else cerr << "Unknown command! <" << tok << ">" << endl;
        }
        else // coordinates
        {
            bezier curve;
            point c1, c2, p;

            switch(currentmode) {
                case MOVE:
                    cerr << "Multiple coords after moveTo command not supported" << endl;
                    break;
                case ABSCUBIC:
                    c1 = splitToken(tokens[i]);
                    c2 = splitToken(tokens[i+1]);
                    p = splitToken(tokens[i+2]);

                    curve.ox = prev_point.x;
                    curve.oy = prev_point.y;
                    curve.c1x = c1.x - path.origin.x;
                    curve.c1y = c1.y - path.origin.y;
                    curve.c2x = c2.x - path.origin.x;
                    curve.c2y = c2.y - path.origin.y;
                    curve.x = prev_point.x = p.x - path.origin.x;
                    curve.y = prev_point.y = p.y - path.origin.y;

                    path.curves.push_back(curve);

                    i = i+2;
                    break;

                case RELCUBIC:
                    c1 = splitToken(tokens[i]);
                    c2 = splitToken(tokens[i+1]);
                    p = splitToken(tokens[i+2]);

                    curve.ox = prev_point.x;
                    curve.oy = prev_point.y;
                    curve.c1x = c1.x + curve.ox;
                    curve.c1y = c1.y + curve.oy ;
                    curve.c2x = c2.x + curve.ox;
                    curve.c2y = c2.y + curve.oy;
                    curve.x = prev_point.x = p.x + curve.ox;
                    curve.y = prev_point.y = p.y + curve.oy;


                    path.curves.push_back(curve);

                    i = i+2;
                    break;
            }
        }
    }

    return true;
}

point SvgPathParser::splitToken(std::string &tok)
{
    point p;
    size_t pos = tok.find(',');
    p.x = ::atof(tok.substr(0, pos).c_str());
    p.y = ::atof(tok.substr(pos+1, tok.size() - pos).c_str());
    return p;
}

