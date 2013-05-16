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
#include <iostream>

#include <QtGui>

#include "renderarea.h"

using namespace std;

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    antialiased = true;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    zoom = 1.0;
    center.setX(width() / 2);
    center.setY(height() / 2);
    showCtlPoints = false;
    showTrajPoints = false;
    showSvg = true;
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::preparePath(const bezierpath &bpath)
{
    path = QPainterPath(); // clear previous paths

    float ox = bpath.origin.x;
    float oy = bpath.origin.y;
    path.moveTo(ox, oy);
    ctlPoints << QPointF(ox, oy);

    for (auto curve : bpath.curves) {
        path.cubicTo(curve.c1x + ox, curve.c1y + oy,
                     curve.c2x + ox, curve.c2y + oy,
                     curve.x + ox, curve.y + oy);
        ctlPoints << QPointF(curve.x + ox, curve.y + oy);
    }

}

void RenderArea::setTrajPoints(const vector<point> &bpoints)
{
    points.clear();
    for (auto p : bpoints) {
        points << QPointF(p.x, p.y);

    }
}



void RenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void RenderArea::drawGrid(QPainter& painter) {

    const int MIN_X = -1000;
    const int MIN_Y = -1000;
    const int MAX_X = 1000;
    const int MAX_Y = 1000;
    const int GRID_SIZE = 100;

    painter.save();
    painter.setPen(QPen(QColor::fromRgbF(0.,0.,0.,0.2)));

    for (int i = MIN_X ; i < MAX_X ; i += GRID_SIZE) {
            painter.drawLine(i,MIN_Y, i, MAX_Y);
            painter.drawText(i + 5,-5,QString::number(i));
    }


    for (int j = MIN_Y ; j < MAX_Y ; j += GRID_SIZE) {
            painter.drawLine(MIN_X, j, MAX_X, j);
            painter.drawText(5,j - 5,QString::number(j));
    }

    painter.restore();
}


void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor::fromRgbF(0.,0.,0.,0.2)));
    painter.setBrush(Qt::NoBrush);
    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    painter.save();
    painter.translate(center);
    painter.scale(zoom, zoom);

    drawGrid(painter);

    if (showSvg) painter.drawPath(path);

    if (showCtlPoints) {
        painter.setPen(QPen(Qt::red, 4, Qt::DashLine, Qt::RoundCap, Qt::MiterJoin));
        painter.drawPoints(ctlPoints);
    }

    if (showTrajPoints) {
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine, Qt::RoundCap, Qt::MiterJoin));
        painter.drawPoints(points);
    }


    painter.restore();

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}