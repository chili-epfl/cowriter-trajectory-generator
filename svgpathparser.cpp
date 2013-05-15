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

bool SvgPathParser::parsePath(std::string &content)
{
    //Tokenize the input
    // stolen from http://stackoverflow.com/questions/236129/splitting-a-string-in-c

    //TODO: spaces between the letter of the command and the values is not mandatory! we assume here it exists
    vector<string> tokens;
    istringstream iss(content);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));


    for (int i = 0; i < tokens.size() ; i++) {
        auto tok = tokens[i];
        if(tok.size() == 1) {// command
            if (tok.compare("M") == 0) {
                path.origin = splitToken(tokens[i+1]);
                i++;
            }
            else if (tok.compare("C") == 0) {
                bezier curve;
                point c1 = splitToken(tokens[i+1]);
                point c2 = splitToken(tokens[i+2]);
                point p = splitToken(tokens[i+3]);

                curve.c1x = c1.x - path.origin.x;
                curve.c1y = c1.y - path.origin.y;
                curve.c2x = c2.x - path.origin.x;
                curve.c2y = c2.y - path.origin.y;
                curve.x = p.x - path.origin.x;
                curve.y = p.y - path.origin.y;

                path.curves.push_back(curve);

                i = i+3;
            }
            else if (tok.compare("c") == 0) {
                bezier curve;
                point c1 = splitToken(tokens[i+1]);
                point c2 = splitToken(tokens[i+2]);
                point p = splitToken(tokens[i+3]);

                curve.c1x = c1.x;
                curve.c1y = c1.y;
                curve.c2x = c2.x;
                curve.c2y = c2.y;
                curve.x = p.x;
                curve.y = p.y;

                path.curves.push_back(curve);

                i = i+3;
            }
            else cerr << "Unknown command! <" << tok << ">" << endl;
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

