/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>

#include "mainwindow.h"

#include <QString>
#include <QFile>
#include <iostream>
#include "svgpathparser.h"
#include "trajsampler.h"
#include "dpi.h"

using namespace std;
void usage(char* name) {
    cout << "Usage: " << name << endl;
    cout << "Run a GUI to display and sample trajectories from an SVG file."<< endl << endl;
    cout << "Usage: " << name << " <path.svg> density samplerType"<< endl;
    cout << "Reads the first path from an SVG file and outputs on" << endl;
    cout << "stdout the corresponding trajectory sampled with " << endl;
    cout << "'density' points per cm by a sampler of type samplerType: " << endl;
    cout << "'homogeneous' (uniform) or 'curvature' (sample rate " << endl;
    cout << "proportional to the curvature of the segment)." << endl;
}
int main(int argc, char *argv[])
{
    if (argc == 1){             // use GUI
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        return app.exec();
    }
    else{                       // don't use GUI. use cout for output and cerr for log.

        int density = atoi(argv[2]);
        int iterations = 5;
        SvgPathParser svgPathParser;
        TrajSampler* sampler;

        if (argc != 4) {
            usage(argv[0]);
            exit(1);
        }

        if (string(argv[3]) == "curvature"){
            sampler = new CurvatureSampler();
        }
        else if(string(argv[3]) == "homogeneous"){
            sampler = new HomogeneousSampler();
        }
        else{
            cerr << "Invalid samplerType" << endl;
            return -1;
        }

        string fileName = string(argv[1]);
        QFile file(QString::fromStdString(fileName));
        if (svgPathParser.read(&file)) {
            sampler->setPath(svgPathParser.path);
        }

        auto traj = sampler->sample(density, iterations);
        cout << PX2MM(sampler->getOrigin().x)/1000.0 << "\t" << PX2MM(sampler->getOrigin().y)/1000.0 << endl; // output origin
        for (auto tp : traj) {
            cout << PX2MM(tp.p.x)/1000.0 << "\t" << PX2MM(tp.p.y)/1000.0 << "\t0.0" << endl; // output trajectory
        }

        return 0;
    }

}
